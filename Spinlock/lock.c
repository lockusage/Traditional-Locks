#include "lock.h"
#include <stdio.h>

int pthread_spin_init(pthread_spinlock_t *lock) {
  // Initialize the lock to 0 (free).
  int init_value = 0;
  __atomic_store(lock, &init_value, __ATOMIC_RELAXED);
  return 0;
}

int pthread_spin_lock(pthread_spinlock_t *lock) {
  int expected = 0;
  if (__atomic_compare_exchange_n(lock, &expected,
	1, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
    // Acquired the lock. Return.
    return 0;
  }
  // Spin until the lock is not free.
  do {
    // First check if the lock is free.
    do {
      expected =  __atomic_load_n(lock, __ATOMIC_RELAXED);
    } while(expected != 0);
  } while(!__atomic_compare_exchange_n(lock, &expected,
	1, 1, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED));
  // Finally, acquired the lock.
  return 0;
}

int pthread_spin_unlock(pthread_spinlock_t *lock) {
  int unlock = 0;
  // Mark the lock free.
  __atomic_store(lock, &unlock, __ATOMIC_RELEASE);
  return 0;
}

