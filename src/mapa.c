#include "mapa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void liberarFase(Jogo *jogo) {
    if (jogo->grade != NULL) {
        for (int i = 0; i < LINHAS; i++) free(jogo->grade[i]);
        free(jogo->grade);
        jogo->grade = NULL;
    }
    free(jogo->kits);
    jogo->kits = NULL;
    jogo->nKits = 0;
    free(jogo->zumbis);
    jogo->zumbis = NULL;
    jogo->nZumbis = 0;
}

static int alocarGrade(Jogo *jogo) {
    jogo->grade = calloc(LINHAS, sizeof(char *));
    if (jogo->grade == NULL) return 0;
    for (int i = 0; i < LINHAS; i++) {
        jogo->grade[i] = malloc(COLUNAS);
        if (jogo->grade[i] == NULL) return 0;
    }
    return 1;
}

int carregarMapa(Jogo *jogo, int numero) {
    char nome[32];
    snprintf(nome, sizeof(nome), "mapa%d.txt", numero);

    FILE *f = fopen(nome, "r");
    if (f == NULL) return 0;

    liberarFase(jogo);

    int tempo = 0;
    char linha[COLUNAS + 8];

    if (fgets(linha, sizeof(linha), f) == NULL || sscanf(linha, "%d", &tempo) != 1 || tempo <= 0) {
        fclose(f);
        return -1;
    }
    if (!alocarGrade(jogo)) {
        fclose(f);
        liberarFase(jogo);
        return -1;
    }

    /* Le a matriz crua e conta kits e zumbis. */
    char **cru = malloc(LINHAS * sizeof(char *));
    if (cru == NULL) { fclose(f); liberarFase(jogo); return -1; }
    int lidas = 0, erro = 0, nKits = 0, nZumbis = 0;
    for (int i = 0; i < LINHAS; i++) cru[i] = NULL;

    for (int i = 0; i < LINHAS && !erro; i++) {
        cru[i] = malloc(COLUNAS);
        if (cru[i] == NULL || fgets(linha, sizeof(linha), f) == NULL) { erro = 1; break; }
        size_t n = strcspn(linha, "\r\n");   /* tolera \r\n */
        if (n != COLUNAS) { erro = 1; break; }
        memcpy(cru[i], linha, COLUNAS);
        lidas++;
        for (int j = 0; j < COLUNAS; j++) {
            char c = cru[i][j];
            if (c == 'a' || c == 'b' || c == 'c') nKits++;
            else if (c == 'Z') nZumbis++;
        }
    }
    fclose(f);

    if (!erro) {
        jogo->kits = malloc((size_t)nKits * sizeof(Kit));
        jogo->zumbis = malloc((size_t)nZumbis * sizeof(Zumbi));
        if ((nKits > 0 && jogo->kits == NULL) || (nZumbis > 0 && jogo->zumbis == NULL)) erro = 1;
    }

    if (!erro) {
        int k = 0, z = 0, achouP = 0;
        for (int i = 0; i < LINHAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                char c = cru[i][j];
                Pos p = { i, j };
                jogo->grade[i][j] = c;
                if (c == 'a' || c == 'b' || c == 'c') {
                    jogo->kits[k].id = k;
                    jogo->kits[k].tipo = c;
                    jogo->kits[k].original = p;
                    jogo->kits[k].estado = KIT_CHAO;
                    k++;
                    jogo->grade[i][j] = ' ';
                } else if (c == 'Z') {
                    jogo->zumbis[z].atual = p;
                    jogo->zumbis[z].inicial = p;
                    jogo->zumbis[z].anterior = p;
                    jogo->zumbis[z].dir = DIR_NENHUMA;
                    jogo->zumbis[z].espera = ZUMBI_ESPERA_INICIAL_S;
                    z++;
                    jogo->grade[i][j] = ' ';
                } else if (c == 'P') {
                    jogo->jogador.atual = p;
                    jogo->jogador.inicial = p;
                    jogo->jogador.anterior = p;
                    achouP = 1;
                    jogo->grade[i][j] = ' ';
                }
            }
        }
        jogo->nKits = k;
        jogo->nZumbis = z;
        if (!achouP) erro = 1;
    }

    for (int i = 0; i < LINHAS; i++) free(cru[i]);
    free(cru);
    (void)lidas;

    if (erro) {
        liberarFase(jogo);
        return -1;
    }

    jogo->limiteTempo = (float)tempo;
    jogo->tempoRestante = (float)tempo;
    jogo->tempoPortoes = 0.0f;
    jogo->portoesAbertos = 1;
    jogo->saidaLiberada = 0;
    jogo->jogador.topo = 0;
    jogo->jogador.espera = 0.0f;
    jogo->mensagem[0] = '\0';
    return 1;
}
