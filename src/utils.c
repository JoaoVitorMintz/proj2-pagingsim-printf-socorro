#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Lê config com formato:
   <num_frames>
   <tamanho_pagina>
   <num_processos>
   (opcionais linhas PID tamanho)
*/
void lerConfiguracao(const char* arquivo, int* numFrames, int* pageSize) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir arquivo de configuração!\n");
        exit(1);
    }
    if (fscanf(f, "%d", numFrames) != 1) {
        printf("Formato inválido em config (numFrames)\n");
        fclose(f);
        exit(1);
    }
    if (fscanf(f, "%d", pageSize) != 1) {
        printf("Formato inválido em config (pageSize)\n");
        fclose(f);
        exit(1);
    }
    /* ler num_procs e pular pares (PID tam) se existirem */
    int num_procs = 0;
    if (fscanf(f, "%d", &num_procs) == 1) {
        for (int i = 0; i < num_procs; ++i) {
            int pid, tam;
            if (fscanf(f, "%d %d", &pid, &tam) != 2) break;
        }
    }
    fclose(f);
}

void lerAcessos(const char* arquivo, int acessos[][2], int* qtdAcessos) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir arquivo de acessos!\n");
        exit(1);
    }
    int pid, endereco;
    *qtdAcessos = 0;
    while (fscanf(f, "%d %d", &pid, &endereco) == 2) {
        acessos[*qtdAcessos][0] = pid;
        acessos[*qtdAcessos][1] = endereco;
        (*qtdAcessos)++;
    }
    fclose(f);
}
