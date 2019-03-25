#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "protected_buffer.h"
#include "sem_protected_buffer.h"
#include "utils.h"

#define BLOCKING 0
#define NONBLOCKING 1
#define TIMEDOUT 2

char sem_img[] = "BUT";

protected_buffer_t * protected_buffer;
long sem_impl;        // Use the semaphore implementation or not
long semantics;       // Semantics BLOCKING 0, NONBLOCKING 1, TMIEDOUT 2
long buffer_size;     // Size of the protected buffer
long n_values;        // Number of produced / consumed values 
long n_consumers;     // Nulber of consumers
long n_producers;     // Number of producers
long consumer_period; // Period of consumer (millis)
long producer_period; // Period of producer (millis)

void print_log(long time, char * kind, int id, int * data) {
  if (data == NULL)
    printf("%06ld %s %d (%c) - data=NULL\n",
	   elapsed_time(),
	   kind,
	   id,
	   sem_img[semantics]);
  else
    printf("%06ld %s %d (%c) - data=%d\n",
	   elapsed_time(),
	   kind,
	   id,
	   sem_img[semantics],
	   *data);
}

// Main consumer
void * main_consumer(void * arg){
  char  kind[] = "consumer";
  int * id = (int *) arg;
  int   i;
  int * data;
  struct timespec deadline = get_start_time();

  for (i=0; i<(n_values/n_consumers); i++) {
    // Behave as a periodic task. next deadline corresponds to the
    // current deadline + one period
    add_millis_to_timespec (&deadline, consumer_period);
    switch (semantics) {
    case BLOCKING:
      data = (int *) protected_buffer_get(protected_buffer);
      break;
    case NONBLOCKING:
      data = (int *) protected_buffer_remove(protected_buffer);
      break;
    case TIMEDOUT:
      data = (int *) protected_buffer_poll(protected_buffer, &deadline);
      break;
    default:;
    }
    print_log (elapsed_time(), kind, *id, data);
    if (data != NULL) free(data);
    delay_until (&deadline);
  }
  pthread_exit (NULL);
  return NULL;
}

// Main producer
void * main_producer(void * arg){
  char  kind[] = "producer";
  int * id = (int *) arg;
  int   i;
  int * data;
  long  done;
  struct timespec deadline = get_start_time();

  for (i=0; i<(n_values/n_producers); i++) {
    data = (int *)malloc(sizeof(int));
    *data=i;
    // Behave as a periodic task. next deadline corresponds to the
    // current deadline + one period
    add_millis_to_timespec (&deadline, producer_period);
    switch (semantics) {
    case BLOCKING:
      protected_buffer_put(protected_buffer, data);
      done=1;
      break;
      
    case NONBLOCKING:
      done=protected_buffer_add(protected_buffer, data);
      break;
      
    case TIMEDOUT:
      done=protected_buffer_offer(protected_buffer, data, &deadline);
      break;
    default:;
    }
    if (!done) data=NULL;
    print_log (elapsed_time(), kind, *id, data);
    delay_until (&deadline);
  }
  pthread_exit (NULL);
  return NULL;
}

// Read scenario file
void read_file(char * filename);

int main(int argc, char *argv[]){
  int   i;
  int * data;
  
  if (argc != 2) {
    printf("Usage : %s <scenario file>\n", argv[0]);
    exit(1);
  }

  init_utils();
  read_file(argv[1]);
  
  protected_buffer = protected_buffer_init(sem_impl, buffer_size);


  set_start_time();

  pthread_t producers[n_producers];
  pthread_t consumers[n_consumers];
  
  // Create producers and consumers
  for (i=0; i<n_producers; i++) {
      pthread_create(&producers[i], NULL, main_producer, NULL);
  }
  for (i=0; i<n_consumers; i++) {
      pthread_create(&consumers[i], NULL, main_consumer, NULL);
  }
  
  // Wait for producers and consumers termination
  for (i=0; i<n_producers; i++) {
      pthread_join(producers[i], NULL);
  }
  for (i=0; i<n_consumers; i++) {
      pthread_join(consumers[i], NULL);
  }
}

void read_file(char * filename){
  FILE * file;

  file = fopen (filename, "r");
  
  get_string (file, "#sem_impl", __FILE__, __LINE__);
  get_long   (file, (long *) &sem_impl, __FILE__, __LINE__);
  printf ("sem_impl = %ld\n", sem_impl);
  
  get_string (file, "#semantics", __FILE__, __LINE__);
  get_long   (file, (long *) &semantics, __FILE__, __LINE__);
  printf ("semantics = %ld\n", semantics);
  
  get_string (file, "#buffer_size", __FILE__, __LINE__);
  get_long   (file, (long *) &buffer_size, __FILE__, __LINE__);
  printf ("buffer_size = %ld\n", buffer_size);
  
  get_string (file, "#n_values", __FILE__, __LINE__);
  get_long   (file, (long *) &n_values, __FILE__, __LINE__);
  printf ("n_values = %ld\n", n_values);
  
  get_string (file, "#n_consumers", __FILE__, __LINE__);
  get_long   (file, (long *) &n_consumers, __FILE__, __LINE__);
  printf ("n_consumers = %ld\n", n_consumers);
  
  get_string (file, "#n_producers", __FILE__, __LINE__);
  get_long   (file, (long *) &n_producers, __FILE__, __LINE__);
  printf ("n_producers = %ld\n", n_producers);
  
  get_string (file, "#consumer_period", __FILE__, __LINE__);
  get_long   (file, (long *) &consumer_period, __FILE__, __LINE__);
  printf ("consumer_period = %ld\n", consumer_period);
  
  get_string (file, "#producer_period", __FILE__, __LINE__);
  get_long   (file, (long *) &producer_period, __FILE__, __LINE__);
  printf ("producer_period = %ld\n", producer_period);
}

