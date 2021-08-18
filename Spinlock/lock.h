#ifndef __lock_h__
#define __lock_h__

typedef volatile int pthread_spinlock_t;

extern int pthread_spin_init(pthread_spinlock_t *lock);
extern int pthread_spin_lock(pthread_spinlock_t *lock);
extern int pthread_spin_unlock(pthread_spinlock_t *lock);

#endif