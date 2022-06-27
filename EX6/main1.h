/**
 * @file main1.h
 * AUTHORS: Niv Kotek - 208236315
 * 
 */



//Compile: cc -Wall -lpthread
//https://gist.github.com/rdleon/d569a219c6144c4dfc04366fd6298554
//https://beej.us/guide/bgnet/html/#a-simple-stream-server

#pragma once
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h> // for sleep() 
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h> 
#include <string.h>
#define ExtraBytes 16


//____________________________________Q.1________________________________________________
typedef struct node {
	void *value;
	struct node *next;
} Node,*Pnode;

typedef struct queue {
	int size;
	Pnode head;
	Pnode tail;
}Queue,*Pqueue;

Pqueue createQ();
void destoryQ(Pqueue que);
void enQ(Pqueue que,void *value);
void* deQ(Pqueue que);

//____________________________________Q.2________________________________________________
typedef struct _AO
{
	Pqueue que;
	void (*Function_before)(char *);
	void (*Function_after)(char *);
}AO,*PAO;

void activ_object(Pqueue que,void (*Function_before)(char *),void (*Function_after)(char *));
void* newAO(void* val);
void destoryAO(Pqueue que,int id);
void start_queue_ao();

void before_activ_object_1(char * str);
void after_activ_object_1(char * str);

void before_activ_object_2(char * str);
void after_activ_object_2(char * str);

void before_activ_object_3(char * str);
void after_activ_object_3(char * str);

//______________________________________________server_______________________________

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void* send_to(void *) ;
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
void* exit_to(void* sockfd1);

