#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mmu.h"

/* os contadores são definidos em mmu.c */
extern int totalAcessos;
extern int totalPageFaults;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Uso: %s <config.txt> <acessos.txt> <fifo|clock>\n", argv[0]);
        return 1;
    }

    const char* arquivoConfig = argv[1];
    const char* arquivoAcessos = argv[2];
    const char* algoritmo = argv[3];

    int numFrames = 0;
    int pageSize = 10;
    lerConfiguracao(arquivoConfig, &numFrames, &pageSize);

    inicializarMemoria(numFrames, pageSize);

    /* ler acessos em memória (array simples) */
    int acessos[2000][2];
    int qtdAcessos = 0;
    lerAcessos(arquivoAcessos, acessos, &qtdAcessos);

    printf("Simulador de Paginação Iniciado (%s)\n", algoritmo);

    for (int i = 0; i < qtdAcessos; ++i) {
        int pid = acessos[i][0];
        int endereco = acessos[i][1];
        int pagina = endereco / pageSize;
        int desloc = endereco % pageSize;

        /* imprime prefixo da linha (igual formato anterior) */
        printf("Acesso: PID %d, Endereço %d (Página %d, Deslocamento %d)", pid, endereco, pagina, desloc);

        if (strcmp(algoritmo, "fifo") == 0) {
            acessarPaginaFIFO(pid, endereco);
        } else if (strcmp(algoritmo, "clock") == 0) {
            acessarPaginaCLOCK(pid, endereco);
        } else {
            printf(" -> Algoritmo desconhecido!\n");
        }
    }

    printf("--- Simulação Finalizada (Algoritmo: %s)\n", algoritmo);
    printf("Total de Acessos: %d\n", totalAcessos);
    printf("Total de Page Faults: %d\n", totalPageFaults);

    liberarMemoria();
    return 0;
}
