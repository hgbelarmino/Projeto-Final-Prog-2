#ifndef MAPA_H
#define MAPA_H

#include "tipos.h"

/* Le mapaN.txt e preenche jogo (grade, kits, zumbis, jogador, tempo).
 * Mantem vidas/pontos/fase ja existentes em jogo.
 * Retorna 1 em sucesso; 0 se o arquivo nao existe; -1 em erro de leitura/memoria.
 * Em falha, jogo fica sem memoria alocada (seguro chamar liberarFase). */
int carregarMapa(Jogo *jogo, int numero);

/* Libera grade, kits e zumbis. Pode ser chamada varias vezes. */
void liberarFase(Jogo *jogo);

#endif
