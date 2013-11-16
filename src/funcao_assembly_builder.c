#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "funcao_assembly_builder.h"

#define INICIO_CABECALHO 0
#define TAMANHO_INICIAL_CABECALHO 3

#define INICIO_CORPO 23
#define TAMANHO_INICIAL_CORPO 0

#define FIM_RODAPE 147
#define TAMANHO_INICIAL_RODAPE 4

#define TAMANHO_ESPACO_INSTRUCOES 147

#define CORRECAO_CALCULO_JUMP 2
// Para explicar esta correção vamos tomar o seguinte exemplo de instruções:
// movl %eax, 0x01        : 0xb8 0x01 0x00 0x00 0x00
// jmp (pular mov abaixo) : 0xbe 0x?? 0x?? 0x?? 0x??
// movl %eax, 0xff        : 0xb8 0xff 0x00 0x00 0x00
//
// Para pular até depois do movl é preciso usar jmp 8
// pois a contagem de quantos bytes pular começa no 2o
// byte após o jmp.

#define MOVL_TO_EAX 0xb8
#define SHORT_JMP 0xeb

typedef struct FABUI_stFuncao
{
	unsigned char *pInstrucoes;

	int tamanhoCabecalho;
	int tamanhoCorpo;
	int tamanhoRodape;
} tpFuncao;

static unsigned char pInstrucoes[] = {
	// Cabeçalho (23 bytes)
	0x55,        // pushl %ebp
	0x89, 0xe5,  // movl %esp, %ebp
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,

	// Corpo (100 bytes)
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,

	// Rodapé (24 bytes)
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00,
	0x89, 0xec,  // movl %ebp, %esp
	0x5d,        // popl %ebp
	0xc3         // ret
};


/* prototipos funcoes encapsuladas */
static void AddByteNoCorpo(FABUI_tppFuncao pFuncaoParm, unsigned char byte);
static void AddIntNoCorpo(FABUI_tppFuncao pFuncaoParm, int inteiro);

static void AddByteNoCabecalho(FABUI_tppFuncao pFuncaoParm, unsigned char byte);
static void AddIntNoCabecalho(FABUI_tppFuncao pFuncaoParm, int inteiro);

static void AddCabecalhoJumpParaOInicioDoCorpo(FABUI_tppFuncao pFuncaoParm);
static void AddCorpoJumpParaOInicioDoRodape(FABUI_tppFuncao pFuncaoParm);

/* funcoes exportadas */

FABUI_tppFuncao FABUI_CriarBuilder()
{
   tpFuncao *pFuncao = (tpFuncao*) malloc(sizeof(tpFuncao));

	 pFuncao->pInstrucoes = (unsigned char*) malloc(sizeof(unsigned char)*TAMANHO_ESPACO_INSTRUCOES);

	 memcpy(pFuncao->pInstrucoes, pInstrucoes, TAMANHO_ESPACO_INSTRUCOES);

	 pFuncao->tamanhoCabecalho = TAMANHO_INICIAL_CABECALHO;
	 pFuncao->tamanhoCorpo     = TAMANHO_INICIAL_CORPO;
	 pFuncao->tamanhoRodape    = TAMANHO_INICIAL_RODAPE;

   return pFuncao;
}

void FABUI_DestruirBuilder(FABUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	free(pFuncao);
}

void FABUI_MovToEAX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, MOVL_TO_EAX);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}

//void FABUI_JumpCondicional(FABUI_tppFuncao pFuncaoParm, FABUI_tpDestinoJump destino)
//{
//	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
//
//}

unsigned char* FABUI_Instrucoes(FABUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	 AddCabecalhoJumpParaOInicioDoCorpo(pFuncaoParm);

	 AddCorpoJumpParaOInicioDoRodape(pFuncaoParm);

	 return pFuncao->pInstrucoes;
}


/* funcoes encapsuladas */

void AddByteNoCorpo(FABUI_tppFuncao pFuncaoParm, unsigned char byte)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	unsigned char *pCorpo;
	
	pCorpo = pFuncao->pInstrucoes + INICIO_CORPO + pFuncao->tamanhoCorpo;

	*pCorpo = byte;
	pFuncao->tamanhoCorpo++;
}

void AddIntNoCorpo(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	int i;

	for (i = 0; i < sizeof(int); i++)
	{
		AddByteNoCorpo(pFuncaoParm, *((unsigned char*) &inteiro + i));
	}
}

void AddByteNoCabecalho(FABUI_tppFuncao pFuncaoParm, unsigned char byte)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	unsigned char *pCabecalho;
	
	pCabecalho = pFuncao->pInstrucoes + INICIO_CABECALHO + pFuncao->tamanhoCabecalho;

	*pCabecalho = byte;
	pFuncao->tamanhoCabecalho++;
}

void AddIntNoCabecalho(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	int i;

	for (i = 0; i < sizeof(int); i++)
	{
		AddByteNoCabecalho(pFuncaoParm, *((unsigned char*) &inteiro + i));
	}
}

void AddCabecalhoJumpParaOInicioDoCorpo(FABUI_tppFuncao pFuncaoParm)
{
	int fimCabecalho, bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimCabecalho = INICIO_CABECALHO + pFuncao->tamanhoCabecalho;
	bytesToJump = (INICIO_CORPO - fimCabecalho) - CORRECAO_CALCULO_JUMP;

	AddByteNoCabecalho(pFuncaoParm, SHORT_JMP);
	AddIntNoCabecalho(pFuncaoParm, bytesToJump);
}

void AddCorpoJumpParaOInicioDoRodape(FABUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	int fimCorpo, inicioRodape, bytesToJump;

	fimCorpo = INICIO_CORPO + pFuncao->tamanhoCorpo;
	inicioRodape = FIM_RODAPE - pFuncao->tamanhoRodape;
	bytesToJump = (inicioRodape - fimCorpo) - CORRECAO_CALCULO_JUMP;

	AddByteNoCorpo(pFuncaoParm, SHORT_JMP);
	AddIntNoCorpo(pFuncaoParm, bytesToJump);
}
