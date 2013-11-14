typedef struct FABUI_stFuncao* FABUI_tppFuncao;

unsigned char* FABUI_Instrucoes(FABUI_tppFuncao pFuncaoParm);
void FABUI_MovToEAX(FABUI_tppFuncao pFuncaoParm, int n);
FABUI_tppFuncao FABUI_CriarBuilder();
void FABUI_DestruirBuilder(FABUI_tppFuncao pFuncaoParm);