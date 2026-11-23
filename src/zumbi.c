/* zumbi.c - zumbis e portoes
 *
 * Aqui vai:
 *  - funcao auxiliar: celula e transitavel? (parede, limites, portao fechado)
 *  - funcao auxiliar: listar direcoes validas a partir de uma posicao
 *  - escolher direcao aleatoria (rand)
 *  - atualizarZumbis (movimento, beco sem saida, zumbi parado se nao ha saida)
 *  - reposicionarZumbis
 *  - atualizarPortoes (ciclo de 8 s)
 *  - regra de quem esta dentro do portao quando ele fecha: pode sair, nao pode
 *    entrar de novo ate reabrir
 */

#include "zumbi.h"
