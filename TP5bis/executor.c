#include <stdio.h>
#include <sys/time.h>

#include "executor.h"
#include "utils.h"

pthread_mutex_t mts0;
pthread_cond_t  cvts0;

void * callable_run (void * arg);

executor_t * executor_init (int core_pool_size,
        int max_pool_size,
        long keep_alive_time,
        int callable_array_size) {
    executor_t * executor;
    executor = (executor_t *) malloc (sizeof(executor_t));

    executor->keep_alive_time = keep_alive_time;
    executor->thread_pool = thread_pool_init (core_pool_size, max_pool_size);
    executor->futures = protected_buffer_init (0, callable_array_size);

    return executor;
}

future_t * submit_callable (executor_t * executor, callable_t * callable) {
    future_t * future = (future_t *) malloc (sizeof(future_t));

    callable->executor = executor;
    future->callable  = callable;
    future->completed = 0;
    pthread_mutex_init(&future->mutex, NULL);
    pthread_cond_init(&future->var, NULL);

    if(executor->thread_pool->size < executor->thread_pool->core_pool_size)
        pool_thread_create(executor->thread_pool, callable_run, future, 0);
    else
        protected_buffer_put(executor->futures, future);

    return future;
}

// Get result from callable execution. Block if not available.
void * get_callable_result (future_t * future) {
    void * result;

    // Protect from concurrent access
    pthread_mutex_lock(&future->mutex);

    // Wait for the result
    while(future->completed == 0)
        pthread_cond_wait(&future->var, &future->mutex);

    result = (void *) future->result;
    // Do not bother to deallocate future

    pthread_mutex_unlock(&future->mutex);

    return result;
}

void * callable_run (void * arg) {
    future_t           * future = (future_t *) arg;
    executor_t         * executor = (executor_t *) future->callable->executor;
    struct timespec      ts_deadline;
    struct timeval       tv_deadline; 

    gettimeofday (&tv_deadline, NULL);
    TIMEVAL_TO_TIMESPEC (&tv_deadline, &ts_deadline);

    while (1) {
        while (1) {
            // Protect from concurrent access
            pthread_mutex_lock(&future->mutex);

            future->result = future->callable->run (future->callable->params);

            // When the callable is not periodic, leave first inner loop
            if (future->callable->period == 0) {
                future->completed = 1;
                pthread_cond_broadcast(&future->var);
                pthread_mutex_unlock(&future->mutex);
                break;
            }
            pthread_mutex_unlock(&future->mutex);

        }

        if (executor->keep_alive_time != 0) {
            future = NULL;
            // If the executor is configured to release threads when they
            // are idle for keep_alive_time milliseconds, try to get a new
            // callable / future for at most keep_alive_time milliseconds.
            if (future == NULL) break;

        } else {
            // If the executor does not realease inactive thread, just wait
            // and block for the next available callable / future.
            future = (future_t *) protected_buffer_get(executor->futures);
        }
    }

    return NULL;
}

// Wait for pool threads to be completed
void executor_shutdown (executor_t * executor) {
    wait_thread_pool_empty(executor->thread_pool);
}

