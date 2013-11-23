#include <stdio.h>
#include <stdlib.h>
#include "../src/parser.h"
#include "../src/tamanho_funcao.h"

typedef int (*tpFunc)();
typedef int (*tpFuncComParam)(int p);

void deveRetornar(char *pathPrograma, int retornoEsperado)
{
	unsigned char *code;
	int retorno;
	int tamanho;
	tpFunc func;
  FILE *myfp;

  if ((myfp = fopen (pathPrograma, "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

	code = PAR_ParseProgram(myfp, &tamanho);
	func = (tpFunc) code;

	retorno = func();
	if (retorno != retornoEsperado)
	{
		printf("%s retorno: %d; esperado:%d \n", pathPrograma, retorno, retornoEsperado);
	}

	fclose(myfp);

	free(code);
}

void segundaDeveRetornar(char *pathPrograma, int retornoEsperado)
{
	unsigned char *code;
	int retorno;
	int tamanho;
	tpFunc func;
  FILE *myfp;

  if ((myfp = fopen (pathPrograma, "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

	code = PAR_ParseProgram(myfp, &tamanho);
	func = (tpFunc) code + TAMANHO_INSTRUCOES;

	retorno = func();
	if (retorno != retornoEsperado)
	{
		printf("%s retorno: %d; esperado:%d \n", pathPrograma, retorno, retornoEsperado);
	}

	fclose(myfp);

	free(code);
}

void deveRetornarComParam(char *pathPrograma, int retornoEsperado, int param)
{
	unsigned char *code;
	int retorno;
	int tamanho;
	tpFunc func;
  FILE *myfp;

  if ((myfp = fopen (pathPrograma, "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

	code = PAR_ParseProgram(myfp, &tamanho);
	func = (tpFuncComParam) code;

	retorno = func(param);
	if (retorno != retornoEsperado)
	{
		printf("%s retorno: %d; esperado:%d \n", pathPrograma, retorno, retornoEsperado);
	}

	fclose(myfp);

	free(code);
}

int main()
{
	deveRetornar("ret_constantes_condicional_true.sb", 50);
	deveRetornar("ret_condicional_false.sb", 99);
	deveRetornar("atribuicao_variavel_constante.sb", 80);
	deveRetornar("ret_variavel.sb", 30);
	deveRetornar("atribuicao_sub_constantes.sb", 28);
	deveRetornar("atribuicao_multiplicacao_constantes.sb", 15);
	deveRetornar("atribuicao_soma_variaveis.sb", 3);
	deveRetornar("atribuicao_subtracao_variaveis.sb", 3);
	deveRetornar("atribuicao_multiplicacao_variaveis.sb", 8);
	deveRetornar("varias_operacoes_aritmeticas.sb", 1);
	deveRetornarComParam("ret_parametro.sb", 5, 5);
	deveRetornarComParam("ret_parametro_como_check.sb", 1, 10);
	deveRetornarComParam("x_mais_um.sb", 2, 1);
	segundaDeveRetornar("call_teste.sb", 5);
	segundaDeveRetornar("call_com_parametro.sb", 10);
	deveRetornarComParam("fatorial.sb", 6, 3);
  return 0;
}
