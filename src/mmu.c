#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmu.h"
#include "FIFO.h"
#include "CLOCK.h"

/* Contadores globais (visíveis em simulador.c via extern) */
int totalAcessos = 0;
int totalPageFaults = 0;

/* Configuração interna */
static int numFramesGlobais = 0;
static int pageSizeGlob = 10;

/* Estrutura de frame usada internamente */
typedef struct {
    int pid;
    int pagina;
    int ocupado; /* 0 = livre, 1 = ocupado */
} FrameEntry;

static FrameEntry *frames = NULL;
static Queue *filaFIFO = NULL;   /* fila que guarda índices de frame (usamos 'pagina' do NodeQ para guardar índice) */
static Clock *clockStruct = NULL; /* lista circular que guarda páginas (valores de página) */

/* inicializa a "memória física" */
void inicializarMemoria(int numFrames, int pageSize) {
    numFramesGlobais = numFrames;
    pageSizeGlob = pageSize;
    if (frames) free(frames);
    frames = (FrameEntry*) malloc(sizeof(FrameEntry) * numFramesGlobais);
    for (int i = 0; i < numFramesGlobais; ++i) {
        frames[i].pid = -1;
        frames[i].pagina = -1;
        frames[i].ocupado = 0;
    }
    if (filaFIFO) liberaFila(filaFIFO);
    filaFIFO = criarFila(numFramesGlobais);
    if (clockStruct) clock_libera(clockStruct);
    clockStruct = clock_criar();
    totalAcessos = 0;
    totalPageFaults = 0;
}

/* busca frame por (pid,pagina) - retorna índice ou -1 */
static int busca_frame_por_pid_pagina(int pid, int pagina) {
    for (int i = 0; i < numFramesGlobais; ++i) {
        if (frames[i].ocupado && frames[i].pid == pid && frames[i].pagina == pagina) return i;
    }
    return -1;
}

/* procura primeiro frame livre, ou -1 se nenhum */
static int encontra_frame_livre() {
    for (int i = 0; i < numFramesGlobais; ++i) {
        if (!frames[i].ocupado) return i;
    }
    return -1;
}

/* auxiliar: procura frame que contém certa página (retorna índice ou -1) */
static int encontra_frame_por_pagina(int pagina) {
    for (int i = 0; i < numFramesGlobais; ++i) {
        if (frames[i].ocupado && frames[i].pagina == pagina) return i;
    }
    return -1;
}

/* marcar bit de uso no CLOCK (procura node com página igual e seta uso=1) */
static void marcar_uso_clock(int pagina) {
    if (!clockStruct) return;
    NodeC *start = clockStruct->ponteiro;
    if (!start) return;
    NodeC *cur = start;
    do {
        if (cur->pagina == pagina) { cur->uso = true; return; }
        cur = cur->proximo;
    } while (cur != start);
}

/* ------------------ implementação FIFO ------------------ */
/* recebe pid e endereco (endereço inteiro), faz tradução e imprime mensagens */
void acessarPaginaFIFO(int pid, int endereco) {
    totalAcessos++;
    int pagina = endereco / pageSizeGlob;
    

    int frame = busca_frame_por_pid_pagina(pid, pagina);
    if (frame != -1) {
        /* HIT */
        printf(" -> HIT: Página %d (PID %d) já está no Frame %d\n", pagina, pid, frame);
        return;
    }

    /* page fault */
    totalPageFaults++;
    int free_idx = encontra_frame_livre();
    if (free_idx != -1) {
        frames[free_idx].ocupado = 1;
        frames[free_idx].pid = pid;
        frames[free_idx].pagina = pagina;
        /* guardamos o índice do frame na fila FIFO */
        push(filaFIFO, free_idx);
        printf(" -> PAGE FAULT -> Página %d (PID %d) alocada no Frame livre %d\n", pagina, pid, free_idx);
    } else {
        /* memória cheia: retirar primeiro índice da fila FIFO */
        int victim_idx = pop(filaFIFO);
        if (victim_idx < 0) victim_idx = 0; /* fallback seguro */
        int vpid = frames[victim_idx].pid;
        int vpage = frames[victim_idx].pagina;
        frames[victim_idx].pid = pid;
        frames[victim_idx].pagina = pagina;
        printf(" -> PAGE FAULT -> Memória cheia. Página %d (PID %d) (Frame %d) será desalocada. -> Página %d (PID %d) alocada no Frame %d\n",
               vpage, vpid, victim_idx, pagina, pid, victim_idx);
        /* colocar de volta no final da fila como mais recente */
        push(filaFIFO, victim_idx);
    }
}

/* ------------------ implementação CLOCK ------------------ */
void acessarPaginaCLOCK(int pid, int endereco) {
    totalAcessos++;
    int pagina = endereco / pageSizeGlob;
    

    int frame = busca_frame_por_pid_pagina(pid, pagina);
    if (frame != -1) {
        /* HIT: marcar bit de uso no relógio */
        printf(" -> HIT: Página %d (PID %d) já está no Frame %d\n", pagina, pid, frame);
        marcar_uso_clock(pagina);
        return;
    }

    /* page fault */
    totalPageFaults++;
    int free_idx = encontra_frame_livre();
    if (free_idx != -1) {
        frames[free_idx].ocupado = 1;
        frames[free_idx].pid = pid;
        frames[free_idx].pagina = pagina;
        /* inserir a página no clock (armazenamos página) */
        clock_push(clockStruct, pagina);
        printf(" -> PAGE FAULT -> Página %d (PID %d) alocada no Frame livre %d\n", pagina, pid, free_idx);
    } else {
        /* memória cheia -> use clock_replace para obter página evictada */
        int evicted_page = clock_replace(clockStruct, pagina);
        /* localizar frame que tinha essa página */
        int victim_idx = encontra_frame_por_pagina(evicted_page);
        if (victim_idx == -1) victim_idx = 0; /* fallback */
        int vpid = frames[victim_idx].pid;
        int vpage = frames[victim_idx].pagina;
        frames[victim_idx].pid = pid;
        frames[victim_idx].pagina = pagina;
        printf(" -> PAGE FAULT -> Memória cheia. Página %d (PID %d) (Frame %d) será desalocada. -> Página %d (PID %d) alocada no Frame %d\n",
               vpage, vpid, victim_idx, pagina, pid, victim_idx);
    }
}

/* free de estruturas */
void liberarMemoria() {
    if (filaFIFO) { liberaFila(filaFIFO); filaFIFO = NULL; }
    if (clockStruct) { clock_libera(clockStruct); clockStruct = NULL; }
    if (frames) { free(frames); frames = NULL; }
}
