#ifndef MMU_H
#define MMU_H

void inicializarMemoria(int numFrames, int pageSize);
void acessarPaginaFIFO(int pid, int endereco);
void acessarPaginaCLOCK(int pid, int endereco);
void liberarMemoria();

#endif
