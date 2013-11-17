typedef struct FABUI_stFuncao* FABUI_tppFuncao;

unsigned char* FABUI_Instrucoes(FABUI_tppFuncao pFuncaoParm);
FABUI_tppFuncao FABUI_CriarBuilder();
void FABUI_DestruirBuilder(FABUI_tppFuncao pFuncaoParm);


void FABUI_MovToEAX(FABUI_tppFuncao pFuncaoParm, int n);
void FABUI_JmpParaRodape(FABUI_tppFuncao pFuncaoParm);

void FABUI_SubDoESP(FABUI_tppFuncao pFuncaoParm, int qnt);
void FABUI_AddToECX(FABUI_tppFuncao pFuncaoParm, int inteiro);
void FABUI_MovECXToStack(FABUI_tppFuncao pFuncaoParm, char stackPosition);
void FABUI_MovDaStackParaEAX(FABUI_tppFuncao pFuncaoParm, char stackPosition);

void FABUI_MovToECX(FABUI_tppFuncao pFuncaoParm, int inteiro);
void FABUI_MovDaStackParaECX(FABUI_tppFuncao pFuncaoParm, char posicaoStack);
void FABUI_MovToEDX(FABUI_tppFuncao pFuncaoParm, int inteiro);
void FABUI_MovDaStackParaEDX(FABUI_tppFuncao pFuncaoParm, char posicaoStack);

void FABUI_AddEDX_ECX(FABUI_tppFuncao pFuncaoParm);
void FABUI_SubEDX_ECX(FABUI_tppFuncao pFuncaoParm);
void FABUI_MulEDX_ECX(FABUI_tppFuncao pFuncaoParm);
