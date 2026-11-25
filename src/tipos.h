#ifndef TIPOS_H
#define TIPOS_H

/* tipos.h - constantes, enums e structs compartilhados por todos os modulos.
 * Nao ha variaveis globais: todo o estado da partida fica na struct Jogo,
 * que e passada por ponteiro para as funcoes. */

/* ===== CONSTANTES ===== */

/* Grade e janela (secao 2.5) */
#define LINHAS          20
#define COLUNAS         40
#define TAM_BLOCO       40                              /* pixels por celula */
#define ALTURA_HUD      80                              /* duas linhas de 40 px */
#define LARGURA_JANELA  (COLUNAS * TAM_BLOCO)           /* 1600 */
#define ALTURA_JANELA   (LINHAS * TAM_BLOCO + ALTURA_HUD) /* 880 */

/* Regras da partida */
#define MAX_PILHA       3       /* carrinho leva no maximo 3 kits */
#define VIDAS_INICIAIS  3
#define PONTOS_ENTREGA  100
#define PONTOS_COLISAO  50

/* Tempos (em segundos) */
#define TEMPO_PORTAO_ABERTO   4.0f
#define TEMPO_PORTAO_FECHADO  4.0f
#define INTERVALO_ZUMBI       0.5f   /* 2 blocos/s */
#define ESPERA_INICIAL_ZUMBI  0.5f   /* primeiro passo apos 0,5 s */
#define DURACAO_MENSAGEM      2.5f   /* quanto tempo um aviso fica no HUD */

/* Caracteres do terreno na grade (Tabela 1).
 * Kits (a/b/c), P e Z NAO ficam na grade: viram entidades separadas,
 * e a celula onde estavam passa a ser PISO. */
#define CEL_PISO    ' '
#define CEL_PAREDE  '#'
#define CEL_PORTAO  'D'
#define CEL_SAIDA   'S'
/* abrigos usam as proprias letras 'A', 'B' e 'C' */

#define TAM_MENSAGEM  64
#define ARQUIVO_SAVE  "save.bin"

/* ===== ENUMS ===== */

typedef enum {
    DIR_CIMA,
    DIR_BAIXO,
    DIR_ESQUERDA,
    DIR_DIREITA,
    DIR_NENHUMA
} Direcao;

typedef enum {
    KIT_NO_CHAO,
    KIT_CARREGADO,
    KIT_ENTREGUE
} EstadoKit;

/* Resultado de uma atualizacao da partida (retornado por atualizarJogo) */
typedef enum {
    RES_CONTINUA,          /* nada especial aconteceu */
    RES_FASE_CONCLUIDA,    /* entrou na saida liberada com tempo > 0 */
    RES_DERROTA_ZUMBI,     /* vidas zeraram por colisao */
    RES_DERROTA_TEMPO      /* vidas zeraram por falta de tempo */
} Resultado;

/* ===== STRUCTS ===== */

/* Posicao na grade (obrigatoria para entregador e zumbis) */
typedef struct {
    int linha;
    int coluna;
} Pos;

typedef struct {
    int id;              /* indice do kit no vetor jogo->kits */
    char tipo;           /* 'A', 'B' ou 'C' (mesma letra do abrigo que aceita) */
    Pos original;        /* onde estava no mapa; usado para devolver apos colisao */
    EstadoKit estado;
} Kit;

typedef struct {
    Pos pos;             /* posicao atual */
    Pos inicial;         /* posicao lida do mapa (Z) */
    Pos anterior;        /* posicao antes do ultimo passo (deteccao de troca) */
    Direcao direcao;
    float espera;        /* segundos ate o proximo passo */
} Zumbi;

typedef struct {
    Pos pos;
    Pos inicial;         /* posicao lida do mapa (P) */
    Pos anterior;
    float espera;        /* segundos ate poder dar o proximo passo */
    int pilha[MAX_PILHA];/* ids dos kits; pilha[0] e a base */
    int topo;            /* quantidade de kits na pilha (0 a 3) */
} Entregador;

/* Tudo o que representa a partida */
typedef struct {
    /* terreno: matriz dinamica LINHAS x COLUNAS (grade[linha][coluna]) */
    char **grade;

    /* entidades (vetores alocados conforme a contagem do mapa) */
    Kit *kits;
    int numKits;
    Zumbi *zumbis;
    int numZumbis;
    Entregador entregador;

    /* progresso */
    int fase;            /* numero N do mapaN.txt atual */
    int vidas;
    int pontos;
    int pontosEntrada;   /* pontuacao ao entrar na fase (restaurada se o tempo acabar) */

    /* tempo */
    float tempoLimite;   /* lido da primeira linha do mapa */
    float tempoRestante;

    /* portoes: todos compartilham o mesmo ciclo de 8 s */
    float tempoPortao;   /* tempo decorrido dentro do ciclo atual */
    int portoesAbertos;  /* 1 = abertos, 0 = fechados */

    int saidaLiberada;   /* 1 quando nao ha mais kits pendentes */

    /* aviso curto exibido no HUD (ex.: "Carga cheia") */
    char mensagem[TAM_MENSAGEM];
    float tempoMensagem; /* segundos que o aviso ainda fica visivel */
} Jogo;

#endif
