typedef struct FBUI_stFuncao* FBUI_tppFuncao;

FBUI_tppFuncao FBUI_CriarBuilder(void *pInstrucoes);
void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm);
unsigned char* FBUI_FinalizarInstrucoes(FBUI_tppFuncao pFuncaoParm);
void FBUI_AtribuirSoma(FBUI_tppFuncao pFuncaoParm, char tipoReceptor, int nVariavel, char tipoA, int a, char op, char tipoB, int b);

void FBUI_Retornar(FBUI_tppFuncao pFuncaoParm, char tipoCheck, int numCheck, char tipoRetorno, int numRetorno);
void FBUI_Invocar(FBUI_tppFuncao pFuncaoParm, char tipoReceptor, int numReceptor, void *endereco, char tipoParametro, int numParametro);
