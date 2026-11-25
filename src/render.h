#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "tipos.h"

/* Cor associada a cada tipo de kit/abrigo ('A', 'B' ou 'C'). */
Color corDoKit(char tipo);

/* Desenha paredes, piso, abrigos, portoes e saida. */
void desenharTerreno(const Jogo *jogo);

/* Desenha kits no chao, zumbis e entregador. */
void desenharEntidades(const Jogo *jogo);

/* Desenha a faixa inferior com vidas, pontos, fase, tempo, kits, carga etc. */
void desenharHud(const Jogo *jogo);

/* Desenha terreno, entidades e HUD.
 * Deve ser chamada entre BeginDrawing/EndDrawing (e dentro do BeginMode2D
 * usado para escalar a janela, ver main.c). */
void desenharJogo(const Jogo *jogo);

#endif
