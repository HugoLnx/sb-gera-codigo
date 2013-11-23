#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "funcao_assembly_builder.h"
#include "funcao_builder.h"

#define CONSTANTE '$'
#define VARIAVEL 'v'
#define PARAMETRO 'p'

#define SOMA_OP '+'
#define SUB_OP '-'
#define MUL_OP '*'

typedef struct FBUI_stFuncao
{
	FABUI_tppFuncao pAssembly;
} tpFuncao;


/* prototipos das funcoes encapsuladas */
char PosicaoNaStackDoParametro(int numeroDoParametro);
char PosicaoNaStackDaVariavel(int numeroDaVariavel);


/***** funções exportadas *****/

FBUI_tppFuncao FBUI_CriarBuilder(void **ppInstrucoes)
{
   tpFuncao *pFuncao = (tpFuncao*) malloc(sizeof(tpFuncao));

	 pFuncao->pAssembly = FABUI_CriarBuilder(ppInstrucoes);

	 FABUI_SubDoESP(pFuncao->pAssembly, 40);

   return pFuncao;
}

void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	FABUI_DestruirBuilder(pFuncao->pAssembly);
	free(pFuncao);
}

void FBUI_Retornar(FBUI_tppFuncao pFuncaoParm, char tipoCheck, int numCheck, char tipoRetorno, int numRetorno)
{
	if (tipoCheck == CONSTANTE)
	{
		FABUI_MovToECX(pFuncaoParm->pAssembly, numCheck);
	} else {
		char posicao;
		if (tipoCheck == VARIAVEL)
		{
			posicao = PosicaoNaStackDaVariavel(numCheck);
		} else if (tipoCheck == PARAMETRO) {
			posicao = PosicaoNaStackDoParametro(numCheck);
		}
		FABUI_MovDaStackParaECX(pFuncaoParm->pAssembly, posicao);
	}

	if (tipoRetorno == CONSTANTE)
	{
		FABUI_MovToEAX(pFuncaoParm->pAssembly, numRetorno);
	} else {
		char posicao;
		if (tipoRetorno == VARIAVEL)
		{
			posicao = PosicaoNaStackDaVariavel(numRetorno);
		} else if (tipoRetorno == PARAMETRO) {
			posicao = PosicaoNaStackDoParametro(numRetorno);
		}
		FABUI_MovDaStackParaEAX(pFuncaoParm->pAssembly, posicao);
	}

	FABUI_CmpECX(pFuncaoParm->pAssembly, 0);
	FABUI_JmpParaRodapeSeIgual(pFuncaoParm->pAssembly);
}

void FBUI_Invocar(FBUI_tppFuncao pFuncaoParm, char tipoReceptor, int numReceptor, void *endereco, char tipoParametro, int numParametro)
{
	if (tipoParametro == CONSTANTE)
	{
		FABUI_MovToECX(pFuncaoParm->pAssembly, numParametro);
	} else {
		char posicao;
		if (tipoParametro == VARIAVEL)
		{
			posicao = PosicaoNaStackDaVariavel(numParametro);
		} else if (tipoParametro == PARAMETRO) {
			posicao = PosicaoNaStackDoParametro(numParametro);
		}
		FABUI_MovDaStackParaECX(pFuncaoParm->pAssembly, posicao);
	}

	FABUI_PushECX(pFuncaoParm->pAssembly);

	FABUI_Call(pFuncaoParm->pAssembly, endereco);

	FABUI_AddESP(pFuncaoParm->pAssembly, sizeof(int));

	char posicaoReceptor;
	if (tipoReceptor == VARIAVEL)
	{
		posicaoReceptor = PosicaoNaStackDaVariavel(numReceptor);
	} else {
		posicaoReceptor = PosicaoNaStackDoParametro(numReceptor);
	}
	FABUI_MovEAXToStack(pFuncaoParm->pAssembly, posicaoReceptor);
}

void FBUI_AtribuirSoma(FBUI_tppFuncao pFuncaoParm, char tipoReceptor, int nVariavel, char tipoA, int a, char op, char tipoB, int b)
{
	char posicaoReceptor;
	if (tipoA == CONSTANTE)
	{
		FABUI_MovToECX(pFuncaoParm->pAssembly, a);
	} else {
		char posicao;
		if (tipoA == VARIAVEL)
		{
			posicao = PosicaoNaStackDaVariavel(a);
		} else if (tipoA == PARAMETRO) {
			posicao = PosicaoNaStackDoParametro(a);
		}
		FABUI_MovDaStackParaECX(pFuncaoParm->pAssembly, posicao);
	}

	if (tipoB == CONSTANTE)
	{
		FABUI_MovToEDX(pFuncaoParm->pAssembly, b);
	} else
	{
		char posicao;
		if (tipoB == VARIAVEL)
		{
			posicao = PosicaoNaStackDaVariavel(b);
		} else if (tipoB == PARAMETRO) {
			posicao = PosicaoNaStackDoParametro(b);
		}
		FABUI_MovDaStackParaEDX(pFuncaoParm->pAssembly, posicao);
	}

	switch(op)
	{
		case SOMA_OP: FABUI_AddEDX_ECX(pFuncaoParm->pAssembly);break;
		case SUB_OP: FABUI_SubEDX_ECX(pFuncaoParm->pAssembly);break;
		case MUL_OP: FABUI_MulEDX_ECX(pFuncaoParm->pAssembly);break;
	}

	if (tipoReceptor == VARIAVEL)
	{
		posicaoReceptor = PosicaoNaStackDaVariavel(nVariavel);
	} else {
		posicaoReceptor = PosicaoNaStackDoParametro(nVariavel);
	}
	FABUI_MovECXToStack(pFuncaoParm->pAssembly, posicaoReceptor);
}

unsigned char* FBUI_FinalizarInstrucoes(FBUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
   return FABUI_FinalizarInstrucoes(pFuncao->pAssembly);
}


/**** funções encapsuladas ***/

char PosicaoNaStackDaVariavel(int numeroDaVariavel)
{
	return (numeroDaVariavel + 1) * -4;
}

char PosicaoNaStackDoParametro(int numeroDoParametro)
{
	return 8 + numeroDoParametro * 4;
}
