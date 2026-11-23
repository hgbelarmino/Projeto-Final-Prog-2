/* main.c - ponto de entrada e loop principal
 *
 * Aqui vai:
 *  - declarar o Jogo local e inicializar (fase 1, 3 vidas, 0 pontos)
 *  - carregar mapa1.txt; se falhar, mensagem de erro e encerrar
 *  - InitWindow / SetTargetFPS
 *  - loop (enquanto a janela nao fecha):
 *      1. dt = GetFrameTime()
 *      2. tratar TAB / menu (pausa)
 *      3. se nao pausado: atualizar portoes, entregador, zumbis, regras
 *      4. se terminou a fase: tentar mapa(N+1).txt; se nao existir, vitoria
 *      5. desenhar
 *  - CloseWindow e liberar toda a memoria no fim
 */

#include "tipos.h"
