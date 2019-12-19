/*hellothreads.c*/

#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

void *hello_thread (void *arg)
{
    printf("Thread %d: hello, world!\n", arg);
    return arg;
}

main()
{
    pthread_t tid[NUM_THREADS];
    int i, status;

    /*make thread*/
    for (i = 0; i < NUM_THREADS; i++) {
        status = pthread_create (&tid[i],NULL, hello_thread, (void *) i);
        if (status != 0) {
            fprintf(stderr, "Create thread %d: %d", i, status);
            exit(0);
        }
    }
    pthread_exit(NULL);
}