/* jogo.c - regras do entregador, pilha, pontuacao, vidas e tempo
 *
 * Aqui vai:
 *  - tabela de intervalos de passo por quantidade de kits (0.25, 0.33, 0.5, 1.0)
 *  - funcoes da pilha: empilhar, desempilhar, consultar topo
 *  - movimentacao do entregador
 *  - coleta e entrega (espaco), com pontuacao (+100 por entrega)
 *  - deteccao de contato com zumbi (3 casos)
 *  - perda de vida: -50 pontos (minimo 0), reposicionar entregador e zumbis,
 *    esvaziar pilha devolvendo os kits as posicoes originais
 *  - fim do tempo: perde vida, reinicia a fase inteira, restaura pontuacao de
 *    entrada, sem a penalidade de 50
 *  - liberar a saida quando nao houver mais kits pendentes
 *  - chegar na saida: soma o tempo restante e avanca de fase
 */

#include "jogo.h"
