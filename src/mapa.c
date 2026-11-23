/* mapa.c - leitura dos arquivos de mapa e alocacao dinamica
 *
 * Aqui vai:
 *  - liberarFase
 *  - funcao auxiliar para alocar a grade (vetor de ponteiros + cada linha)
 *  - carregarMapa:
 *      1. montar o nome "mapaN.txt" e abrir com fopen (tratar falha)
 *      2. ler a primeira linha (tempo limite)
 *      3. ler as 20 linhas de 40 caracteres (tratar \r\n e linhas curtas)
 *      4. contar kits e zumbis
 *      5. alocar os vetores de kits e zumbis
 *      6. percorrer a matriz: preencher kits, zumbis e posicao do P,
 *         deixando a grade apenas com o terreno
 *      7. inicializar tempo, portoes, saida e pilha
 *      8. em qualquer erro: liberar tudo e retornar codigo de erro
 */

#include "mapa.h"
