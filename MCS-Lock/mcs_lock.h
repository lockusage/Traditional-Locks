#ifndef __mcs_lock_h__
#define __mcs_lock_h__
#include <stdio.h>

struct lnode {
  struct lnode *next; // Use for queueing.
  int locked; // Local variable for spining
};

typedef struct lock {
  struct lnode *tail;
} mcs_lock;

void mcs_acquire (mcs_lock *lock, struct lnode *p);
void mcs_release(mcs_lock *lock, struct lnode *p);

#endif