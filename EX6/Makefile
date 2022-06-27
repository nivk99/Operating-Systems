#!make -f
#  *  @file makefile
#  *  AUTHORS: Niv Kotek - 208236315


CXX=clang++-9
CC=gcc
FLAGS=-fPIC -Wall -pthread  -lpthread
EXE= main1 main1_test guard singleton reactor selectclient_reactor_test client 
all: $(EXE)

librarymain1: libclassmain1.so
librarymain1_test: libclassmain1_test.so
libraryguard: libclassguard.so
librarysingleton: libclasssingleton.so
libraryreactor: libclassreactor.so
libraryselectclient_reactor_test: libclassselectclient_reactor_test.so
libraryclient: libclassclient.so

#Main1_test
Active_object: main1  
	./main1

main1: libclassmain1.so 
	$(CC) $(FLAGS) -o main1 ./libclassmain1.so

libclassmain1.so: main1.o
	$(CC) -shared -o libclassmain1.so main1.o

main1.o: main1.c main1.h
	$(CC) $(FLAGS) -c main1.c


#Main1_test
Main1_test: main1_test
	./main1_test

main1_test : libclassmain1_test.so 
	$(CC) $(FLAGS) -o main1_test ./libclassmain1_test.so 

libclassmain1_test.so: main1_test.o 
	$(CC) -shared -o libclassmain1_test.so main1_test.o 

main1_test.o: main1_test.c 
	$(CC) $(FLAGS) -c main1_test.c



#Guard
Guard: guard 
	./guard

guard: libclassguard.so
	$(CXX) $(FLAGS) -o guard ./libclassguard.so

libclassguard.so: guard.o 
	$(CXX) -shared -o libclassguard.so guard.o 

guard.o: guard.cpp guard.hpp 
	$(CXX) $(FLAGS) -c guard.cpp



#Singleton
Singleton: singleton
	./singleton

singleton: libclasssingleton.so
	$(CXX) $(FLAGS) -o singleton ./libclasssingleton.so

libclasssingleton.so: singleton.o 
	$(CXX) -shared -o libclasssingleton.so singleton.o 

singleton.o: singleton.cpp  
	$(CXX) $(FLAGS) -c singleton.cpp


#Reactor
Reactor: reactor
	./reactor

reactor: libclassreactor.so
	$(CXX) $(FLAGS) -o reactor ./libclassreactor.so 

libclassreactor.so: reactor.o 
	$(CXX) -shared -o libclassreactor.so reactor.o 

reactor.o: reactor.cpp reactor.hpp 
	$(CXX) $(FLAGS) -c reactor.cpp


#Reactor_test
Reactor_test: selectclient_reactor_test
	./selectclient_reactor_test

selectclient_reactor_test: libclassselectclient_reactor_test.so
	$(CXX) $(FLAGS) -o selectclient_reactor_test ./libclassselectclient_reactor_test.so

libclassselectclient_reactor_test.so: selectclient_reactor_test.o
	$(CXX) -shared -o libclassselectclient_reactor_test.so selectclient_reactor_test.o

selectclient_reactor_test.o: selectclient_reactor_test.cpp 
	$(CXX) $(FLAGS) -c selectclient_reactor_test.cpp


#Client
Client: client
	./client

client: libclassclient.so
	$(CC) -o client ./libclassclient.so

libclassclient.so: client.o
	$(CC) -shared -o libclassclient.so client.o

client.o: client.c
	$(CC)  -c client.c

#.PHONY &  clean
.PHONY: clean all Active_object Guard Singleton Reactor Reactor_test Main1_test
clean:
	rm -f $(EXE) tempCodeRunnerFile *.o *.a *.so 