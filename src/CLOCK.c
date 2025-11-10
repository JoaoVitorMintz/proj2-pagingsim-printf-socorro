#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CLOCK.h"

// Cria e inicializa uma nova estrutura CLOCK vazia
Clock* criarClock() {
    Clock* novoClock = (Clock*)malloc(sizeof(Clock));
    if (novoClock == NULL) {
        printf("Erro ao alocar memória para o CLOCK!\n"); // Facilitar depuração de erro na alocação
        exit(1);
    }
    novoClock->ponteiro = NULL;
    return novoClock;
}

// Verifica se o CLOCK está vazio
bool vazia(Clock* c) {
    return (c->ponteiro == NULL);
}

// Inserir página no CLOCK
void push(Clock* c, int pagina) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó!\n"); // Facilitar depuração de erro na alocação
        exit(1);
    }

    novoNo->pagina = pagina;
    novoNo->uso = true;

    if (vazia(c)) {
        novoNo->proximo = novoNo; // Primeiro nó aponta pra si mesmo
        c->ponteiro = novoNo;
    } else {
        novoNo->proximo = c->ponteiro->proximo;
        c->ponteiro->proximo = novoNo;
    }
}

// Substituir página segundo o algoritmo CLOCK
void pop(Clock* c, int novaPagina) {
    if (vazia(c)) {
        printf("CLOCK vazio, não há página para substituir!\n");
        exit(1);
    }

    while (true) {
        if (c->ponteiro->uso == false) {
            c->ponteiro->pagina = novaPagina;
            c->ponteiro->uso = true;
            c->ponteiro = c->ponteiro->proximo;
            break;
        } else {
            c->ponteiro->uso = false;
            c->ponteiro = c->ponteiro->proximo;
        }
    }
}

// Libera memória da estrutura CLOCK
void liberaClock(Clock* c) {
    if (vazia(c)) {
        free(c);
        return;
    }

    Node* atual = c->ponteiro->proximo;
    Node* inicio = c->ponteiro;
    while (atual != inicio) {
        Node* aux = atual;
        atual = atual->proximo;
        free(aux);
    }
    free(inicio);
    free(c);
}

int main() {
    Clock* c = criarClock();

    return 0;
}
