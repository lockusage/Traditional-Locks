#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <stdio.h>
#include <stdlib.h>

int sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void
*addr2, int val3)
{
	return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))
#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N))
#define atomic_inc(P) __sync_add_and_fetch((P), 1)
#define atomic_dec(P) __sync_add_and_fetch((P), -1) 
#define atomic_add(P, V) __sync_add_and_fetch((P), (V))
#define atomic_set_bit(P, V) __sync_or_and_fetch((P), 1<<(V))
#define atomic_clear_bit(P, V) __sync_and_and_fetch((P), ~(1<<(V)))

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

/* Atomic exchange (of various sizes) */
static inline void *xchg_64(void *ptr, void *x)
{
	__asm__ __volatile__("xchgq %0,%1"
				:"=r" ((unsigned long long) x)
				:"m" (*(volatile long long *)ptr), "0" ((unsigned long long) x)
				:"memory");

	return x;
}

static inline unsigned xchg_32(void *ptr, unsigned x)
{
	__asm__ __volatile__("xchgl %0,%1"
				:"=r" ((unsigned) x)
				:"m" (*(volatile unsigned *)ptr), "0" (x)
				:"memory");

	return x;
}

static inline unsigned short xchg_16(void *ptr, unsigned short x)
{
	__asm__ __volatile__("xchgw %0,%1"
				:"=r" ((unsigned short) x)
				:"m" (*(volatile unsigned short *)ptr), "0" (x)
				:"memory");

	return x;
}

/* Test and set a bit */
static inline char atomic_bitsetandtest(void *ptr, int x)
{
	char out;
	__asm__ __volatile__("lock; bts %2,%1\n"
						"sbb %0,%0\n"
				:"=r" (out), "=m" (*(volatile long long *)ptr)
				:"Ir" (x)
				:"memory");

	return out;
}

typedef int mutex;

#define MUTEX_INITIALIZER {0}

int mutex_init(mutex *m)
{
	*m = 0;
	return 0;
}

int mutex_destroy(mutex *m)
{
	/* Do nothing */
	(void) m;
	return 0;
}

int mutex_lock(mutex *m)
{
	int c = 0;
	__atomic_compare_exchange_n(m, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	if (!c) {
		return 0;
	} else if (c == 1) {
	/* The lock is now contended */
		c = __atomic_exchange_n(m, 2, __ATOMIC_SEQ_CST);
	}

	while (c) {
		/* Wait in the kernel */
		sys_futex(m, FUTEX_WAIT_PRIVATE, 2, NULL, NULL, 0);
		c = __atomic_exchange_n(m, 2, __ATOMIC_SEQ_CST);
	}

	return 0;
}

int mutex_unlock(mutex *m)
{
	int state = __atomic_exchange_n(m, 0, __ATOMIC_SEQ_CST);

	if (state > 1) {	
		/* We need to wake someone up */
		sys_futex(m, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
	}

	return 0;
}
