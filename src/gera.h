/* Hugo Roque de Figueiredo 1311162 */
/* Robert Correa dos Santos 1210325 */

typedef int (*funcp)();
void gera(FILE *f, void **code, funcp *entry);
void libera(void *code);
