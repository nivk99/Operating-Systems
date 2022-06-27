
CC=gcc
Wall=-Wall -pthread  -lpthread
all: server 
	./$^
server: server_threads.o
	$(CC) $(Wall) -o server server_threads.o
server_threads.o: server_threads.c
	$(CC) -c server_threads.c
.PHONY: clean all 
clean:
	rm -f  server tempCodeRunnerFile *.o *.a

