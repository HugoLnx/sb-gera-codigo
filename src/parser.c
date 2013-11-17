#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "funcao_builder.h"

#define MAX_FUNCOES 10

static void error (const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}


unsigned char** PAR_ParseProgram(char *pathPrograma)
{
  int line = 1;
  int  c;
  FILE *myfp;
	FBUI_tppFuncao pFuncao;
	int qntFuncoes = 0;
	unsigned char **ppFuncoes;
	
	ppFuncoes = (unsigned char**) malloc(sizeof(void*) * MAX_FUNCOES);

  if ((myfp = fopen (pathPrograma, "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

  while ((c = fgetc(myfp)) != EOF) {
    switch (c) {

			/***** function ******/
      case 'f': {
        char c0;
        if (fscanf(myfp, "unction%c", &c0) != 1) error("comando invalido", line);
				pFuncao = FBUI_CriarBuilder();
        break;
      }

			/***** end ******/
      case 'e': {
        char c0;
        if (fscanf(myfp, "nd%c", &c0) != 1) error("comando invalido", line);
				*(ppFuncoes+qntFuncoes) = FBUI_Instrucoes(pFuncao);
				qntFuncoes++;
        break;
      }



			/***** atribuição ******/
      case 'v': case 'p': {
        int i0;
        char v0 = c, c0;

        if (fscanf(myfp, "%d = %c", &i0, &c0) != 2)
          error("att comando invalido", line);

			  /***** call ******/
        if (c0 == 'c') {
          int f, i1;
          char v1;
          if (fscanf(myfp, "all %d %c%d", &f, &v1, &i1) != 3) 
            error("att call comando invalido", line);
          printf("%c%d = call %d %c%d\n", v0, i0, f, v1, i1);
        }


			  /***** operação aritmética ******/
        else {
          int i1, i2;
          char v1 = c0, v2, op;
          if (fscanf(myfp, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
            error("att arit comando invalido", line);
          printf("%c%d = %c%d %c %c%d\n", v0, i0, v1, i1, op, v2, i2);
        }
        break;
      }

			/***** ret ******/
      case 'r': {
        int i0, i1;
        char v0, v1;
        if (fscanf(myfp, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
           error("comando invalido", line);

				FBUI_RetornarComConstantes(pFuncao, i0, i1);

        break;
      }
      default: error("comando desconhecido", line);
    }
    line ++;
    fscanf(myfp, " ");
  }

	fclose(myfp);

  return ppFuncoes;
}
