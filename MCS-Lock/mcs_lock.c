#include "mcs_lock.h"

void mcs_acquire (mcs_lock *lock, struct lnode *p) {
  struct lnode *prev;
  p->next = NULL; // Init the new node
  p->locked = 0;
  // Attach the thread to the end of the tail
  __atomic_exchange(&lock->tail, &p, &prev, __ATOMIC_SEQ_CST);

  if (prev != NULL) {
    prev->next = p;
    while ( !p->locked ); // Spin until its your turn
  }
}
void mcs_release(mcs_lock *lock, struct lnode *p) {
  struct lnode *next;
  struct lnode *desired;
  struct lnode *cmp_pointer; 
  // Get the next rightful owner
  __atomic_load(&p->next, &next, __ATOMIC_SEQ_CST);

  if (next == NULL) { // Set tail to NULL
    desired = NULL;
    cmp_pointer = p;
    if (__atomic_compare_exchange(&lock->tail, &cmp_pointer, &desired,
		1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
      return;
    }
    do { // Some other thread just arrived
      __atomic_load(&p->next, &next, __ATOMIC_SEQ_CST);
    } while (next == NULL);
  }
  next->locked = 1;
}