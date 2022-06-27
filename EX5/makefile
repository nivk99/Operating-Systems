#  *  @file makefile
#  *  AUTHORS: Niv Kotek - 208236315

CC=gcc
Wall=-Wall -pthread   -lpthread
all: server 
	./server
server: my_stack.o server.o heap.o 
	$(CC) $(Wall) -o server my_stack.o server.o heap.o

server.o: server.cpp server.hpp
	$(CC) -c server.cpp

client: clientRun
	./client

clientRun: client.cpp
	$(CC) -o client client.cpp

my_stack.o: my_stack.cpp my_stack.hpp
	$(CC) -c my_stack.cpp

heap.o: heap.cpp heap.hpp
	$(CC) -c heap.cpp


test: testRun
	./test

testRun: test.cpp
	$(CC) -o test test.cpp

.PHONY: clean all testRun clientRun 
clean:
	rm -f server client test tempCodeRunnerFile *.o *.a

