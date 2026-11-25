/* menu.c - menu de pausa, menu inicial e telas de vitoria/derrota
 *
 * Tabela 4 do enunciado:
 *   N - Novo Jogo : comeca no mapa1.txt com 3 vidas, 0 pontos e carga vazia
 *   C - Carregar  : restaura o save.bin e continua no menu ate V
 *   S - Salvar    : grava o estado atual e continua no menu
 *   Q - Sair      : encerra sem salvar
 *   V - Voltar    : retoma a partida pausada
 */

#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "menu.h"
#include "mapa.h"
#include "zumbi.h"
#include "save.h"

#define COR_FUNDO_MENU   (Color){ 15, 20, 28, 230 }
#define COR_DESTAQUE     (Color){ 250, 210, 60, 255 }
#define COR_INDISPONIVEL (Color){ 90, 95, 105, 255 }

void iniciarMenu(Menu *menu)
{
    menu->aberto = 1;
    menu->partidaAtiva = 0;
    menu->sair = 0;
    menu->acabouDeFechar = 0;
    menu->fim = FIM_NENHUM;
    menu->pontuacaoFinal = 0;
    menu->aviso[0] = '\0';
}

static void definirAviso(Menu *menu, const char *texto)
{
    snprintf(menu->aviso, sizeof(menu->aviso), "%s", texto);
}

/* Fecha o menu e joga fora as teclas que ainda estao na fila do raylib,
 * para que a tecla usada no menu nao vire movimento/interacao no jogo. */
static void fecharMenu(Menu *menu)
{
    menu->aberto = 0;
    menu->acabouDeFechar = 1;
    menu->aviso[0] = '\0';
    while (GetKeyPressed() != 0) {
        /* esvazia a fila de teclas */
    }
}

/* N - Novo Jogo. Se o mapa1.txt nao puder ser lido, a partida atual
 * (se existir) continua intacta, pois carregarMapa nao altera o jogo em erro. */
static void novoJogo(Menu *menu, Jogo *jogo)
{
    ResultadoMapa r = carregarMapa(jogo, 1);

    if (r == MAPA_INEXISTENTE) {
        definirAviso(menu, "Erro: arquivo mapa1.txt nao encontrado.");
        fprintf(stderr, "Erro: arquivo mapa1.txt nao encontrado.\n");
        return;
    }
    if (r == MAPA_ERRO_LEITURA) {
        definirAviso(menu, "Erro: mapa1.txt esta incompleto ou invalido.");
        return;
    }
    if (r == MAPA_ERRO_MEMORIA) {
        definirAviso(menu, "Erro: memoria insuficiente para carregar o mapa.");
        return;
    }

    jogo->vidas = VIDAS_INICIAIS;
    jogo->pontos = 0;
    jogo->pontosEntrada = 0;
    reposicionarZumbis(jogo);   /* sorteia a direcao inicial dos zumbis */

    menu->partidaAtiva = 1;
    menu->fim = FIM_NENHUM;
    fecharMenu(menu);
}

/* C - Carregar. Em falha, carregarJogo mantem o estado atual em memoria. */
static void carregar(Menu *menu, Jogo *jogo)
{
    if (carregarJogo(jogo, ARQUIVO_SAVE)) {
        menu->partidaAtiva = 1;
        menu->fim = FIM_NENHUM;
        definirAviso(menu, "Jogo carregado. Pressione V para continuar.");
    } else {
        definirAviso(menu, "Nao foi possivel carregar o save.bin.");
    }
}

/* S - Salvar */
static void salvar(Menu *menu, const Jogo *jogo)
{
    if (salvarJogo(jogo, ARQUIVO_SAVE))
        definirAviso(menu, "Jogo salvo em save.bin.");
    else
        definirAviso(menu, "Erro ao salvar: o arquivo pode estar incompleto.");
}

void processarMenu(Menu *menu, Jogo *jogo)
{
    /* menu fechado: so o TAB interessa aqui */
    if (!menu->aberto) {
        if (menu->partidaAtiva && IsKeyPressed(KEY_TAB)) {
            menu->aberto = 1;
            menu->aviso[0] = '\0';
        }
        return;
    }

    /* menu aberto: as teclas abaixo so valem aqui dentro */
    if (IsKeyPressed(KEY_Q)) {
        menu->sair = 1;
    } else if (IsKeyPressed(KEY_N)) {
        novoJogo(menu, jogo);
    } else if (IsKeyPressed(KEY_C)) {
        carregar(menu, jogo);
    } else if (IsKeyPressed(KEY_S)) {
        if (menu->partidaAtiva)
            salvar(menu, jogo);
        else
            definirAviso(menu, "Salvar indisponivel: nenhuma partida ativa.");
    } else if (IsKeyPressed(KEY_V)) {
        if (menu->partidaAtiva)
            fecharMenu(menu);
        else
            definirAviso(menu, "Voltar indisponivel: nenhuma partida ativa.");
    }
}

