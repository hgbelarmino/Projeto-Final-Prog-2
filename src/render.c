/* render.c - desenho com raylib
 *
 * Aqui vai:
 *  - funcao auxiliar: cor de cada tipo de kit/abrigo (a/b/c)
 *  - desenharTerreno:
 *      paredes, piso, abrigos A/B/C, portoes (aberto x fechado), saida
 *      (bloqueada x liberada)
 *  - desenharEntidades:
 *      kits no chao, zumbis (circulos verdes), entregador (circulo amarelo)
 *  - desenharHud (faixa inferior, 2 linhas de 40 px):
 *      vidas, pontos com 6 digitos, fase, tempo mm:ss (arredondado para cima),
 *      kits pendentes, carga da base ao topo com destaque no topo,
 *      estado dos portoes, dica de controles, mensagem de aviso
 *  - desenharJogo: chama as tres acima
 */

#include "render.h"
