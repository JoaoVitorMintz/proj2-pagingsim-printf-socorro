#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "FIFO.h"

Queue* criarFila(int capacidade) {
    Queue* novaFila = (Queue*)malloc(sizeof(Queue));
    if (novaFila == NULL) {
        printf("Erro ao alocar memória para a fila!\n");
        exit(1);
    }
    novaFila->inicio = NULL;
    novaFila->fim = NULL;
    novaFila->tamanho = 0;
    novaFila->capacidade = capacidade;
    return novaFila;
}

bool vazia(Queue* f) {
    return (f->inicio == NULL);
}

bool cheia(Queue* f) {
    return f->tamanho >= f->capacidade;
}

bool contem(Queue* f, int pagina) {
    NodeQ* atual = f->inicio;
    while (atual) {
        if (atual->pagina == pagina) return true;
        atual = atual->next;
    }
    return false;
}

void push(Queue* f, int pagina) {
    NodeQ* novoNo = (NodeQ*)malloc(sizeof(NodeQ));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó!\n");
        exit(1);
    }
    novoNo->pagina = pagina;
    novoNo->next = NULL;

    if (vazia(f)) {
        f->inicio = novoNo;
    } else {
        f->fim->next = novoNo;
    }
    f->fim = novoNo;
    f->tamanho++;
}

int pop(Queue* f) {
    if (vazia(f)) return -1;

    NodeQ* noRemovido = f->inicio;
    int pagina = noRemovido->pagina;
    f->inicio = noRemovido->next;

    if (f->inicio == NULL) f->fim = NULL;

    free(noRemovido);
    f->tamanho--;
    return pagina;
}

void liberaFila(Queue* f) {
    while (!vazia(f)) pop(f);
    free(f);
}
