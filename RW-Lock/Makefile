
all: test-lock

clean:
	rm -f test-lock rw_lock.o rw_liblock.so

lock: rw_lock.c rw_lock.h
	gcc -g -c -Wall -Werror -fpic rw_lock.c
	gcc -g -shared -o librw_lock.so rw_lock.o
	chmod +x librw_lock.so rw_lock.o

test-lock: lock	test-lock.c
	gcc -L. -o test-lock test-lock.c -Wall -pthread -g -lrw_lock
