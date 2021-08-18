#ifndef __rw_lock_h__
#define __rw_lock_h__
typedef volatile int rwlock;

extern void rwlock_init(rwlock *lock);
extern void rwlock_reader_acquire(rwlock *lock);
extern void rwlock_reader_release(rwlock *lock);
extern void rwlock_writer_acquire(rwlock *lock);
extern void rwlock_writer_release(rwlock *lock);

#endif