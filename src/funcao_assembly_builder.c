#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "funcao_assembly_builder.h"
#include "tamanho_funcao.h"

#define INICIO_PRE_CODIGO 0
#define INICIO_CABECALHO 3
#define INICIO_CORPO 23
#define INICIO_RODAPE 223
#define INICIO_POS_CODIGO 243

#define MOVL_TO_EAX 0xb8
#define MOVL_TO_ECX 0xb9
#define MOVL_TO_EDX 0xba

#define MOVL_ENTRE_LOCAIS 0x89
#define MOVL_ECX_PARA_pEBP 0x4d
#define JMP_LONG 0xe9
#define JE_REL_P1 0x0f
#define JE_REL_P2 0x84

#define QUANTIDADE_BYTES_PARA_JMP 5
#define QUANTIDADE_BYTES_PARA_JE 6
#define MOVL_ENTRE_LOCAIS_2 0x8b
#define MOVL_pEBP_PARA_EAX 0x45
#define MOVL_pEBP_PARA_ECX 0x4d
#define MOVL_pEBP_PARA_EDX 0x55

#define OPER_L 0x81
#define SUB_ESP 0xec
#define ADD_ECX 0xc1

#define OPER_ENTRE_REGISTRADORES 0xd1
#define OPER_ADD_EDX_ECX 0x01
#define OPER_SUB_EDX_ECX 0x29

#define MUL_ENTRE_REGISTRADORES_Byte1 0x0f
#define MUL_ENTRE_REGISTRADORES_Byte2 0xaf
#define MUL_EDX_ECX 0xca

#define CMP_ECX_P1 0x81
#define CMP_ECX_P2 0xf9

#define PUSH_ECX 0x51
#define CALL_BYTE 0xe8
#define QUANTIDADE_BYTES_PARA_CALL 5
#define ADD_ESP_P1 0x83
#define ADD_ESP_P2 0xc4
#define MOVL_EAX_PARA_pEBP 0x45

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

	// Corpo (200 bytes)
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

static int BytesEntreCorpoERodape(FABUI_tppFuncao pFuncaoParm);


/* funcoes exportadas */

FABUI_tppFuncao FABUI_CriarBuilder(void *pInstrucoes2)
{
   tpFuncao *pFuncao = (tpFuncao*) malloc(sizeof(tpFuncao));

	 pFuncao->pInstrucoes = (unsigned char*) pInstrucoes2;

	 memcpy(pFuncao->pInstrucoes, pInstrucoes, TAMANHO_INSTRUCOES);

	 pFuncao->tamanhoCabecalho = 0;
	 pFuncao->tamanhoCorpo     = 0;
	 pFuncao->tamanhoRodape    = 0;

   return pFuncao;
}

void FABUI_DestruirBuilder(FABUI_tppFuncao pFuncaoParm)
{
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;
	free(pFuncao->pInstrucoes);
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
void FABUI_MovDaStackParaEAX(FABUI_tppFuncao pFuncaoParm, char stackPosition)
{
	unsigned char *pos = (unsigned char*) &stackPosition;
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS_2);
	AddByteNoCorpo(pFuncaoParm, MOVL_pEBP_PARA_EAX);
	AddByteNoCorpo(pFuncaoParm, *pos);
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

void FABUI_MovToECX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, MOVL_TO_ECX);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}

void FABUI_MovToEDX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, MOVL_TO_EDX);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}

void FABUI_MovDaStackParaECX(FABUI_tppFuncao pFuncaoParm, char posicaoStack)
{
	unsigned char *pos = (unsigned char*) &posicaoStack;
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS_2);
	AddByteNoCorpo(pFuncaoParm, MOVL_pEBP_PARA_ECX);
	AddByteNoCorpo(pFuncaoParm, *pos);
}

void FABUI_MovDaStackParaEDX(FABUI_tppFuncao pFuncaoParm, char posicaoStack)
{
	unsigned char *pos = (unsigned char*) &posicaoStack;
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS_2);
	AddByteNoCorpo(pFuncaoParm, MOVL_pEBP_PARA_EDX);
	AddByteNoCorpo(pFuncaoParm, *pos);
}


void FABUI_CmpECX(FABUI_tppFuncao pFuncaoParm, int inteiro)
{
	AddByteNoCorpo(pFuncaoParm, CMP_ECX_P1);
	AddByteNoCorpo(pFuncaoParm, CMP_ECX_P2);
	AddIntNoCorpo(pFuncaoParm, inteiro);
}


