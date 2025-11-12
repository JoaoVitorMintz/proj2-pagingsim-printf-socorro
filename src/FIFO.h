#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>

typedef struct NodeQ {
    int pagina;
    struct NodeQ* next;
} NodeQ;

typedef struct {
    NodeQ* inicio;
    NodeQ* fim;
    int tamanho;
    int capacidade;
} Queue;

Queue* criarFila(int capacidade);
bool vazia(Queue* f);
bool cheia(Queue* f);
bool contem(Queue* f, int pagina);
void push(Queue* f, int pagina);
int pop(Queue* f);
void liberaFila(Queue* f);

#endif
