#include <stdio.h>
#include <pthread.h>

void *run(void *parameter) {
    printf("Hello Word!\n");
    return NULL;
}

int main() {
    pthread_t th1, th2;
    pthread_create(&th1, NULL, run, NULL);
    pthread_create(&th2, NULL, run, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}

