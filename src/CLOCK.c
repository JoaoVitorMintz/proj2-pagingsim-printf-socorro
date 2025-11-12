#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CLOCK.h"

/* Implementação de lista circular simples para o CLOCK.
   Mantivemos os nomes originais (push/pop) mas com prefixo clock_ para evitar colisões. */

Clock* clock_criar() {
    Clock* novoClock = (Clock*)malloc(sizeof(Clock));
    if (novoClock == NULL) {
        printf("Erro ao alocar memória para o CLOCK!\n");
        exit(1);
    }
    novoClock->ponteiro = NULL;
    return novoClock;
}

bool clock_vazia(Clock* c) {
    return (c->ponteiro == NULL);
}

/* Insere página no relógio (usada quando ainda há frame livre) */
void clock_push(Clock* c, int pagina) {
    NodeC* novoNo = (NodeC*)malloc(sizeof(NodeC));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para o nó do CLOCK!\n");
        exit(1);
    }
    novoNo->pagina = pagina;
    novoNo->uso = true;

    if (clock_vazia(c)) {
        novoNo->proximo = novoNo;
        c->ponteiro = novoNo;
    } else {
        /* insere logo depois do ponteiro (implementação simples) */
        novoNo->proximo = c->ponteiro->proximo;
        c->ponteiro->proximo = novoNo;
    }
}

/* Substitui conforme algoritmo CLOCK (segunda chance).
   Retorna a página que foi evict (antiga) ou -1 em erro. */
int clock_replace(Clock* c, int novaPagina) {
    if (clock_vazia(c)) return -1;

    while (true) {
        if (!c->ponteiro->uso) {
            int evicted = c->ponteiro->pagina;
            c->ponteiro->pagina = novaPagina;
            c->ponteiro->uso = true;
            c->ponteiro = c->ponteiro->proximo;
            return evicted;
        } else {
            /* dá segunda chance: zera bit e avança */
            c->ponteiro->uso = false;
            c->ponteiro = c->ponteiro->proximo;
        }
    }
}

void clock_libera(Clock* c) {
    if (clock_vazia(c)) {
        free(c);
        return;
    }
    NodeC* inicio = c->ponteiro;
    NodeC* atual = inicio->proximo;
    while (atual != inicio) {
        NodeC* aux = atual;
        atual = atual->proximo;
        free(aux);
    }
    free(inicio);
    free(c);
}
