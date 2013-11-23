#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "gera.h"

void gera(FILE *f, void **code, funcp *entry)
{
	unsigned char *ultima;
	*code = (void *) PAR_ParseProgram(f, &ultima);
	*entry = (funcp) ultima;
}

void libera(void *code)
{
	free(code);
}
