#include <pthread.h>
#include <stdlib.h>
#include "circular_buffer.h"
#include "protected_buffer.h"

protected_buffer_t * protected_buffer_init(int length) {
  protected_buffer_t * b;
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);
  // Initialize the synchronization components
  return b;
}

void * protected_buffer_get(protected_buffer_t * b){
  void * d;
  // Enter mutual exclusion
  //
  // Wait until there is a full slot to get data from the unprotected
  // circular buffer (circular_buffer_get).
  d = circular_buffer_get(b->buffer);
  // Signal or broadcast that an empty slot is available in the
  // unprotected circular buffer.
  //
  // Leave mutual exclusion
  return d;
}

int protected_buffer_put(protected_buffer_t * b, void * d){
  // Enter mutual exclusion
  //
  // Wait until there is an empty slot to put data in the unprotected
  // circular buffer (circular_buffer_put).
  circular_buffer_put(b->buffer, d);
  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer.
  //
  // Leave mutual exclusion
  return 1;
}
   
