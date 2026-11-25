#ifndef MENU_H
#define MENU_H

#include "tipos.h"

/* Como a ultima partida terminou (para as telas de vitoria/derrota) */
typedef enum {
    FIM_NENHUM,
    FIM_VITORIA,
    FIM_DERROTA_ZUMBI,
    FIM_DERROTA_TEMPO
} FimDePartida;

/* Estado do menu. Fica separado do Jogo porque nao faz parte da partida
 * (nao vai para o save.bin). Tambem e passado por parametro: sem globais. */
typedef struct {
    int aberto;          /* 1 = menu na tela e simulacao pausada */
    int partidaAtiva;    /* 1 = existe uma fase carregada em memoria */
    int sair;            /* 1 = jogador apertou Q */
    int acabouDeFechar;  /* 1 = pular a atualizacao no quadro em que o menu fechou */
    FimDePartida fim;    /* resultado da ultima partida (FIM_NENHUM se nao houve) */
    int pontuacaoFinal;
    char aviso[2 * TAM_MENSAGEM]; /* mensagem de erro/sucesso dentro do menu */
} Menu;

/* Estado inicial: menu aberto, sem partida (opcoes Novo Jogo, Carregar, Sair). */
void iniciarMenu(Menu *menu);

/* Chamada a cada quadro.
 *  - menu fechado: TAB abre o menu (so com partida ativa) e pausa tudo
 *  - menu aberto: N novo jogo, C carregar, S salvar, Q sair, V voltar
 *    (S e V ficam indisponiveis sem partida ativa)
 *  - ao fechar, descarta as teclas pendentes para nao gerar movimento */
void processarMenu(Menu *menu, Jogo *jogo);

/* Encerra a partida (vitoria ou derrota): guarda a pontuacao final,
 * libera a fase e volta ao menu inicial mostrando o resultado. */
void encerrarPartida(Menu *menu, Jogo *jogo, FimDePartida fim);

/* Desenha o menu (por cima do jogo pausado, se houver partida). */
void desenharMenu(const Menu *menu, const Jogo *jogo);

#endif
