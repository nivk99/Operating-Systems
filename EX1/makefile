FLAGS=-Wall -g
CC=gcc
all: main
	./$^
	
main: main.o
	$(CC) $(FLAGS) -o main main.o
main.o: main.c main.h
	$(CC) $(FLAGS) -c main.c
.PHONY: clean all 
clean:
	rm -f server main tempCodeRunnerFile *.o *.a
