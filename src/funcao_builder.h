typedef struct FBUI_stFuncao* FBUI_tppFuncao;

FBUI_tppFuncao FBUI_CriarBuilder();
void FBUI_DestruirBuilder(FBUI_tppFuncao pFuncaoParm);
unsigned char* FBUI_Instrucoes(FBUI_tppFuncao pFuncaoParm);
void FBUI_AtribuirSoma(FBUI_tppFuncao pFuncaoParm, char tipoReceptor, int nVariavel, char tipoA, int a, char op, char tipoB, int b);

void FBUI_RetornarComConstantes(FBUI_tppFuncao pFuncaoParm, int condicao, int retorno);
void FBUI_RetornarComConstanteEVariavel(FBUI_tppFuncao pFuncaoParm, int condicao, int numDaVariavelRetornada);