void FABUI_JmpParaRodapeSeIgual(FABUI_tppFuncao pFuncaoParm)
{
	int bytesToJump;
	bytesToJump = BytesEntreCorpoERodape(pFuncaoParm) - QUANTIDADE_BYTES_PARA_JE;

	AddByteNoCorpo(pFuncaoParm, JE_REL_P1);
	AddByteNoCorpo(pFuncaoParm, JE_REL_P2);
	AddIntNoCorpo(pFuncaoParm, bytesToJump);
}


void FABUI_AddEDX_ECX(FABUI_tppFuncao pFuncaoParm)
{
  AddByteNoCorpo(pFuncaoParm, OPER_ADD_EDX_ECX);
  AddByteNoCorpo(pFuncaoParm, OPER_ENTRE_REGISTRADORES);
}

void FABUI_SubEDX_ECX(FABUI_tppFuncao pFuncaoParm)
{
  AddByteNoCorpo(pFuncaoParm, OPER_SUB_EDX_ECX);
  AddByteNoCorpo(pFuncaoParm, OPER_ENTRE_REGISTRADORES);
}

void FABUI_MulEDX_ECX(FABUI_tppFuncao pFuncaoParm)
{
	AddByteNoCorpo(pFuncaoParm, MUL_ENTRE_REGISTRADORES_Byte1);
	AddByteNoCorpo(pFuncaoParm, MUL_ENTRE_REGISTRADORES_Byte2);
	AddByteNoCorpo(pFuncaoParm, MUL_EDX_ECX);
}

void FABUI_PushECX(FABUI_tppFuncao pFuncaoParm)
{
	AddByteNoCorpo(pFuncaoParm, PUSH_ECX);
}

void FABUI_Call(FABUI_tppFuncao pFuncaoParm, void *endereco)
{
	void *proximaInst = (void*) (pFuncaoParm->pInstrucoes + INICIO_CORPO + pFuncaoParm->tamanhoCorpo + QUANTIDADE_BYTES_PARA_CALL);
  AddByteNoCorpo(pFuncaoParm, CALL_BYTE);
	AddIntNoCorpo(pFuncaoParm, endereco - proximaInst);
}

void FABUI_AddESP(FABUI_tppFuncao pFuncaoParm, char quantidade)
{
	AddByteNoCorpo(pFuncaoParm, ADD_ESP_P1);
	AddByteNoCorpo(pFuncaoParm, ADD_ESP_P2);
	AddByteNoCorpo(pFuncaoParm, quantidade);
}

void FABUI_MovEAXToStack(FABUI_tppFuncao pFuncaoParm, char posicaoStack)
{
	AddByteNoCorpo(pFuncaoParm, MOVL_ENTRE_LOCAIS);
	AddByteNoCorpo(pFuncaoParm, MOVL_EAX_PARA_pEBP);
	AddByteNoCorpo(pFuncaoParm, posicaoStack);
}

unsigned char* FABUI_FinalizarInstrucoes(FABUI_tppFuncao pFuncaoParm)
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
	int fimCabecalho;
	int bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimCabecalho = INICIO_CABECALHO + pFuncao->tamanhoCabecalho;
	bytesToJump = (INICIO_CORPO - fimCabecalho) - QUANTIDADE_BYTES_PARA_JMP;

	AddByteNoCabecalho(pFuncaoParm, JMP_LONG);
	AddIntNoCabecalho(pFuncaoParm, bytesToJump);
}

void AddCorpoJumpParaORodape(FABUI_tppFuncao pFuncaoParm)
{
	int bytesToJump;
	bytesToJump = BytesEntreCorpoERodape(pFuncaoParm) - QUANTIDADE_BYTES_PARA_JMP;

	AddByteNoCorpo(pFuncaoParm, JMP_LONG);
	AddIntNoCorpo(pFuncaoParm, bytesToJump);
}

void AddRodapeJumpParaOPosCodigo(FABUI_tppFuncao pFuncaoParm)
{
	int fimRodape;
  int bytesToJump;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimRodape = INICIO_RODAPE + pFuncao->tamanhoRodape;
	bytesToJump = (INICIO_POS_CODIGO - fimRodape) - QUANTIDADE_BYTES_PARA_JMP;

	AddByteNoRodape(pFuncaoParm, JMP_LONG);
	AddIntNoRodape(pFuncaoParm, bytesToJump);
}

int BytesEntreCorpoERodape(FABUI_tppFuncao pFuncaoParm)
{
	int fimCorpo;
	int bytes;
	tpFuncao *pFuncao = (tpFuncao*) pFuncaoParm;

	fimCorpo = INICIO_CORPO + pFuncao->tamanhoCorpo;
	bytes = INICIO_RODAPE - fimCorpo;

	return bytes;
}
