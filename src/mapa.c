/* mapa.c - leitura dos arquivos de mapa e alocacao dinamica
 *
 * Formato do mapaN.txt (secao 2.2):
 *   linha 1: tempo limite em segundos
 *   linhas 2 a 21: 20 linhas com 40 caracteres cada (Tabela 1)
 *
 * O terreno (paredes, piso, abrigos, portoes, saida) fica na grade.
 * Kits, zumbis e o entregador sao entidades separadas: a celula onde
 * estavam no arquivo vira piso na grade.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mapa.h"

#define TAM_BUFFER 256   /* maior que 40 para aceitar \r\n e lixo no fim */

/* ----- alocacao da grade ----- */

char **alocarGrade(void)
{
    char **grade = malloc(LINHAS * sizeof(char *));
    if (grade == NULL)
        return NULL;

    for (int i = 0; i < LINHAS; i++) {
        grade[i] = malloc(COLUNAS * sizeof(char));
        if (grade[i] == NULL) {
            /* desfaz o que ja foi alocado */
            for (int j = 0; j < i; j++)
                free(grade[j]);
            free(grade);
            return NULL;
        }
        memset(grade[i], CEL_PISO, COLUNAS);
    }
    return grade;
}

void liberarGrade(char **grade)
{
    if (grade == NULL)
        return;
    for (int i = 0; i < LINHAS; i++)
        free(grade[i]);
    free(grade);
}

void liberarFase(Jogo *jogo)
{
    liberarGrade(jogo->grade);
    free(jogo->kits);
    free(jogo->zumbis);

    jogo->grade = NULL;
    jogo->kits = NULL;
    jogo->zumbis = NULL;
    jogo->numKits = 0;
    jogo->numZumbis = 0;
}

/* ----- leitura do arquivo ----- */

/* Remove \n e \r do fim da string (arquivos criados no Windows usam \r\n). */
static void removerQuebraDeLinha(char *s)
{
    int n = (int)strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

/* Le as 20 linhas do mapa para 'texto' (matriz fixa temporaria).
 * Linhas com menos de 40 caracteres sao completadas com espacos,
 * pois alguns editores apagam espacos do fim da linha.
 * Retorna 1 se conseguiu ler todas as linhas, 0 caso contrario. */
static int lerLinhas(FILE *arq, char texto[LINHAS][COLUNAS])
{
    char buffer[TAM_BUFFER];

    for (int i = 0; i < LINHAS; i++) {
        if (fgets(buffer, TAM_BUFFER, arq) == NULL)
            return 0;   /* arquivo terminou antes da hora */
        removerQuebraDeLinha(buffer);

        int tam = (int)strlen(buffer);
        for (int j = 0; j < COLUNAS; j++)
            texto[i][j] = (j < tam) ? buffer[j] : CEL_PISO;
    }
    return 1;
}

static int ehKit(char c)
{
    return c == 'a' || c == 'b' || c == 'c';
}

ResultadoMapa carregarMapa(Jogo *jogo, int numero)
{
    char nomeArquivo[32];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "mapa%d.txt", numero);

    /* 1. abrir o arquivo */
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL)
        return MAPA_INEXISTENTE;

    /* 2. tempo limite (primeira linha) */
    char buffer[TAM_BUFFER];
    int tempoLimite = 0;
    if (fgets(buffer, TAM_BUFFER, arq) == NULL ||
        sscanf(buffer, "%d", &tempoLimite) != 1 || tempoLimite <= 0) {
        fclose(arq);
        return MAPA_ERRO_LEITURA;
    }

    /* 3. as 20 linhas do mapa */
    char texto[LINHAS][COLUNAS];
    int leuTudo = lerLinhas(arq, texto);
    fclose(arq);
    if (!leuTudo)
        return MAPA_ERRO_LEITURA;

    /* 4. contar kits, zumbis e entregadores */
    int numKits = 0, numZumbis = 0, numP = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (ehKit(texto[i][j]))
                numKits++;
            else if (texto[i][j] == 'Z')
                numZumbis++;
            else if (texto[i][j] == 'P')
                numP++;
        }
    }
    if (numP != 1 || numKits == 0)
        return MAPA_ERRO_LEITURA;

    /* 5. alocar grade e vetores */
    char **grade = alocarGrade();
    Kit *kits = malloc(numKits * sizeof(Kit));
    /* malloc(0) pode retornar NULL; so aloca zumbis se houver algum */
    Zumbi *zumbis = (numZumbis > 0) ? malloc(numZumbis * sizeof(Zumbi)) : NULL;

    if (grade == NULL || kits == NULL || (numZumbis > 0 && zumbis == NULL)) {
        liberarGrade(grade);
        free(kits);
        free(zumbis);
        return MAPA_ERRO_MEMORIA;
    }

    /* 6. separar terreno e entidades */
    Pos inicioP = {0, 0};
    int k = 0, z = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            char c = texto[i][j];
            Pos p = {i, j};

            if (ehKit(c)) {
                kits[k].id = k;
                kits[k].tipo = (char)(c - 'a' + 'A');   /* 'a' -> 'A' */
                kits[k].original = p;
                kits[k].estado = KIT_NO_CHAO;
                k++;
                grade[i][j] = CEL_PISO;
            } else if (c == 'Z') {
                zumbis[z].pos = p;
                zumbis[z].inicial = p;
                zumbis[z].anterior = p;
                zumbis[z].direcao = DIR_NENHUMA;   /* sorteada por reposicionarZumbis */
                zumbis[z].espera = ESPERA_INICIAL_ZUMBI;
                z++;
                grade[i][j] = CEL_PISO;
            } else if (c == 'P') {
                inicioP = p;
                grade[i][j] = CEL_PISO;
            } else {
                grade[i][j] = c;   /* ' ', '#', 'A', 'B', 'C', 'D', 'S' */
            }
        }
    }

    /* 7. deu tudo certo: troca a fase antiga pela nova */
    liberarFase(jogo);

    jogo->grade = grade;
    jogo->kits = kits;
    jogo->numKits = numKits;
    jogo->zumbis = zumbis;
    jogo->numZumbis = numZumbis;

    jogo->entregador.pos = inicioP;
    jogo->entregador.inicial = inicioP;
    jogo->entregador.anterior = inicioP;
    jogo->entregador.espera = 0.0f;
    jogo->entregador.topo = 0;

    jogo->fase = numero;
    jogo->tempoLimite = (float)tempoLimite;
    jogo->tempoRestante = (float)tempoLimite;
    jogo->tempoPortao = 0.0f;
    jogo->portoesAbertos = 1;     /* o ciclo comeca com os portoes abertos */
    jogo->saidaLiberada = 0;
    jogo->mensagem[0] = '\0';
    jogo->tempoMensagem = 0.0f;

    return MAPA_OK;
}
