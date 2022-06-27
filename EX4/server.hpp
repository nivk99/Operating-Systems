/**
 * 
 *  @file server.hpp
 *  AUTHORS: Niv Kotek - 208236315
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
# include <stdbool.h>
# include "my_stack.hpp"
void* send_to(void *) ;
void* exit_to(void *);
void push_enqueve(char *buff,char ch);
void pop_dequeve(char ch);
void top_peek(int sock,char ch);