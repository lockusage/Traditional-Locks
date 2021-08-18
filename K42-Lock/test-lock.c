#include "k42_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
k42_lock *lock;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
    	acquire(lock);
    	counter++;
    	release(lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    lock = (k42_lock *)malloc(sizeof(k42_lock));
    lock->queue.next = NULL;
    lock->queue.tail = NULL;
    if (argc != 2) { 
	fprintf(stderr, "usage: test-lock <loops>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

