#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "readers_writers_mutexes.h"

#define READER_OFFSET       10000
#define READER_EXEC_TIME    50000
#define READER_REST_TIME    20000
#define WRITER_OFFSET      100000
#define WRITER_EXEC_TIME   100000
#define WRITER_REST_TIME    40000
#define MAX_SHARED_VARIABLE    20

long shared_variable;
rw_mutex_t rw_mutex;

int main(int argc, char *argv[]) {
  pthread_t    threads[20];
  thread_conf_t args[20];
  int i, n_readers, n_writers;
  void * main_reader (void *arg);
  void * main_writer (void *arg);
  
  if (argc != 3){
    printf (" Usage : %s n_readers n_writers\n", argv[0]);
    exit(1);
  }
  
  n_readers = atoi(argv[1]);
  n_writers = atoi(argv[2]);

  rw_mutex_init(&rw_mutex);
  
  if (n_writers > 0){
    for (i = 0; i < n_writers ; i++){
      args[i].offset    = (i+1) * WRITER_OFFSET;
      args[i].exec_time = WRITER_EXEC_TIME;
      args[i].rest_time = WRITER_REST_TIME;
      args[i].increment = i + 1;
      args[i].identifier = i;
      pthread_create (&threads[i], NULL, main_writer, &args[i]);
    }
  }
  
  if (n_writers == 0) shared_variable = MAX_SHARED_VARIABLE;
  
  for (i = n_writers; i < n_writers + n_readers; i++){
    args[i].offset    = (i-n_writers+1) * READER_OFFSET;
    args[i].exec_time = READER_EXEC_TIME;
    args[i].rest_time = READER_REST_TIME;
    args[i].identifier = i;
    pthread_create (&threads[i], NULL, main_reader, &args[i]);
  }
  
  for (i = 0; i < n_writers + n_readers; i++){
    pthread_join (threads[i], NULL);
  }
  
 return (0);
}

void * main_reader (void *arg){
  thread_conf_t * conf;
  int  completed = 0;
  
  conf = (thread_conf_t *) arg;
  usleep (conf->offset);
  
  while (!completed){
    rw_mutex_read_lock (&rw_mutex, conf);
    usleep (conf->exec_time);
    completed = (MAX_SHARED_VARIABLE <= shared_variable);
    rw_mutex_read_unlock (&rw_mutex, conf);
    usleep (conf->rest_time);
  }
  printf ("reader (%ld): completed\n", conf->identifier);
  pthread_exit (NULL);
  return NULL;
}

void * main_writer (void *arg){
  int inc;
  thread_conf_t * conf;
  int completed = 0;
  
  conf = (thread_conf_t *) arg;
  inc = conf->increment;
  usleep (conf->offset);
  
  while (!completed){
    rw_mutex_writer_lock (&rw_mutex, conf);
    printf ("writer (%ld): enter v = %ld\n", conf->identifier, shared_variable);
    shared_variable = shared_variable + inc;
    usleep (conf->exec_time);
    completed = (MAX_SHARED_VARIABLE <= shared_variable);
    printf ("writer (%ld): leave v = %ld\n", conf->identifier, shared_variable);
    rw_mutex_writer_unlock (&rw_mutex, conf);
    usleep (conf->rest_time);
  }
  printf ("writer (%ld): completed\n", conf->identifier);
  return NULL;
}
