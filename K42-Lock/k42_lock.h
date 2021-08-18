#ifndef __k42_lock_h__
#define __k42_lock_h__
#include <stdio.h>


struct lnode{
    // Store the next node address to signal.
	struct lnode *next;
  struct lnode *tail;
};

typedef struct lock{
    // Store the queue.
    struct lnode queue;
}k42_lock;

void acquire (k42_lock *lock);
void release (k42_lock *lock);

#endif