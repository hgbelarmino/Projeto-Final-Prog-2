/* render.c - desenho com raylib
 *
 * Todas as coordenadas usam a janela de referencia 1600 x 880 (secao 2.5).
 * Se a janela real for menor, o main.c aplica uma Camera2D com zoom,
 * entao aqui nao precisamos nos preocupar com escala.
 *
 * Estas funcoes so LEEM o jogo (const Jogo *): desenhar nunca altera o estado.
 */

#include <stdio.h>
#include <math.h>

#include "render.h"

/* ----- paleta ----- */
#define COR_FUNDO_HUD    (Color){ 24, 30, 40, 255 }
#define COR_PAREDE       (Color){ 52, 70, 95, 255 }
#define COR_PISO         (Color){ 228, 232, 236, 255 }
#define COR_LINHA_PISO   (Color){ 205, 211, 218, 255 }
#define COR_PORTAO_ABERTO (Color){ 170, 225, 205, 255 }
#define COR_PORTAO_FECHADO (Color){ 120, 72, 40, 255 }
#define COR_SAIDA_BLOQ   (Color){ 150, 150, 150, 255 }
#define COR_SAIDA_LIVRE  (Color){ 60, 190, 90, 255 }
#define COR_ZUMBI        (Color){ 110, 170, 60, 255 }
#define COR_ENTREGADOR   (Color){ 250, 210, 60, 255 }
#define COR_TEXTO_HUD    RAYWHITE
#define COR_TEXTO_FRACO  (Color){ 160, 170, 185, 255 }

#define FONTE_HUD     22
#define FONTE_HUD_2   18

/* ----- auxiliares ----- */

Color corDoKit(char tipo)
{
    switch (tipo) {
        case 'A': return (Color){ 70, 170, 230, 255 };   /* azul */
        case 'B': return (Color){ 240, 160, 70, 255 };   /* laranja */
        case 'C': return (Color){ 180, 130, 220, 255 };  /* roxo */
        default:  return GRAY;
    }
}

/* Canto superior esquerdo da celula em pixels. */
static int xCelula(int coluna) { return coluna * TAM_BLOCO; }
static int yCelula(int linha)  { return linha * TAM_BLOCO; }

/* Centro da celula em pixels (para os circulos). */
static Vector2 centroCelula(Pos p)
{
    Vector2 c = { xCelula(p.coluna) + TAM_BLOCO / 2.0f,
                  yCelula(p.linha) + TAM_BLOCO / 2.0f };
    return c;
}

/* Escreve 'texto' centralizado em um quadrado de lado 'lado' com canto (x, y). */
static void textoCentralizado(const char *texto, int x, int y, int lado,
                              int fonte, Color cor)
{
    int largura = MeasureText(texto, fonte);
    DrawText(texto, x + (lado - largura) / 2, y + (lado - fonte) / 2, fonte, cor);
}

static int entregadorEm(const Jogo *jogo, int linha, int coluna)
{
    return jogo->entregador.pos.linha == linha &&
           jogo->entregador.pos.coluna == coluna;
}

/* ----- terreno ----- */

static void desenharAbrigo(const Jogo *jogo, char tipo, int i, int j)
{
    int x = xCelula(j), y = yCelula(i);
    int margem = 3;
    int lado = TAM_BLOCO - 2 * margem;
    Rectangle r = { x + margem, y + margem, lado, lado };

    DrawRectangleRec(r, corDoKit(tipo));
    /* contorno muda quando o entregador esta em cima (abrigo continua visivel) */
    if (entregadorEm(jogo, i, j))
        DrawRectangleLinesEx(r, 4, YELLOW);
    else
        DrawRectangleLinesEx(r, 2, DARKGRAY);
    char letra[2] = { tipo, '\0' };
    textoCentralizado(letra, x + margem, y + margem, lado, 24, BLACK);
}

