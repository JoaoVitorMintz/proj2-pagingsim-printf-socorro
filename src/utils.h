#ifndef UTILS_H
#define UTILS_H

void lerConfiguracao(const char* arquivo, int* numFrames, int* pageSize);
void lerAcessos(const char* arquivo, int acessos[][2], int* qtdAcessos);

#endif