void encerrarPartida(Menu *menu, Jogo *jogo, FimDePartida fim)
{
    menu->fim = fim;
    menu->pontuacaoFinal = jogo->pontos;
    menu->partidaAtiva = 0;
    menu->aberto = 1;
    menu->aviso[0] = '\0';
    liberarFase(jogo);
}

/* ----- desenho ----- */

static void textoCentralizadoX(const char *texto, int y, int fonte, Color cor)
{
    int x = (LARGURA_JANELA - MeasureText(texto, fonte)) / 2;
    DrawText(texto, x, y, fonte, cor);
}

static void desenharTelaVitoria(const Menu *menu, int y)
{
    char texto[64];
    textoCentralizadoX("VITORIA! Todos os suprimentos foram entregues.", y, 36, GREEN);
    snprintf(texto, sizeof(texto), "Pontuacao final: %06d", menu->pontuacaoFinal);
    textoCentralizadoX(texto, y + 50, 28, RAYWHITE);
}

static void desenharTelaDerrota(const Menu *menu, int y)
{
    char texto[64];
    const char *motivo = (menu->fim == FIM_DERROTA_ZUMBI)
                         ? "Motivo: atacado por um zumbi sem vidas restantes."
                         : "Motivo: o tempo acabou sem vidas restantes.";
    textoCentralizadoX("FIM DE JOGO", y, 36, RED);
    textoCentralizadoX(motivo, y + 45, 24, RAYWHITE);
    snprintf(texto, sizeof(texto), "Pontuacao final: %06d", menu->pontuacaoFinal);
    textoCentralizadoX(texto, y + 80, 28, RAYWHITE);
}

/* Desenha uma opcao do menu; se indisponivel, fica cinza. */
static void desenharOpcao(const char *tecla, const char *nome, int disponivel,
                          int x, int y)
{
    Color corTecla = disponivel ? COR_DESTAQUE : COR_INDISPONIVEL;
    Color corNome = disponivel ? RAYWHITE : COR_INDISPONIVEL;

    DrawText(tecla, x, y, 30, corTecla);
    DrawText(nome, x + 50, y, 30, corNome);
    if (!disponivel)
        DrawText("(indisponivel)", x + 50 + MeasureText(nome, 30) + 15, y + 6, 20,
                 COR_INDISPONIVEL);
}

void desenharMenu(const Menu *menu, const Jogo *jogo)
{
    (void)jogo;   /* o jogo pausado ja foi desenhado por baixo no main.c */

    /* fundo semitransparente: o jogo pausado continua visivel atras */
    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, COR_FUNDO_MENU);

    int y = 110;
    textoCentralizadoX("RESIDENT EVIL", y, 64, RED);
    textoCentralizadoX("Entregador de Suprimentos", y + 70, 28, COR_INDISPONIVEL);
    y += 150;

    if (menu->fim == FIM_VITORIA) {
        desenharTelaVitoria(menu, y);
        y += 130;
    } else if (menu->fim == FIM_DERROTA_ZUMBI || menu->fim == FIM_DERROTA_TEMPO) {
        desenharTelaDerrota(menu, y);
        y += 130;
    } else if (menu->partidaAtiva) {
        textoCentralizadoX("PAUSADO", y, 36, COR_DESTAQUE);
        y += 70;
    }

    int x = LARGURA_JANELA / 2 - 160;
    int ativa = menu->partidaAtiva;
    desenharOpcao("N", "Novo Jogo", 1, x, y);       y += 50;
    desenharOpcao("C", "Carregar", 1, x, y);        y += 50;
    desenharOpcao("S", "Salvar", ativa, x, y);      y += 50;
    desenharOpcao("V", "Voltar", ativa, x, y);      y += 50;
    desenharOpcao("Q", "Sair", 1, x, y);            y += 70;

    if (menu->aviso[0] != '\0')
        textoCentralizadoX(menu->aviso, y, 24, ORANGE);
}
