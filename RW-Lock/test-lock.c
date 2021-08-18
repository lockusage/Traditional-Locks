#include "rw_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
rwlock lock;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        if (i%100 == 0){
            rwlock_reader_acquire(&lock);
            //printf("Ack reader lock: %d\n", counter);
            rwlock_reader_release(&lock);
        }
    	rwlock_writer_acquire(&lock);
    	counter++;
    	rwlock_writer_release(&lock);
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
    rwlock_init(&lock);
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

