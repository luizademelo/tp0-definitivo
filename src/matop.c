//---------------------------------------------------------------------
// Arquivo      : matop.c
// Conteudo     : programa de avaliacao do TAD MAT
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//              : 2021-10-21 - estrutura de diretorios
//              : 2021-10-25 - opcoes de linha de comando
//              : 2021-11-14 - adequacao para versao 1.1 memlog
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "mat.h"
#include "memlog.h"
#include "msgassert.h"

// definicoes de operacoes a serem testadas
#define OPSOMAR 1
#define OPMULTIPLICAR 2
#define OPTRANSPOR 3

// variaveis globais para opcoes
static int opescolhida;
char *lognome;
char *nomeMatriz1;
char *nomeMatriz2;
char *outputName;
int regmem;

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr, "matop\n");
  fprintf(stderr, "\t-s \t\t(soma matrizes) \n");
  fprintf(stderr, "\t-m \t\t(multiplica matrizes) \n");
  fprintf(stderr, "\t-t \t\t(transpoe matriz 1)\n");
  fprintf(stderr, "\t-1 <arq>\t(matriz 1)\n");
  fprintf(stderr, "\t-2 <arq>\t(matriz 2)\n");
  fprintf(stderr, "\t-o <arq>\t(matriz resultante)\n");
  fprintf(stderr, "\t-p <arq>\t(registro de desempenho)\n");
  fprintf(stderr, "\t-l \t\t(padrao de acesso e localidade)\n");
}

void parse_args(int argc, char **argv)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc e argv
// Saida: optescolhida, optx, opty, regmem, lognome
{
  // variaveis externas do getopt
  extern char *optarg;

  // variavel auxiliar
  int c;

  // inicializacao variaveis globais para opcoes
  opescolhida = -1;
  regmem = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "smtp:1:2:lo:h")) != EOF)
    switch (c)
    {
    case 'm':
      avisoAssert(opescolhida == -1, "Mais de uma operacao escolhida");
      opescolhida = OPMULTIPLICAR;
      break;
    case 's':
      avisoAssert(opescolhida == -1, "Mais de uma operacao escolhida");
      opescolhida = OPSOMAR;
      break;
    case 't':
      avisoAssert(opescolhida == -1, "Mais de uma operacao escolhida");
      opescolhida = OPTRANSPOR;
      break;
    case 'p':
      lognome = optarg;
      break;
    case 'o':
      outputName = optarg;
      break;
    case '1':
      nomeMatriz1 = optarg;
      break;
    case '2':
      nomeMatriz2 = optarg;
      break;
    case 'l':
      regmem = 1;
      break;
    case 'h':
    default:
      uso();
      exit(1);
    }
}

int main(int argc, char **argv)
// Descricao: programa principal para execucao de operacoes de matrizes
// Entrada: argc e argv
// Saida: depende da operacao escolhida
{
  // ate 3 matrizes sao utilizadas, dependendo da operacao
  mat_tipo a, b, c;

  // avaliar linha de comando
  parse_args(argc, argv);

  // iniciar registro de acesso
  iniciaMemLog(lognome);

  // execucao dependente da operacao escolhida
  switch (opescolhida)
  {
  case OPSOMAR:
    // cria matrizes a e b aleatorias, que sao somadas para a matriz c
    // matriz c é impressa e todas as matrizes sao destruidas
    leMatriz(nomeMatriz1, &a);
    leMatriz(nomeMatriz2, &b);
    defineFaseMemLog(1);
    acessaMatriz(&a);
    acessaMatriz(&b);
    somaMatrizes(&a, &b, &c);
    acessaMatriz(&c);
    escreveSaida(&c, outputName);
    defineFaseMemLog(2);
    acessaMatriz(&c);
    if (regmem)
      imprimeMatriz(&c);
    destroiMatriz(&a);
    destroiMatriz(&b);
    destroiMatriz(&c);
    break;
  case OPMULTIPLICAR:
    // cria matrizes a e b aleatorias, que sao multiplicadas para matriz c
    // matriz c é impressa e todas as matrizes sao destruidas
    defineFaseMemLog(0);
    leMatriz(nomeMatriz1, &a);
    leMatriz(nomeMatriz2, &b);
    multiplicaMatrizes(&a, &b, &c);
    defineFaseMemLog(1);
    acessaMatriz(&a);
    acessaMatriz(&b);
    acessaMatriz(&c);
    escreveSaida(&c, outputName);
    defineFaseMemLog(2);
    acessaMatriz(&c);
    if (regmem)
      imprimeMatriz(&c);
    destroiMatriz(&a);
    destroiMatriz(&b);
    destroiMatriz(&c);
    break;
  case OPTRANSPOR:
    // cria matriz a aleatoria, que e transposta, impressa e destruida
    defineFaseMemLog(0);
    leMatriz(nomeMatriz1, &a);
    defineFaseMemLog(1);
    acessaMatriz(&a);
    transpoeMatriz(&a);
    escreveSaida(&a, outputName);
    defineFaseMemLog(2);
    acessaMatriz(&a);
    if (regmem)
      imprimeMatriz(&a);
    destroiMatriz(&a);
    break;
  default:
    // nao deve ser executado, pois ha um erroAssert em parse_args
    uso();
    exit(1);
  }

  // conclui registro de acesso
  return finalizaMemLog();
}
