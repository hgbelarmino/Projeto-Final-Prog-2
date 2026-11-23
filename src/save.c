/* save.c - salvamento e restauracao em save.bin
 *
 * O que precisa ser gravado:
 *  - fase, limite de tempo, vidas, pontos, pontos de entrada
 *  - tempo restante, posicao no ciclo dos portoes, intervalos parciais
 *  - entregador (posicoes, espera, pilha e topo)
 *  - zumbis (posicoes, direcao, espera) e quantidade
 *  - kits (id, tipo, posicao original, estado) e quantidade
 *  - a grade do terreno
 *
 * Aqui vai:
 *  - salvarJogo
 *  - carregarJogo (carrega em uma variavel temporaria e so substitui o estado
 *    atual se tudo deu certo)
 */

#include "save.h"
