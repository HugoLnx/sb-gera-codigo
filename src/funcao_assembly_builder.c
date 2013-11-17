#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "funcao_assembly_builder.h"

#define INICIO_PRE_CODIGO 0
#define INICIO_CABECALHO 3
#define INICIO_CORPO 23
#define INICIO_RODAPE 123
#define INICIO_POS_CODIGO 143

#define TAMANHO_INSTRUCOES 147

#define CORRECAO_CALCULO_JUMP_RELATIVO 2
// Para explicar esta correção vamos tomar o seguinte exemplo de instruções:
// movl %eax, 0x01        : 0xb8 0x01 0x00 0x00 0x00
// jmp (pular mov abaixo) : 0xbe 0x?? 0x?? 0x?? 0x??
// movl %eax, 0xff        : 0xb8 0xff 0x00 0x00 0x00
//
// Para pular até depois do movl é preciso usar jmp 8
// pois a contagem de quantos bytes pular começa no 2o
// byte após o jmp.

#define MOVL_TO_EAX 0xb8
#define MOVL_TO_ECX 0xb9

#define MOVL_ENTRE_LOCAIS 0x89
#define MOVL_ECX_PARA_pEBP 0x4d
#define JMP_REL 0xeb

#define MOVL_ENTRE_LOCAIS_2 0x8b
#define MOVL_pEBP_PARA_EAX 0x45

#define OPER_L 0x81
#define SUB_ESP 0xec
#define ADD_ECX 0xc1

typedef struct FABUI_stFuncao
{
	unsigned char *pInstrucoes;

	int tamanhoCabecalho;
	int tamanhoCorpo;
	int tamanhoRodape;
} tpFuncao;

static unsigned char pInstrucoes[] = {
	0x55,        // pushl %ebp
	0x89, 0xe5,  // movl %esp, %ebp

	// Cabeçalho (20 bytes)
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

	// Rodapé (20 bytes)
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

static void AddByteNoRodape(FABUI_tppFuncao pFuncaoParm, unsigned char byte);
static void AddIntNoRodape(FABUI_tppFuncao pFuncaoParm, int inteiro);

static void AddCabecalhoJumpParaOInicioDoCorpo(FABUI_tppFuncao pFuncaoParm);
static void AddCorpoJumpParaORodape(FABUI_tppFuncao pFuncaoParm);
static void AddRodapeJumpParaOPosCodigo(FABUI_tppFuncao pFuncaoParm);


/* funcoes exportadas */

FABUI_tppFuncao FABUI_CriarBuilder()
{
   tpFuncao *pFuncao = (tpFuncao*) malloc(sizeof(tpFuncao));

	 pFuncao->pInstrucoes = (unsigned char*) malloc(sizeof(unsigned char)*TAMANHO_INSTRUCOES);

	 memcpy(pFuncao->pInstrucoes, pInstrucoes, TAMANHO_INSTRUCOES);

	 pFuncao->tamanhoCabecalho = 0;
	 pFuncao->tamanhoCorpo     = 0;
	 pFuncao->tamanhoRodape    = 0;

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

void FABUI_JmpParaRodape(FABUI_tppFuncao pFuncaoParm)
{
  AddCorpoJumpParaORodape(pFuncaoParm);
}

void FABUI_SubDoESP(FABUI_tppFuncao pFuncaoParm, int qnt)
{
	AddByteNoCorpo(pFuncaoParm, OPER_L);
	AddByteNoCorpo(pFuncaoParm, SUB_ESP);
	AddIntNoCorpo(pFuncaoParm, qnt);
}

void FABUI_MovToECX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, MOVL_TO_ECX);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}

void FABUI_AddToECX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, OPER_L);
	AddByteNoCorpo(pFuncaoParm, ADD_ECX);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}

void FABUI_MovECXToStack(FABUI_tppFuncao pFuncaoParm, char stackPosition)
{
	unsigned char *pos = (unsigned char*) &stackPosition;
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS);
	AddByteNoCorpo(pFuncaoParm, MOVL_ECX_PARA_pEBP);
	AddByteNoCorpo(pFuncaoParm, *pos);
}

void FABUI_MovDaStackParaEAX(FABUI_tppFuncao pFuncaoParm, char stackPosition)
{
	unsigned char *pos = (unsigned char*) &stackPosition;
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS_2);
	AddByteNoCorpo(pFuncaoParm, MOVL_pEBP_PARA_EAX);
	AddByteNoCorpo(pFuncaoParm, *pos);
}

unsigned char* FABUI_Instrucoes(FABUI_tppFuncao pFuncaoParm)
{
   tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	 AddCabecalhoJumpParaOInicioDoCorpo(pFuncaoParm);
	 AddCorpoJumpParaORodape(pFuncaoParm);
	 AddRodapeJumpParaOPosCodigo(pFuncaoParm);

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

void AddByteNoRodape(FABUI_tppFuncao pFuncaoParm, unsigned char byte)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	unsigned char *pRodape;
	
	pRodape = pFuncao->pInstrucoes + INICIO_RODAPE + pFuncao->tamanhoRodape;

	*pRodape = byte;
	pFuncao->tamanhoRodape++;
}

void AddIntNoRodape(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	int i;

	for (i = 0; i < sizeof(int); i++)
	{
		AddByteNoRodape(pFuncaoParm, *((unsigned char*) &inteiro + i));
	}
}

void AddCabecalhoJumpParaOInicioDoCorpo(FABUI_tppFuncao pFuncaoParm)
{
	int fimCabecalho, bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimCabecalho = INICIO_CABECALHO + pFuncao->tamanhoCabecalho;
	bytesToJump = (INICIO_CORPO - fimCabecalho) - CORRECAO_CALCULO_JUMP_RELATIVO;

	AddByteNoCabecalho(pFuncaoParm, JMP_REL);
	AddIntNoCabecalho(pFuncaoParm, bytesToJump);
}

void AddCorpoJumpParaORodape(FABUI_tppFuncao pFuncaoParm)
{
	int fimCorpo, bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimCorpo = INICIO_CORPO + pFuncao->tamanhoCorpo;
	bytesToJump = (INICIO_RODAPE - fimCorpo) - CORRECAO_CALCULO_JUMP_RELATIVO;

	AddByteNoCorpo(pFuncaoParm, JMP_REL);
	AddIntNoCorpo(pFuncaoParm, bytesToJump);
}

void AddRodapeJumpParaOPosCodigo(FABUI_tppFuncao pFuncaoParm)
{
	int fimRodape, bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimRodape = INICIO_RODAPE + pFuncao->tamanhoRodape;
	bytesToJump = (INICIO_POS_CODIGO - fimRodape) - CORRECAO_CALCULO_JUMP_RELATIVO;

	AddByteNoRodape(pFuncaoParm, JMP_REL);
	AddIntNoRodape(pFuncaoParm, bytesToJump);
}
