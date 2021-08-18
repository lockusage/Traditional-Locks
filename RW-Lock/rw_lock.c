#include "rw_lock.h"
int WA_FLAG = 1; int RC_INC = 2;

void rwlock_init(rwlock *lock)
{
    int init_value = 0; 
    __atomic_store(lock, &init_value,
	__ATOMIC_RELAXED); //Init lock
}

void rwlock_reader_acquire(rwlock *lock) {
  // Reader acquiring lock. Increment by 2.
  __atomic_add_fetch(lock, RC_INC,
	__ATOMIC_SEQ_CST);
  // Spin until writer is present.
  while((__atomic_load_n(lock,
	 __ATOMIC_SEQ_CST) & WA_FLAG) == 1);
}

void rwlock_reader_release(rwlock *lock) {
  // Reader releasing lock. Decrement by 2.
  __atomic_sub_fetch(lock, RC_INC,
	__ATOMIC_SEQ_CST);
}

void rwlock_writer_acquire(rwlock *lock) {
  int expected_writers = 0;
  // Spin until either readers or writers present.
  while(!(__atomic_compare_exchange_n(lock, 
	  &expected_writers, WA_FLAG, 1, 
	  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))) {
    expected_writers = 0;
  }
}

void rwlock_writer_release(rwlock *lock) {
  // Writer releasing lock. Decrement by 1.
  __atomic_sub_fetch(lock, WA_FLAG,
	__ATOMIC_SEQ_CST);
}
