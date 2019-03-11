#ifndef PROTECTED_BUFFER_H
#define PROTECTED_BUFFER_H
#include <pthread.h>
#include <stdlib.h>
#include "circular_buffer.h"

typedef struct {
  pthread_mutex_t mutex;
  circular_buffer_t * buffer;
} protected_buffer_t;

// Initialise la structure du tampon circulaire
protected_buffer_t * protected_buffer_init(int length);

// Retire une donnée du tampon circulaire et bloque s'il est vide.
void * protected_buffer_get(protected_buffer_t * b);

// Ajoute une donnée dans le tampon circulaire et bloque s'il est plein
int protected_buffer_put(protected_buffer_t * b, void * d);
#endif
