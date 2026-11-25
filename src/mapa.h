#ifndef MAPA_H
#define MAPA_H

#include "tipos.h"

/* Codigos de retorno de carregarMapa */
typedef enum {
    MAPA_OK,
    MAPA_INEXISTENTE,    /* fopen falhou: arquivo nao existe */
    MAPA_ERRO_LEITURA,   /* arquivo existe, mas o conteudo esta incompleto/invalido */
    MAPA_ERRO_MEMORIA    /* malloc falhou */
} ResultadoMapa;

/* Le mapaN.txt (N = numero) e monta a fase em *jogo:
 *   - tempo limite, grade do terreno, kits, zumbis e posicao do entregador
 *   - zera cronometro, portoes, saida e pilha
 * Nao altera vidas nem pontos.
 * Se der qualquer erro, *jogo NAO e modificado (a fase atual continua valida). */
ResultadoMapa carregarMapa(Jogo *jogo, int numero);

/* Libera grade, kits e zumbis. Pode ser chamada mais de uma vez
 * (os ponteiros ficam NULL depois de liberados). */
void liberarFase(Jogo *jogo);

/* Aloca uma grade LINHAS x COLUNAS preenchida com piso.
 * Retorna NULL se faltar memoria (nada fica alocado nesse caso). */
char **alocarGrade(void);

/* Libera uma grade criada por alocarGrade (aceita NULL). */
void liberarGrade(char **grade);

#endif
