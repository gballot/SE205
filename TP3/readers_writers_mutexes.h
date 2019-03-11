#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
  int n_readers;
} rw_mutex_t;

typedef struct {
  rw_mutex_t *rw_mutex; 
  long offset;
  long exec_time;
  long rest_time;
  long increment;
  long identifier;
} thread_conf_t;

extern long shared_variable;

void rw_mutex_init(rw_mutex_t * rw_mutex);
void rw_mutex_read_lock (rw_mutex_t * rw_mutex, thread_conf_t * conf);
void rw_mutex_read_unlock (rw_mutex_t * rw_mutex, thread_conf_t * conf);
void rw_mutex_writer_lock (rw_mutex_t * rw_mutex, thread_conf_t * conf);
void rw_mutex_writer_unlock (rw_mutex_t * rw_mutex, thread_conf_t * conf);
