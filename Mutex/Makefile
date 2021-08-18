
all: test-lock

clean:
	rm -f test-lock

test-lock: test-lock.c common.h common_threads.h
	gcc -o test-lock test-lock.c -Wall -pthread
