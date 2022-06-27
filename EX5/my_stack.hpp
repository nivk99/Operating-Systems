
/**
 * 
 *  @file my_stack.hpp
 *  AUTHORS: Niv Kotek - 208236315
 */
#pragma once
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
#include <unistd.h>
#include "heap.hpp"
# define _MAX_BUFF 1024

typedef struct NodeData
{
	char text[_MAX_BUFF];
	struct NodeData *Next_Node;
} NodeData ,*PNodeData;

typedef struct my_stack
{
	size_t size_Node;
    PNodeData List_Node;
}Stack,*PStack;


PNodeData* __end_stack(PStack stack);
PNodeData __new_node(char *text,int fd);

int isempty(PStack stack) ;

void PUSH(PStack stack,char *text,int fd);
void ENQUEUE(PStack *stack,char *text,int fd);

char* PEEK(PStack stack);
char* TOP(PStack stack);

 void DEQUEUE(PStack *stack);
void POP(PStack stack);

 void __free(PStack stack);
 void freeAll(PStack stack);
