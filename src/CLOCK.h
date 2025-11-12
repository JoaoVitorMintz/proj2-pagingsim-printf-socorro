#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

typedef struct NodeC {
    int pagina;
    bool uso;
    struct NodeC* proximo;
} NodeC;

typedef struct {
    NodeC* ponteiro;
} Clock;

Clock* clock_criar();
bool clock_vazia(Clock* c);
void clock_push(Clock* c, int pagina);
int clock_replace(Clock* c, int novaPagina); /* substitui e retorna página evict (ou -1) */
void clock_libera(Clock* c);

#endif
