/**
 * 
 *  @file my_stach.cpp
 *  AUTHORS: Niv Kotek - 208236315
 */

#include "my_stack.hpp"
#include <pthread.h>
pthread_mutex_t mt=PTHREAD_MUTEX_INITIALIZER;

PNodeData* __end_stack(PStack stack)
{
	PNodeData *ptr=&(stack->List_Node);
	for (size_t i = 0; i < stack->size_Node-1; i++)
	{
		ptr=&((*ptr)->Next_Node);
	}
	return ptr;
	
}


PNodeData __new_node(char *text)
{
	
	PNodeData new_node=(PNodeData)my_malloc(sizeof(NodeData));
	if (!new_node)
	{
		perror("!error malloc!\n");
		exit(1);
	}
	new_node->Next_Node=NULL;
	memcpy(new_node->text,text,strlen(text));
	new_node->text[strlen(text)]='\0';
	return new_node;
}



int isempty(PStack stack) 
{
	
	return stack->size_Node == 0;
	

}

//PUSH will push <text> into the stack.
void PUSH(PStack stack,char *text)
{
	pthread_mutex_lock(&mt);
	
	if(isempty(stack))
	{
		stack->List_Node=__new_node(text);
	}

	else
	{
		(*__end_stack(stack))->Next_Node=__new_node(text);

	}

	stack->size_Node++;
	pthread_mutex_unlock(&mt);
}

//TOP will display (output) the stack top.
char* TOP(PStack stack)
{
	pthread_mutex_lock(&mt);
	if (!isempty(stack))
	{
		pthread_mutex_unlock(&mt);
		return (*__end_stack(stack))->text;

	}
	else
	{

		 printf("Could not retrieve data, Stack is empty.\n");
		 pthread_mutex_unlock(&mt);
		 return NULL;
	}
	pthread_mutex_unlock(&mt);
}

// POP will pop text from the stack
void POP(PStack stack)
 {
	 pthread_mutex_lock(&mt);
   if(!isempty(stack)) 
   {
	   __free(stack);  
   } 
   else 
   {
      printf("Could not retrieve data, Stack is empty.\n");
   }
   pthread_mutex_unlock(&mt);
}

 void __free(PStack stack)
 {
	 if(stack->size_Node==1)
	 {
		  stack->size_Node--;
		 return;
	 }
	  my_free((*__end_stack(stack)));
	   
	 stack->size_Node--;

	 if(!isempty(stack))
	 {
	 	(*__end_stack(stack))->Next_Node=NULL;
	 }
	
 }

 void freeAll(PStack stack)
 {
	 if (stack->size_Node==1)
	 {
		 return;
	 }
	 while (!isempty(stack))
	 {
		__free(stack);
		if (stack->size_Node==1)
		{
			break;
		}
	 }
	 
 }


//insert to the tail
void ENQUEUE(PStack *stack,char *text)
{
	pthread_mutex_lock(&mt);
	if(!*stack)
	{
		pthread_mutex_unlock(&mt);
		return;

	}

	if(isempty(*(stack)))
	{
		(*stack)->List_Node=__new_node(text);
	}

	else
	{
		PNodeData head=(*stack)->List_Node;
		(*stack)->List_Node=__new_node(text);
		(*stack)->List_Node->Next_Node=head;

	}
	(*stack)->size_Node++;
	pthread_mutex_unlock(&mt);

}

//remove the tail
 void DEQUEUE(PStack *stack)
 {
	 pthread_mutex_lock(&mt);
	 if(!*stack)
	 {
		 pthread_mutex_unlock(&mt);
		 return;
	 }

	 if(!isempty(*stack)) 
	 {
		 if((*stack)->size_Node==1)
		 {
			 (*stack)->size_Node--;
			 return;
		 }
		PNodeData ptr=(*stack)->List_Node;
		(*stack)->List_Node=ptr->Next_Node;
		my_free(ptr);
		(*stack)->size_Node--;
	 }

	 else
	 {
		 printf("Could not retrieve data, Stack is empty.\n");
	 }
	 pthread_mutex_unlock(&mt);

 }

//will display (output) the stack tail
 char* PEEK(PStack stack)
 {
	 pthread_mutex_lock(&mt);
	 if (!isempty(stack))
	{
		pthread_mutex_unlock(&mt);
		return stack->List_Node->text;

	}
	else
	{
		 printf("Could not retrieve data, Stack is empty.\n");
		 pthread_mutex_unlock(&mt);
		 return NULL;
	}
	pthread_mutex_unlock(&mt);

 }

 
 

