#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>

typedef struct Node {
    int pagina;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* inicio;
    Node* fim;
    int tamanho;
    int capacidade;
} Queue;


Queue* criarFila(int capacidade);
bool vazia(Queue* f);
bool cheia(Queue* f);
bool contem(Queue* f, int pagina);
void push(Queue* f, int dado);
int pop(Queue* f);
void liberaFila(Queue* f);

#endif