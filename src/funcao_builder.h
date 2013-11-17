typedef struct FBUI_stFuncao* FBUI_tppFuncao;

FBUI_tppFuncao FBUI_CriarBuilder();
void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm);
unsigned char* FBUI_Instrucoes(FBUI_tppFuncao pFuncaoParm);
void FBUI_Retornar(FBUI_tppFuncao pFuncaoParm, int condicao, int retorno);
