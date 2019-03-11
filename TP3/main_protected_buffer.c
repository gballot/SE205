#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "protected_buffer.h"

#define MAX 8

protected_buffer_t * protected_buffer;

// Main consumer
void * main_consumer(void * arg){
  int i;
  int * d;

  printf ("consume %d elements\n", (int) MAX/4);
  printf ("consumer should hang for 5s\n");
  for (i = 0; i < MAX/4; i++){
    d = protected_buffer_get (protected_buffer);
    printf ("consume [%d]\n", *d);
  }
  printf ("consumer waiting for 5s\n");
  printf ("producer should hang for 5s\n");
  sleep (5);
  printf ("consume %d elements\n", (int) (MAX - MAX/4));
  for (; i < MAX; i++){
    d = protected_buffer_get (protected_buffer);
    printf ("consume [%d]\n", *d);
  }
  pthread_exit (NULL);
  return NULL;
}

// Main producer
int main(int argc, char *argv[]){
  int   i;
  int * d;
  
  protected_buffer = protected_buffer_init(MAX);

  // Creer un thread consumer qui exécute main_consumer

  printf ("producer waiting for 5s\n");
  sleep (5);
  printf ("produce %d elements\n", 2 * MAX);

  for (i = 0; i < 2 * MAX; i++) {
    d = (int *)malloc(sizeof(int));
    *d = i;
    protected_buffer_put(protected_buffer, d);
    printf ("produce [%d]\n", i);
  }
  
  // Attendre la terminaison de consumer

  return 0;
}





