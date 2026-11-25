#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"

/* Funcoes planejadas (regras do entregador e da partida):
 *
 * atualizarEntregador(jogo, dt)
 *   - leitura das setas/WASD com prioridade fixa
 *   - intervalo entre passos conforme a carga
 *   - bloqueio por parede e portao fechado
 *
 * coletarOuEntregar(jogo)
 *   - espaco: coleta kit na celula atual ou entrega o do topo no abrigo
 *   - mensagens: carga cheia, carga vazia, tipo incompativel
 *
 * detectarContato(jogo)
 *   - mesma celula, troca de posicao, entrada em celula ocupada
 *
 * perderVida / reiniciarAposColisao(jogo)
 * reiniciarFasePorTempo(jogo)
 * atualizarCronometro(jogo, dt)
 * verificarEvacuacao(jogo)
 *
 * atualizarJogo(jogo, dt)
 *   - ordem de prioridade: tempo, contato com zumbi, kit/abrigo, evacuacao
 */

/* Prototipos usados pelo main.c / menu.c (Pessoa A).
 * Se precisarem mudar a assinatura, avisem para ajustar as chamadas. */
Resultado atualizarJogo(Jogo *jogo, float dt);

#endif
