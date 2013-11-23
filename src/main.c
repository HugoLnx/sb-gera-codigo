#include <stdio.h>
#include <stdlib.h>
#include "gera.h"

#define PARAM_NULO 0

void deveRetornar(char *pathPrograma, int retornoEsperado, int param, int param2);

int main()
{
	deveRetornar("../samples/ret_constantes_condicional_true.sb"       , 50, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/ret_condicional_false.sb"                 , 99, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_variavel_constante.sb"         , 80, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/ret_variavel.sb"                          , 30, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_sub_constantes.sb"             , 28, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_multiplicacao_constantes.sb"   , 15, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_soma_variaveis.sb"             , 3 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_subtracao_variaveis.sb"        , 3 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/atribuicao_multiplicacao_variaveis.sb"    , 8 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/varias_operacoes_aritmeticas.sb"          , 1 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/ret_parametro.sb"                         , 5 , 5         , PARAM_NULO);
	deveRetornar("../samples/ret_parametro_como_check.sb"              , 1 , 10        , PARAM_NULO);
	deveRetornar("../samples/x_mais_um.sb"                             , 2 , 1         , PARAM_NULO);
	deveRetornar("../samples/call_teste.sb"                            , 5 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/call_com_parametro.sb"                    , 10, PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/fatorial.sb"                              , 6 , 3         , PARAM_NULO);
	deveRetornar("../samples/dois_params.sb"                           , 6 , 2         , 4         );
	deveRetornar("../samples/dez_funcoes_cem_linhas.sb"                , 1 , PARAM_NULO, PARAM_NULO);
	deveRetornar("../samples/duas_funcoes_cem_linhas.sb"               , 1 , PARAM_NULO, PARAM_NULO);

  return 0;
}

void deveRetornar(char *pathPrograma, int retornoEsperado, int param, int param2)
{
	unsigned char *code;
	int retorno;
	funcp func;
  FILE *myfp;

  if ((myfp = fopen (pathPrograma, "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

	gera(myfp, (void**) &code, &func);

	retorno = func(param, param2);
	if (retorno != retornoEsperado)
	{
		printf("%s retorno: %d; esperado:%d \n", pathPrograma, retorno, retornoEsperado);
	}

	fclose(myfp);

	libera(code);
}