static void desenharPortao(const Jogo *jogo, int i, int j)
{
    int x = xCelula(j), y = yCelula(i);

    if (jogo->portoesAbertos) {
        /* aberto: piso com moldura clara */
        DrawRectangle(x + 4, y + 4, TAM_BLOCO - 8, TAM_BLOCO - 8, COR_PORTAO_ABERTO);
        DrawRectangleLines(x + 4, y + 4, TAM_BLOCO - 8, TAM_BLOCO - 8, DARKGREEN);
        textoCentralizado("D", x, y, TAM_BLOCO, 18, DARKGREEN);
    } else {
        /* fechado: bloco escuro com grades verticais */
        DrawRectangle(x, y, TAM_BLOCO, TAM_BLOCO, COR_PORTAO_FECHADO);
        for (int b = 1; b <= 3; b++)
            DrawRectangle(x + b * TAM_BLOCO / 4 - 2, y + 2, 4, TAM_BLOCO - 4, BLACK);
    }
}

static void desenharSaida(const Jogo *jogo, int i, int j)
{
    int x = xCelula(j), y = yCelula(i);
    Rectangle r = { x + 3, y + 3, TAM_BLOCO - 6, TAM_BLOCO - 6 };

    DrawRectangleRec(r, jogo->saidaLiberada ? COR_SAIDA_LIVRE : COR_SAIDA_BLOQ);
    if (entregadorEm(jogo, i, j))
        DrawRectangleLinesEx(r, 4, YELLOW);
    else
        DrawRectangleLinesEx(r, 2, BLACK);
    textoCentralizado("S", x, y, TAM_BLOCO, 22, BLACK);
}

void desenharTerreno(const Jogo *jogo)
{
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            int x = xCelula(j), y = yCelula(i);
            char c = jogo->grade[i][j];

            if (c == CEL_PAREDE) {
                DrawRectangle(x, y, TAM_BLOCO, TAM_BLOCO, COR_PAREDE);
                continue;
            }

            /* tudo que nao e parede tem piso por baixo */
            DrawRectangle(x, y, TAM_BLOCO, TAM_BLOCO, COR_PISO);
            DrawRectangleLines(x, y, TAM_BLOCO, TAM_BLOCO, COR_LINHA_PISO);

            if (c == 'A' || c == 'B' || c == 'C')
                desenharAbrigo(jogo, c, i, j);
            else if (c == CEL_PORTAO)
                desenharPortao(jogo, i, j);
            else if (c == CEL_SAIDA)
                desenharSaida(jogo, i, j);
        }
    }
}

/* ----- entidades ----- */

void desenharEntidades(const Jogo *jogo)
{
    /* kits no chao: quadrados pequenos com a letra minuscula */
    int ladoKit = 20;
    for (int k = 0; k < jogo->numKits; k++) {
        const Kit *kit = &jogo->kits[k];
        if (kit->estado != KIT_NO_CHAO)
            continue;

        int x = xCelula(kit->original.coluna) + (TAM_BLOCO - ladoKit) / 2;
        int y = yCelula(kit->original.linha) + (TAM_BLOCO - ladoKit) / 2;
        DrawRectangle(x, y, ladoKit, ladoKit, corDoKit(kit->tipo));
        DrawRectangleLines(x, y, ladoKit, ladoKit, BLACK);
        char letra[2] = { (char)(kit->tipo - 'A' + 'a'), '\0' };
        textoCentralizado(letra, x, y, ladoKit, 16, BLACK);
    }

    /* zumbis: circulos verdes */
    for (int z = 0; z < jogo->numZumbis; z++) {
        Vector2 c = centroCelula(jogo->zumbis[z].pos);
        DrawCircleV(c, 13, COR_ZUMBI);
        DrawCircleLinesV(c, 13, DARKGREEN);
        textoCentralizado("Z", (int)c.x - 13, (int)c.y - 13, 26, 16, BLACK);
    }

    /* entregador: circulo amarelo (desenhado por ultimo, fica por cima) */
    Vector2 c = centroCelula(jogo->entregador.pos);
    DrawCircleV(c, 14, COR_ENTREGADOR);
    DrawCircleLinesV(c, 14, BROWN);
    textoCentralizado("P", (int)c.x - 14, (int)c.y - 14, 28, 16, BLACK);
}

/* ----- HUD ----- */

