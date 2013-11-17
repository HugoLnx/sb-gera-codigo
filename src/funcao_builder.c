#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
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

	 FABUI_SubDoESP(pFuncao->pAssembly, 40);

   return pFuncao;
}

void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	FABUI_DestruirBuilder(pFuncao->pAssembly);
	free(pFuncao);
}

void FBUI_RetornarComConstantes(FBUI_tppFuncao pFuncaoParm, int condicao, int retorno)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
  
	if (condicao == 0)
	{
		FABUI_MovToEAX(pFuncao->pAssembly, retorno);
		FABUI_JmpParaRodape(pFuncao->pAssembly);
	}
}

void FBUI_RetornarComConstanteEVariavel(FBUI_tppFuncao pFuncaoParm, int condicao, int numDaVariavelRetornada)
{

	if (condicao == 0)
	{
		char stackPosition = (numDaVariavelRetornada + 1) * -4;
		FABUI_MovDaStackParaEAX(pFuncaoParm->pAssembly, stackPosition);
		FABUI_JmpParaRodape(pFuncaoParm->pAssembly);
	}
}

void FBUI_AtribuirSoma(FBUI_tppFuncao pFuncaoParm, int nVariavel, int a, int b)
{
	char stackPosition = (nVariavel + 1) * -4;
	FABUI_MovToECX(pFuncaoParm->pAssembly, a);
	FABUI_AddToECX(pFuncaoParm->pAssembly, b);
	FABUI_MovECXToStack(pFuncaoParm->pAssembly, stackPosition);
}

unsigned char* FBUI_Instrucoes(FBUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
   return FABUI_Instrucoes(pFuncao->pAssembly);
}
