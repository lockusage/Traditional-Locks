
all: test-lock

clean:
	rm -f test-lock k42_lock.o libk42_lock.so

lock: k42_lock.c k42_lock.h
	gcc -g -c -Wall -Werror -fpic k42_lock.c
	gcc -g -shared -o libk42_lock.so k42_lock.o
	chmod +x libk42_lock.so k42_lock.o

test-lock: lock	test-lock.c
	gcc -L. -o test-lock test-lock.c -Wall -pthread -g -lk42_lock
