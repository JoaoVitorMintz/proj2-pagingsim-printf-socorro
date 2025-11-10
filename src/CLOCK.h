#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

typedef struct Node {
    int pagina;
    bool uso;
    struct Node* proximo;
} Node;

typedef struct Clock {
    Node* ponteiro;
} Clock;

Clock* criarClock();
bool vazia(Clock* c);
void push(Clock* c, int pagina);
void pop(Clock* c, int novaPagina);
void liberaClock(Clock* c);

#endif