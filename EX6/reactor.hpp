/**
 * @file reactor.hpp
 * AUTHORS: Niv Kotek - 208236315
 * 
 */



//#pragma once
//https://beej.us/guide/bgnet/html/#a-simple-stream-server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <unordered_map>
#include <pthread.h>
#include <functional>
#define PORT "8584"


class reactor
{
private:
fd_set _file_descriptor_list;
std::unordered_map<int,std::function<void(reactor)>> _functions_list;
pthread_t _thread;
pthread_t _run;
int fdmax;
	reactor();
public:
	~reactor()=default;

    int get_fdmax();
    pthread_t get_thread();
    pthread_t get_thread_run();
    std::unordered_map<int,std::function<void(reactor)>> get_functions_list();
    fd_set get_file_descriptor_list();

	static void * selectserver(void *ptr);
    static void* exit_to(void* ptr);
    static reactor newReactor();
    static void InstallHandler(int fil,void (*functptr)(reactor),reactor Reactor );
    static void RemoveHandler(reactor Reactor);
    static void *get_in_addr(struct sockaddr *sa);

};



