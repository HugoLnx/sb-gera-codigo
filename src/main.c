#include <stdlib.h>
#include <stdio.h>
#include "funcao_assembly_builder.h"

typedef int (*tpFunc)();
// gcc -m32 -Wall -Wa,--execstack -o ~/Desktop/main main.c funcao_assembly_builder.c lista.c

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %x\n", p1, *p1);
    p1++;
  }
}

int main()
{
   unsigned char* funcCode;
   tpFunc funcao;
   FABUI_tppFuncao pFunc = FABUI_CriarBuilder();

   FABUI_MovToEAX(pFunc, 10);

   funcCode = FABUI_Instrucoes(pFunc);

   funcao = (tpFunc) funcCode;
   printf("x: %d\n", funcao());

	 return 0;
}
