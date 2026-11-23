#include "render.h"

#include <stdio.h>
#include "raylib.h"

static Color corDoKit(char tipo) {
    switch (tipo) {
        case 'a': case 'A': return (Color){ 70, 170, 230, 255 };
        case 'b': case 'B': return (Color){ 240, 160, 60, 255 };
        default:            return (Color){ 180, 110, 220, 255 };
    }
}

static void desenharTerreno(const Jogo *jogo) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            int x = j * TAM_BLOCO, y = i * TAM_BLOCO;
            char c = jogo->grade[i][j];
            Color fundo = (Color){ 235, 238, 240, 255 };
            if (c == '#') fundo = (Color){ 35, 50, 80, 255 };
            DrawRectangle(x, y, TAM_BLOCO, TAM_BLOCO, fundo);
            if (c != '#') DrawRectangleLines(x, y, TAM_BLOCO, TAM_BLOCO, (Color){ 215, 220, 225, 255 });

            if (c == 'A' || c == 'B' || c == 'C') {
                DrawRectangle(x + 4, y + 4, TAM_BLOCO - 8, TAM_BLOCO - 8, corDoKit(c));
                DrawText(TextFormat("%c", c), x + 14, y + 10, 20, WHITE);
            } else if (c == 'D') {
                Color cor = jogo->portoesAbertos ? (Color){ 120, 220, 190, 255 } : (Color){ 200, 60, 60, 255 };
                DrawRectangle(x + 2, y + 2, TAM_BLOCO - 4, TAM_BLOCO - 4, cor);
                DrawText("D", x + 14, y + 10, 20, WHITE);
            } else if (c == 'S') {
                Color cor = jogo->saidaLiberada ? (Color){ 60, 190, 90, 255 } : GRAY;
                DrawRectangle(x + 4, y + 4, TAM_BLOCO - 8, TAM_BLOCO - 8, cor);
                DrawText("S", x + 14, y + 10, 20, WHITE);
            }
        }
    }
}

static void desenharEntidades(const Jogo *jogo) {
    for (int k = 0; k < jogo->nKits; k++) {
        const Kit *kit = &jogo->kits[k];
        if (kit->estado != KIT_CHAO) continue;
        int x = kit->original.coluna * TAM_BLOCO, y = kit->original.linha * TAM_BLOCO;
        DrawRectangle(x + 11, y + 11, 18, 18, corDoKit(kit->tipo));
        DrawText(TextFormat("%c", kit->tipo), x + 15, y + 12, 16, WHITE);
    }
    for (int z = 0; z < jogo->nZumbis; z++) {
        int cx = jogo->zumbis[z].atual.coluna * TAM_BLOCO + TAM_BLOCO / 2;
        int cy = jogo->zumbis[z].atual.linha * TAM_BLOCO + TAM_BLOCO / 2;
        DrawCircle(cx, cy, 14, (Color){ 90, 170, 60, 255 });
        DrawText("Z", cx - 6, cy - 9, 18, WHITE);
    }
    int px = jogo->jogador.atual.coluna * TAM_BLOCO + TAM_BLOCO / 2;
    int py = jogo->jogador.atual.linha * TAM_BLOCO + TAM_BLOCO / 2;
    DrawCircle(px, py, 14, (Color){ 250, 205, 50, 255 });
    DrawText("P", px - 6, py - 9, 18, BLACK);
}

static void desenharHud(const Jogo *jogo) {
    int y0 = LINHAS * TAM_BLOCO;
    DrawRectangle(0, y0, LARGURA_TELA, ALTURA_HUD, (Color){ 20, 28, 40, 255 });

    int seg = (int)(jogo->tempoRestante + 0.999f);   /* arredonda para cima */
    if (seg < 0) seg = 0;
    int pendentes = 0;
    for (int k = 0; k < jogo->nKits; k++)
        if (jogo->kits[k].estado != KIT_ENTREGUE) pendentes++;

    DrawText(TextFormat("VIDAS %d   PONTOS %06d   FASE %d   TEMPO %02d:%02d   KITS PENDENTES %d",
                        jogo->vidas, jogo->pontos, jogo->fase, seg / 60, seg % 60, pendentes),
             20, y0 + 8, 24, WHITE);

    /* Linha 2: carga da base ao topo */
    DrawText("CARGA (base->topo):", 20, y0 + 48, 20, LIGHTGRAY);
    int x = 260;
    if (jogo->jogador.topo == 0) {
        DrawText("vazia (0/3)", x, y0 + 48, 20, LIGHTGRAY);
    }
    for (int i = 0; i < jogo->jogador.topo; i++) {
        const Kit *kit = &jogo->kits[jogo->jogador.pilha[i]];
        DrawRectangle(x + i * 40, y0 + 44, 32, 28, corDoKit(kit->tipo));
        DrawText(TextFormat("%c", kit->tipo), x + i * 40 + 11, y0 + 48, 20, WHITE);
        if (i == jogo->jogador.topo - 1)
            DrawRectangleLines(x + i * 40 - 2, y0 + 42, 36, 32, YELLOW);   /* destaque do topo */
    }
    DrawText(jogo->portoesAbertos ? "PORTOES: abertos" : "PORTOES: fechados", 520, y0 + 48, 20, LIGHTGRAY);
    DrawText("TAB: menu   ESPACO: coletar/entregar", 800, y0 + 48, 20, LIGHTGRAY);
    if (jogo->mensagem[0] != '\0') DrawText(jogo->mensagem, 1250, y0 + 8, 20, ORANGE);
}

void desenharJogo(const Jogo *jogo) {
    desenharTerreno(jogo);
    desenharEntidades(jogo);
    desenharHud(jogo);
}
