#include "readers_writers_mutexes_2.h"

const char READER_K = 'R';
const char WRITER_K = 'W';
const char UNUSED_K = 'U';

void * READER;
void * WRITER;
void * UNUSED;

void rw_mutex_init (rw_mutex_t * rw_mutex, int n_threads){
  READER = (void *) &READER_K;
  WRITER = (void *) &WRITER_K;
  UNUSED = (void *) &UNUSED_K;
  rw_mutex->n_readers = 0;
  rw_mutex->owner = UNUSED;
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  int i;
  rw_mutex->owner = READER;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : enter r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_read_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : leave r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
  rw_mutex->owner = UNUSED;
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  rw_mutex->owner = WRITER;
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  rw_mutex->owner = UNUSED;
}
