#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "lista.h"
#include "funcao_assembly_builder.h"

typedef struct FABUI_stFuncao
{
   LIS_tppLista pCabecalho;
   LIS_tppLista pCorpo;
   LIS_tppLista pRodape;
} tpFuncao;

#define TAMANHO_CABECALHO 3
#define TAMANHO_RODAPE 4

#define MOVL_TO_EAX 0xb8

static unsigned char CABECALHO[] = {
	0x55,       // pushl %ebp
	0x89, 0xe5  // movl %esp, %ebp
};


static unsigned char RODAPE[] = {
	0x89, 0xec,  // movl %ebp, %esp
	0x5d,        // popl %ebp
	0xc3         // ret
};


/* prototipos funcoes encapsuladas */

/* funcoes exportadas */

FABUI_tppFuncao FABUI_CriarBuilder()
{
   int i;
   tpFuncao *pFunc = (tpFuncao*) malloc(sizeof(tpFuncao));
   LIS_CriarLista(&pFunc->pCabecalho);
   LIS_CriarLista(&pFunc->pCorpo);
   LIS_CriarLista(&pFunc->pRodape);

   for (i = 0; i < TAMANHO_CABECALHO; i++)
   {
      LIS_InserirElementoApos(pFunc->pCabecalho, CABECALHO[i]);
   }

   for (i = 0; i < TAMANHO_RODAPE; i++)
   {
      LIS_InserirElementoApos(pFunc->pRodape, RODAPE[i]);
   }

   return pFunc;
}

void FABUI_DestruirBuilder(FABUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	LIS_DestruirLista(pFuncao->pCabecalho);
	LIS_DestruirLista(pFuncao->pCorpo);
	LIS_DestruirLista(pFuncao->pRodape);
	free(pFuncao);
}

void FABUI_MovToEAX(FABUI_tppFuncao pFuncaoParm, int n)
{
	int i;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	LIS_tppLista pInstrucoes = pFuncao->pCorpo;

	LIS_InserirElementoApos(pInstrucoes, MOVL_TO_EAX);

	for (i = 0; i < sizeof(int); i++)
	{
		LIS_InserirElementoApos(pInstrucoes, *((unsigned char*) &n + i));
	}
}

unsigned char* FABUI_Instrucoes(FABUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
   return LIS_ToArray(3, pFuncao->pCabecalho, pFuncao->pCorpo, pFuncao->pRodape);
}


/* funcoes encapsuladas */
