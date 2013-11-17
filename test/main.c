#include <stdio.h>
#include "../src/parser.h"

typedef int (*funcp)();

void deveRetornar(char *pathPrograma, int retornoEsperado)
{
	unsigned char **codes;
	int retorno;
	funcp func;
	codes = PAR_ParseProgram(pathPrograma);
	func = (funcp) codes[0];

	retorno = func();
	if (retorno != retornoEsperado)
	{
		printf("%s retorno: %d; esperado:%d \n", pathPrograma, retorno, retornoEsperado);
	}
}

int main()
{
	deveRetornar("program1.sb", 50);
  return 0;
}
