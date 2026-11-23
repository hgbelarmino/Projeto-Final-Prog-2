#ifndef TIPOS_H
#define TIPOS_H

/* ===== Constantes (macros permitidas pela especificacao) ===== */
#define LINHAS        20
#define COLUNAS       40
#define TAM_BLOCO     40
#define ALTURA_HUD    80
#define LARGURA_TELA  (COLUNAS * TAM_BLOCO)               /* 1600 */
#define ALTURA_TELA   (LINHAS * TAM_BLOCO + ALTURA_HUD)   /* 880  */

#define MAX_PILHA     3
#define VIDAS_INICIAIS 3

#define PORTAO_ABERTO_S  4.0f
#define PORTAO_FECHADO_S 4.0f
#define ZUMBI_INTERVALO_S 0.5f   /* 2 blocos/s */
#define ZUMBI_ESPERA_INICIAL_S 0.5f

/* ===== Posicao (obrigatoria para entregador e zumbis) ===== */
typedef struct {
    int linha;
    int coluna;
} Pos;

typedef enum { DIR_CIMA, DIR_BAIXO, DIR_ESQ, DIR_DIR, DIR_NENHUMA } Direcao;

/* ===== Kits ===== */
typedef enum { KIT_CHAO, KIT_CARREGADO, KIT_ENTREGUE } EstadoKit;

typedef struct {
    int id;            /* identidade propria */
    char tipo;         /* 'a', 'b' ou 'c' */
    Pos original;      /* posicao de origem no mapa */
    EstadoKit estado;
} Kit;

/* ===== Zumbis ===== */
typedef struct {
    Pos atual;
    Pos inicial;
    Pos anterior;
    Direcao dir;
    float espera;      /* tempo restante ate o proximo passo */
} Zumbi;

/* ===== Entregador ===== */
typedef struct {
    Pos atual;
    Pos inicial;
    Pos anterior;
    float espera;      /* tempo restante ate poder andar de novo */
    int pilha[MAX_PILHA]; /* ids dos kits, base = indice 0 */
    int topo;          /* quantidade de kits carregados */
} Entregador;

/* ===== Estado completo da partida (passado por parametro, sem globais) ===== */
typedef struct {
    char **grade;          /* terreno: '#', ' ', 'A','B','C','D','S' (sem kits/entidades) */
    Kit *kits;
    int nKits;
    Zumbi *zumbis;
    int nZumbis;
    Entregador jogador;

    int fase;
    int vidas;
    int pontos;
    int pontosEntrada;     /* pontuacao ao entrar na fase */
    float limiteTempo;     /* lido do mapa */
    float tempoRestante;
    float tempoPortoes;    /* posicao no ciclo de 8s */
    int portoesAbertos;    /* 1 = abertos */
    int saidaLiberada;

    char mensagem[64];     /* aviso na HUD (carga cheia, tipo errado...) */
} Jogo;

#endif
