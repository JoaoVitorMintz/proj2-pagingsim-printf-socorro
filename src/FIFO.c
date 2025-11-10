#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "FIFO.h"

// Cria e inicializa uma nova fila vazia
Queue criarFila(int capacidade) {
    Queue* novaFila = (Queue*)malloc(sizeof(Queue));
    if (novaFila == NULL) {
        printf("Erro ao alocar memória para a fila!\n");// Facilitar depuração de erro na alocação (idealmente nunca aparecerá)
        exit(1);
    }
    novaFila->inicio = NULL;
    novaFila->fim = NULL;
    f->tamanho = 0;
    f->capacidade = capacidade;
    return novaFila;
}

// Verifica se a fila está vazia
bool vazia(Queue* f) {
    return (f->inicio == NULL);
}

// Verificar se está cheia
bool cheia(Queue* f) {
    return f->tamanho == f->capacidade;
}

bool contem(Queue* f, int pagina) {
    Node* atual = f->inicio;
    while (atual) {
        if (atual->pagina == pagina) return true;
        atual = atual->next;
    }
    return false;
}

// Inserir valor na fila
void push(Queue* f, int dado) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó!\n");// Facilitar depuração de erro na alocação (idealmente nunca aparecerá)
        exit(1);
    }
    novoNo->data = dado;
    novoNo->next = NULL;

    if (vazia(f)) {
        f->inicio = novoNo;
    } else {
        f->fim->next = novoNo; // o próximo do último nó atual aponta para o novo nó
    }
    f->fim = novoNo; // novo nó se torna o novo último nó
    f->tamanho++;
}

// Remover valor da fila
int pop(Queue* f) {
    if (vazia(f)) {
        return -1;
    }

    Node* noARemover = f->inicio;
    int pagina = noARemover->pagina;
    f->inicio = f->inicio->next;

    free(noARemover);
    f->tamanho--;
    return pagina;
}

// Liberar memória da fila
void liberaFila(Queue* f) {
    while(!vazia(f)) {
        pop(f);
    }
    free(f);
}
