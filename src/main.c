#include <stdlib.h>
#include <stdio.h>
#include "funcao_assembly_builder.h"
#include "funcao_builder.h"

typedef int (*tpFunc)();
// gcc -m32 -Wall -Wa,--execstack -o ~/Desktop/main main.c funcao_assembly_builder.c lista.c

void dump(void *pointer, int size)
{
	unsigned char *byte = (unsigned char*) pointer;
	unsigned int linhas[] = {1, 2, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 2, 1, 1};
	unsigned int *linha = linhas;
	while(--size >= 0)
	{
		printf("0x%02x ", *(byte++));
		if (--(*linha) == 0)
		{
			printf("\n");
			linha++;
		}
	}
}

int main()
{
   unsigned char* funcCode;
   tpFunc funcao;
   FBUI_tppFuncao pFunc = FBUI_CriarBuilder();

	 FBUI_Retornar(pFunc, 1, 99);

   funcCode = FBUI_Instrucoes(pFunc);
	 //dump(funcCode, 147);

   funcao = (tpFunc) funcCode;
   printf("x: %d\n", funcao());

	 return 0;
}
