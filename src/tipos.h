#ifndef TIPOS_H
#define TIPOS_H

/* ===== CONSTANTES (macros) =====
 * - dimensoes da grade: linhas, colunas, tamanho do bloco, altura do HUD
 * - tamanho da janela (largura e altura)
 * - capacidade maxima da pilha (3)
 * - vidas iniciais (3)
 * - tempos dos portoes (4s abertos / 4s fechados)
 * - intervalo do passo dos zumbis (0,5 s) e espera inicial (0,5 s)
 */


/* ===== STRUCTS / ENUMS =====
 * - Pos: linha e coluna (usada por entregador e zumbis)
 * - Direcao: enum cima, baixo, esquerda, direita, nenhuma
 * - EstadoKit: enum no chao, carregado, entregue
 * - Kit: id, tipo, posicao original, estado
 * - Zumbi: posicao atual, inicial, anterior, direcao, tempo ate o proximo passo
 * - Entregador: posicao atual, inicial, anterior, tempo ate o proximo passo,
 *               pilha (vetor de 3 ids de kits) e contador do topo
 * - Jogo: tudo que representa a partida (sem variaveis globais)
 *     * grade (matriz dinamica) do terreno
 *     * vetor de kits + quantidade
 *     * vetor de zumbis + quantidade
 *     * entregador
 *     * fase, vidas, pontos, pontos na entrada da fase
 *     * tempo limite, tempo restante
 *     * estado do ciclo dos portoes
 *     * se a saida esta liberada
 *     * mensagem curta para o HUD
 */

#endif
