/* main.c - ponto de entrada e loop principal
 *
 * Fluxo de cada quadro:
 *   1. dt = tempo desde o quadro anterior
 *   2. menu (TAB abre e pausa; N/C/S/Q/V dentro do menu)
 *   3. se o menu esta fechado: atualizarJogo (portoes, entregador, zumbis,
 *      regras) e tratamento do resultado (proxima fase, vitoria, derrota)
 *   4. desenho
 *
 * Com o menu aberto nada e atualizado, entao o tempo passado no menu
 * nao e descontado do cronometro (secao 2.1).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "tipos.h"
#include "mapa.h"
#include "render.h"
#include "menu.h"
#include "jogo.h"
#include "zumbi.h"

/* Limite para o dt de um quadro. Se a janela travar (ex.: ao ser arrastada),
 * evita que o jogo "pule" varios segundos de uma vez. */
#define DT_MAXIMO 0.1f

/* Se o monitor for menor que 1600x880, diminui a janela proporcionalmente
 * (permitido pela secao 2.5). */
static void ajustarJanelaAoMonitor(void)
{
    int monitor = GetCurrentMonitor();
    float largMax = GetMonitorWidth(monitor) * 0.95f;
    float altMax = GetMonitorHeight(monitor) * 0.88f;   /* sobra para a barra de tarefas */

    float escala = 1.0f;
    if (largMax < LARGURA_JANELA)
        escala = largMax / LARGURA_JANELA;
    if (altMax < ALTURA_JANELA * escala)
        escala = altMax / ALTURA_JANELA;

    if (escala < 1.0f) {
        int w = (int)(LARGURA_JANELA * escala);
        int h = (int)(ALTURA_JANELA * escala);
        SetWindowSize(w, h);
        SetWindowPosition((GetMonitorWidth(monitor) - w) / 2,
                          (GetMonitorHeight(monitor) - h) / 2);
    }
}

/* Camera que desenha a tela de referencia (1600x880) encaixada na janela
 * atual, mantendo a proporcao e centralizando. Todo o desenho do jogo
 * usa coordenadas de referencia; a camera faz a conversao. */
static Camera2D cameraDaJanela(void)
{
    float escalaX = (float)GetScreenWidth() / LARGURA_JANELA;
    float escalaY = (float)GetScreenHeight() / ALTURA_JANELA;
    float escala = (escalaX < escalaY) ? escalaX : escalaY;

    Camera2D camera = { 0 };
    camera.zoom = escala;
    camera.offset.x = (GetScreenWidth() - LARGURA_JANELA * escala) / 2.0f;
    camera.offset.y = (GetScreenHeight() - ALTURA_JANELA * escala) / 2.0f;
    return camera;
}

/* Fase concluida: tenta abrir mapa(N+1).txt. Se nao existir, venceu. */
static void avancarFase(Jogo *jogo, Menu *menu)
{
    int proxima = jogo->fase + 1;
    ResultadoMapa r = carregarMapa(jogo, proxima);

    if (r == MAPA_OK) {
        /* vidas e pontos sao preservados; registra a nova pontuacao de entrada */
        jogo->pontosEntrada = jogo->pontos;
        reposicionarZumbis(jogo);
    } else if (r == MAPA_INEXISTENTE) {
        encerrarPartida(menu, jogo, FIM_VITORIA);
    } else {
        /* o arquivo existe mas nao pode ser lido: encerra sem travar o programa */
        encerrarPartida(menu, jogo, FIM_NENHUM);
        snprintf(menu->aviso, sizeof(menu->aviso),
                 "Erro ao ler mapa%d.txt. Partida encerrada.", proxima);
        fprintf(stderr, "Erro ao ler mapa%d.txt\n", proxima);
    }
}

static void tratarResultado(Jogo *jogo, Menu *menu, Resultado r)
{
    switch (r) {
        case RES_FASE_CONCLUIDA:
            avancarFase(jogo, menu);
            break;
        case RES_DERROTA_ZUMBI:
            encerrarPartida(menu, jogo, FIM_DERROTA_ZUMBI);
            break;
        case RES_DERROTA_TEMPO:
            encerrarPartida(menu, jogo, FIM_DERROTA_TEMPO);
            break;
        case RES_CONTINUA:
        default:
            break;
    }
}

int main(void)
{
    srand((unsigned)time(NULL));   /* direcoes aleatorias dos zumbis */

    /* todo o estado fica em variaveis locais (sem globais) */
    Jogo jogo = { 0 };   /* ponteiros NULL: liberarFase e segura desde o inicio */
    Menu menu;
    iniciarMenu(&menu);

    /* avisa logo na abertura se o mapa1.txt nao estiver no diretorio */
    FILE *teste = fopen("mapa1.txt", "r");
    if (teste == NULL) {
        snprintf(menu.aviso, sizeof(menu.aviso),
                 "Erro: arquivo mapa1.txt nao encontrado no diretorio de execucao.");
        fprintf(stderr, "Erro: arquivo mapa1.txt nao encontrado.\n");
    } else {
        fclose(teste);
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Resident Evil - Entregador de Suprimentos");
    ajustarJanelaAoMonitor();
    SetExitKey(KEY_NULL);   /* ESC nao fecha o jogo; para sair use Q no menu */
    SetTargetFPS(60);

    while (!WindowShouldClose() && !menu.sair) {
        float dt = GetFrameTime();
        if (dt > DT_MAXIMO)
            dt = DT_MAXIMO;

        processarMenu(&menu, &jogo);

        /* simulacao: so roda com partida ativa e menu fechado (pausa) */
        if (menu.partidaAtiva && !menu.aberto) {
            if (menu.acabouDeFechar) {
                /* descarta o quadro em que o menu fechou */
                menu.acabouDeFechar = 0;
            } else {
                if (jogo.tempoMensagem > 0.0f)
                    jogo.tempoMensagem -= dt;

                Resultado r = atualizarJogo(&jogo, dt);
                tratarResultado(&jogo, &menu, r);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cameraDaJanela());

        if (menu.partidaAtiva)
            desenharJogo(&jogo);
        if (menu.aberto)
            desenharMenu(&menu, &jogo);

        EndMode2D();
        EndDrawing();
    }

    /* liberacao final da memoria */
    liberarFase(&jogo);
    CloseWindow();
    return 0;
}
