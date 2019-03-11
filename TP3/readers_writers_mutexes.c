#include "readers_writers_mutexes.h"

void rw_mutex_init (rw_mutex_t * rw_mutex){
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : enter r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_read_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : leave r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
}
