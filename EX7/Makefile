#!make -f
#  *  @file makefile
#  *  AUTHORS: Niv Kotek - 208236315


# ************     To run, type make all and then type make Test     ********************8

CC=gcc
FLAGS=-fPIC -Wall
EXE= mystdio
all: $(EXE)

libmyfs: libmyfs.so
libmylibc: libmylibc.so


#myfs
myfs: libmyfs.so 
	$(CC) $(FLAGS) -o myfs ./libmyfs.so

libmyfs.so: myfs.o
	$(CC) -shared -o libmyfs.so myfs.o

myfs.o: myfs.c myfs.h
	$(CC) $(FLAGS) -c myfs.c

#mystdio
mystdio: libmylibc.so 
	$(CC) $(FLAGS) -o mystdio ./libmylibc.so

libmylibc.so: myfs.o mystdio.o test.o
	$(CC) -shared -o libmylibc.so mystdio.o myfs.o test.o

mystdio.o: mystdio.c mystdio.h
	$(CC) $(FLAGS) -c mystdio.c


Test: test
	./test

#test
test: test.c myfs.c mystdio.c myfs.h mystdio.h
	$(CC) -o test test.c myfs.c mystdio.c
test.o: test.c
	$(CC) -c test.c


#.PHONY &  clean
.PHONY: clean all myfs mystdio Test
clean:
	rm -f mystdio test myfs tempCodeRunnerFile *.o *.a *.so 