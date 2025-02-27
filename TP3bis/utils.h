#ifndef UTILS_H
#define UTILS_H
#include <sys/time.h>
#ifndef TIMEVAL_TO_TIMESPEC
#define TIMEVAL_TO_TIMESPEC(tv, ts) {                                   \
        (ts)->tv_sec = (tv)->tv_sec;                                    \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;                           \
}
#endif
#ifndef TIMESPEC_TO_TIMEVAL
#define TIMESPEC_TO_TIMEVAL(tv, ts) {                                   \
        (tv)->tv_sec = (ts)->tv_sec;                                    \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;                           \
}
#endif

// Initialize the data structure used in this unti
void init_utils();

// Add msec milliseconds to a timespec (seconds, nanoseconds)
void add_millis_to_timespec (struct timespec * ts, long msec);

// Wait until deadline
void delay_until(struct timespec * deadline);

// Compute time elapsed from the start time
long elapsed_time();

// Return the start time
struct timespec get_start_time();

// Store current time as the start time
void set_start_time();

// Read long in file f and store it in l. If there is an error,
// provide filename and line number (file:line).
int get_long (FILE * f, long * l, char * file, int line);

// Read string in file f and store it in s. If there is an error,
// provide filename and line number (file:line).
int get_string (FILE * f, char * s, char * file, int line);
#endif
