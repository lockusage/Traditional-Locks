
all: test-lock

clean:
	rm -f test-lock ticket_lock.o ticket_liblock.so

lock: ticket_lock.c ticket_lock.h
	gcc -g -c -Wall -Werror -fpic ticket_lock.c
	gcc -g -shared -o libticket_lock.so ticket_lock.o
	chmod +x libticket_lock.so ticket_lock.o

test-lock: lock	test-lock.c
	gcc -L. -o test-lock test-lock.c -Wall -pthread -g -lticket_lock
