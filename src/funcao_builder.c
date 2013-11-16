#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "lista.h"
#include "funcao_assembly_builder.h"
#include "funcao_builder.h"

typedef struct FBUI_stFuncao
{
	FABUI_tppFuncao pAssembly;
} tpFuncao;

FBUI_tppFuncao FBUI_CriarBuilder()
{
   tpFuncao *pFuncao = (tpFuncao*) malloc(sizeof(tpFuncao));

	 pFuncao->pAssembly = FABUI_CriarBuilder();

   return pFuncao;
}

void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	FABUI_DestruirBuilder(pFuncao->pAssembly);
	free(pFuncao);
}

void FBUI_Retornar(FBUI_tppFuncao pFuncaoParm, int condicao, int retorno)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
  
	if (condicao)
	{

	}
}

unsigned char* FBUI_Instrucoes(FBUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
   return FABUI_Instrucoes(pFuncao->pAssembly);
}
