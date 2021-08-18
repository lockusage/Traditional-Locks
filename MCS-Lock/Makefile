
all: test-lock

clean:
	rm -f test-lock mcs_lock.o libmcs_lock.so

lock: mcs_lock.c mcs_lock.h
	gcc -g -c -Wall -Werror -fpic mcs_lock.c
	gcc -g -shared -o libmcs_lock.so mcs_lock.o
	chmod +x libmcs_lock.so mcs_lock.o

test-lock: lock	test-lock.c
	gcc -L. -o test-lock test-lock.c -Wall -pthread -g -lmcs_lock
