#include <stdio.h>
#include <stdlib.h>
//#include "common.h"
#include "common_threads.h"
#include "mutex.h"

volatile int counter = 0; 
int loops;
//pthread_mutex_t m;
mutex m;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
    	mutex_lock(&m);
	counter++;
    	mutex_unlock(&m);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { 
	fprintf(stderr, "usage: threads <loops>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    mutex_init(&m);
    Pthread_create(&p1, NULL, worker, NULL); 
    Pthread_create(&p2, NULL, worker, NULL);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