static int contarKitsPendentes(const Jogo *jogo)
{
    /* pendentes = no chao + carregados (so a entrega reduz) */
    int n = 0;
    for (int k = 0; k < jogo->numKits; k++)
        if (jogo->kits[k].estado != KIT_ENTREGUE)
            n++;
    return n;
}

/* Desenha a carga da base para o topo; o topo ganha contorno amarelo.
 * Retorna a coordenada x onde o desenho terminou. */
static int desenharCarga(const Jogo *jogo, int x, int y)
{
    const Entregador *e = &jogo->entregador;
    char texto[32];

    DrawText("CARGA (base->topo):", x, y, FONTE_HUD_2, COR_TEXTO_FRACO);
    x += MeasureText("CARGA (base->topo):", FONTE_HUD_2) + 10;

    if (e->topo == 0) {
        DrawText("vazia", x, y, FONTE_HUD_2, COR_TEXTO_HUD);
        x += MeasureText("vazia", FONTE_HUD_2) + 10;
    } else {
        for (int i = 0; i < e->topo; i++) {
            char tipo = jogo->kits[e->pilha[i]].tipo;
            Rectangle r = { x, y - 2, 24, 22 };
            DrawRectangleRec(r, corDoKit(tipo));
            if (i == e->topo - 1)
                DrawRectangleLinesEx(r, 3, YELLOW);   /* destaque do topo */
            char letra[2] = { tipo, '\0' };
            textoCentralizado(letra, x, y - 2, 24, 16, BLACK);
            x += 30;
        }
        DrawText("<- topo", x, y, FONTE_HUD_2, YELLOW);
        x += MeasureText("<- topo", FONTE_HUD_2) + 10;
    }

    snprintf(texto, sizeof(texto), "(%d/%d)", e->topo, MAX_PILHA);
    DrawText(texto, x, y, FONTE_HUD_2, COR_TEXTO_HUD);
    return x + MeasureText(texto, FONTE_HUD_2);
}

void desenharHud(const Jogo *jogo)
{
    int yHud = LINHAS * TAM_BLOCO;
    DrawRectangle(0, yHud, LARGURA_JANELA, ALTURA_HUD, COR_FUNDO_HUD);

    /* tempo exibido arredondado para cima e limitado a zero (secao 2.13) */
    int segundos = (int)ceilf(jogo->tempoRestante);
    if (segundos < 0)
        segundos = 0;

    /* linha 1 */
    char linha1[160];
    snprintf(linha1, sizeof(linha1),
             "VIDAS %d     PONTOS %06d     FASE %d     TEMPO %02d:%02d     KITS PENDENTES %d",
             jogo->vidas, jogo->pontos, jogo->fase,
             segundos / 60, segundos % 60, contarKitsPendentes(jogo));
    DrawText(linha1, 20, yHud + 10, FONTE_HUD, COR_TEXTO_HUD);

    /* linha 2 */
    int y2 = yHud + 48;
    int x = desenharCarga(jogo, 20, y2) + 40;

    char portoes[32];
    snprintf(portoes, sizeof(portoes), "PORTOES: %s",
             jogo->portoesAbertos ? "abertos" : "fechados");
    DrawText(portoes, x, y2, FONTE_HUD_2, COR_TEXTO_FRACO);
    x += MeasureText(portoes, FONTE_HUD_2) + 40;

    const char *dica = "TAB: menu   ESPACO: coletar / entregar";
    DrawText(dica, x, y2, FONTE_HUD_2, COR_TEXTO_FRACO);
    x += MeasureText(dica, FONTE_HUD_2) + 40;

    /* aviso curto (carga cheia, tipo incompativel, etc.) */
    if (jogo->tempoMensagem > 0.0f && jogo->mensagem[0] != '\0')
        DrawText(jogo->mensagem, x, y2, FONTE_HUD_2, ORANGE);
}

void desenharJogo(const Jogo *jogo)
{
    if (jogo->grade == NULL)
        return;   /* nenhuma fase carregada */

    desenharTerreno(jogo);
    desenharEntidades(jogo);
    desenharHud(jogo);
}
