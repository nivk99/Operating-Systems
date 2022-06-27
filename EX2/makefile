
CC=gcc
all: signal 
	./$^
signal: signal.o
	$(CC)  -o signal signal.o
signal.o: signal.c signal.h
	$(CC)  -c signal.c
.PHONY: clean all 
clean:
	rm -f  signal tempCodeRunnerFile *.o *.a

