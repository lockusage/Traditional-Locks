#include "mcs_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
mcs_lock lock;

void *worker(void *arg) {
    int i;
    struct lnode *node; 
    for (i = 0; i < loops; i++) {
        node = (struct lnode *)malloc(sizeof(struct lnode));
    	mcs_acquire(&lock, node);
    	counter++;
    	mcs_release(&lock, node);
        free(node);
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
    lock.tail = NULL;
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

