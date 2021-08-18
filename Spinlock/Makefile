
all: test-lock

clean:
	rm -f test-lock lock.o liblock.so

lock: lock.c lock.h
	gcc -g -c -Wall -Werror -fpic lock.c
	gcc -g -shared -o liblock.so lock.o
	chmod +x liblock.so lock.o

test-lock: lock	test-lock.c
	gcc -L. -o test-lock test-lock.c -Wall -pthread -g -llock
