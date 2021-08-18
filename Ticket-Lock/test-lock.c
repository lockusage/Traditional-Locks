#include "ticket_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
ticket_lock lock;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
    	ticket_lock_acquire(&lock);
    	counter++;
    	ticket_lock_release(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { 
	fprintf(stderr, "usage: test-lock <loops>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2, p3, p4;
    ticket_lock_init(&lock);
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_create(&p3, NULL, worker, NULL);
    pthread_create(&p4, NULL, worker, NULL);    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

