#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "protected_buffer.h"

// Initialise the protected buffer structure above. 
protected_buffer_t * cond_protected_buffer_init(int length) {
  protected_buffer_t * b;
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);
  // Initialize the synchronization components
  pthread_mutex_init(b->mutex, NULL);
  pthread_cond_init(b->not_empty, NULL);
  pthread_cond_init(b->not_full, NULL);
  b->count = 0;
  return b;
}

// Extract an element from buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void * cond_protected_buffer_get(protected_buffer_t * b){
  void * d;
  
  // Enter mutual exclusion
  pthread_mutex_lock(b->mutex);
  
  // Wait until there is a full slot to get data from the unprotected
  // circular buffer (circular_buffer_get).
  while(b->count <= 0)
      pthread_cond_wait(b->not_empty, b->mutex);
  
  // Signal or broadcast that an empty slot is available in the
  // unprotected circular buffer (if needed)
  if (b->count == b->buffer->size)
      pthread_cond_broadcast(b->not_full);

  d = circular_buffer_get(b->buffer);
  b->count--;

  // Leave mutual exclusion
  pthread_mutex_unlock(b->mutex);
  
  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void cond_protected_buffer_put(protected_buffer_t * b, void * d){

  // Enter mutual exclusion
  pthread_mutex_lock(b->mutex);
  
  // Wait until there is an empty slot to put data in the unprotected
  // circular buffer (circular_buffer_put).
  while(b->count >= b->buffer->size)
      pthread_cond_wait(b->not_full, b->mutex);
  
  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)
  if (b->count == 0)
      pthread_cond_broadcast(b->not_empty);

  circular_buffer_put(b->buffer, d);
  b->count++;

  // Leave mutual exclusion
  pthread_mutex_unlock(b->mutex);
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, return NULL. Otherwise, return the element.
void * cond_protected_buffer_remove(protected_buffer_t * b){
  void * d;
  

  // Signal or broadcast that an empty slot is available in the
  // unprotected circular buffer (if needed)

  d = circular_buffer_get(b->buffer);
  
  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, return 0. Otherwise, return 1.
int cond_protected_buffer_add(protected_buffer_t * b, void * d){
  int done;
  
  // Enter mutual exclusion
  
  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)

  done = circular_buffer_put(b->buffer, d);

  // Leave mutual exclusion
  return done;
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return the element if
// successful. Otherwise, return NULL.
void * cond_protected_buffer_poll(protected_buffer_t * b, struct timespec *abstime){
  void * d = NULL;
  int    rc = -1;
  
  // Enter mutual exclusion
  
  // Wait until there is an empty slot to put data in the unprotected
  // circular buffer (circular_buffer_put) but waits no longer than
  // the given timeout.
  
  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)

  d = circular_buffer_get(b->buffer);
  
  // Leave mutual exclusion
  return d;
}

// Insert an element into buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return 0 if not
// successful. Otherwise, return 1.
int cond_protected_buffer_offer(protected_buffer_t * b, void * d, struct timespec * abstime){
  int rc = -1;
  int done;
  
  // Enter mutual exclusion
  
  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed) but waits no longer than
  // the given timeout.

  done = circular_buffer_put(b->buffer, d);

  // Leave mutual exclusion
  return done;
}
