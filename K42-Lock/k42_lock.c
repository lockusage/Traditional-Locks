#include <stdlib.h>
#include <stdio.h>
//#include "common.h"
#include <pthread.h>
#include "k42_lock.h"

struct lnode *waiting=1;

void acquire (k42_lock *lock){
    struct lnode *prev;
    struct lnode *desired;
    struct lnode *new;
    struct lnode *succ;
    struct lnode *temp;
    struct lnode *oldnew;

    while(1){
        desired = NULL;
        prev = lock->queue.tail;
        if (prev == NULL){
            temp = &lock->queue;
            if (__atomic_compare_exchange(&lock->queue.tail, &desired, &temp, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)){
                // Lock is available.
                return;
            }
        }
        else{
            // Lock is not available; add thread to queue
            // Create a new Node 
            new = (struct lnode*)malloc(sizeof(struct lnode));
            new->next = NULL;
            new->tail = waiting;
            if(__atomic_compare_exchange(&lock->queue.tail, &prev, &new, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)){
                // Added to queue's tail successfully
                prev->next = new;
                while (new->tail == waiting); //spin
                // got the lock
                succ = new->next;
                if (succ==NULL){
                    lock->queue.next = NULL;
                    temp = &lock->queue;
                    // try to make lock point at itself
                    oldnew = new;
                    // TODO
                    if(!__atomic_compare_exchange(&lock->queue.tail, &oldnew, &temp, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)){
                        // Somebody got in; get him to front of the queue
                        do{
                            succ = new->next;
                        }while(succ == NULL);
                        lock->queue.next = succ;
                    }
                    return;
                }
                else{
                    // Move the next waiting thread to front of the queue
                    lock->queue.next = succ;
                    return;
                }

            }

        }
    }
}
void release (k42_lock *lock){
    struct lnode *succ;
    struct lnode *desired = NULL;
    struct lnode *temp;
    succ = lock->queue.next;
    if (succ==NULL){
        temp = &lock->queue;
        if (__atomic_compare_exchange(&lock->queue.tail, &temp, &desired, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)){
            return;
        }
        do{
            succ = lock->queue.next;
        }while(succ == NULL);
    }
    succ->tail = NULL;
    return;
}
