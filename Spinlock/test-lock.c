#include "lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
pthread_spinlock_t lock;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
    	pthread_spin_lock(&lock);
//	printf("Acq TID: %ld\n", pthread_self());
	counter++;
    	pthread_spin_unlock(&lock);
//   	printf("Release TID: %ld\n", pthread_self());
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { 
	fprintf(stderr, "usage: test-lock <loops>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    pthread_spin_init(&lock);
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

