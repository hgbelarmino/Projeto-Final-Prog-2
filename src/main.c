#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "tipos.h"
#include "mapa.h"
#include "render.h"

int main(void) {
    Jogo jogo;
    memset(&jogo, 0, sizeof(jogo));
    jogo.fase = 1;
    jogo.vidas = VIDAS_INICIAIS;
    jogo.pontos = 0;
    jogo.pontosEntrada = 0;

    int r = carregarMapa(&jogo, jogo.fase);
    if (r == 0) {
        fprintf(stderr, "Erro: mapa1.txt nao encontrado no diretorio de execucao.\n");
        return 1;
    }
    if (r < 0) {
        fprintf(stderr, "Erro: falha ao ler mapa1.txt (formato invalido ou sem memoria).\n");
        return 1;
    }

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Resident Evil - Prog II");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        /* TODO (Pessoa B): atualizar entregador, tempo, colisoes
         * TODO (Pessoa C): atualizar zumbis, portoes, menu */
        BeginDrawing();
        ClearBackground(BLACK);
        desenharJogo(&jogo);
        EndDrawing();
    }

    CloseWindow();
    liberarFase(&jogo);
    return 0;
}
