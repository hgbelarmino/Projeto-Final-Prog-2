#ifndef SAVE_H
#define SAVE_H

#include "tipos.h"

/* Funcoes planejadas:
 *
 * salvarJogo(jogo, nomeArquivo)
 *   - grava em binario (assinatura + versao no inicio)
 *   - verifica o retorno de cada fwrite
 *
 * carregarJogo(jogo, nomeArquivo)
 *   - le e valida assinatura/versao e quantidades antes de alocar
 *   - reconstroi grade, kits, zumbis e pilha (sem gravar ponteiros)
 *   - em falha: manter o estado atual em memoria
 */

/* Prototipos usados pelo main.c / menu.c (Pessoa A).
 * Se precisarem mudar a assinatura, avisem para ajustar as chamadas. */
/* retornam 1 em caso de sucesso e 0 em caso de falha */
int salvarJogo(const Jogo *jogo, const char *nomeArquivo);
int carregarJogo(Jogo *jogo, const char *nomeArquivo);

#endif
