
/**
 * 
 *  @file heap.cpp
 *  AUTHORS: Niv Kotek - 208236315
 */
# include<stdio.h>
# include "heap.hpp"
#include <pthread.h>
//#define DEBUG
pthread_mutex_t mtx1=PTHREAD_MUTEX_INITIALIZER;
void * my_malloc(unsigned int  byte_num) 
{
	if( byte_num ==0)
	{
		return NULL;
	}
    pthread_mutex_lock(&mtx1);
	void * ret = sbrk(0);	
	void * siz=sbrk(ExtraBytes+ byte_num);
	if(siz == (void *)-1)
	{
		pthread_mutex_unlock(&mtx1);
  		return NULL;
 	}
	 int* size = (int*)ret;
	*size = byte_num;	
	pthread_mutex_unlock(&mtx1);
	#if defined(DEBUG)
	printf("malloc: %d\n",byte_num+ExtraBytes);
	#endif
	return (void*)((int*)ret+ExtraBytes);
}


void *my_calloc(size_t nmemb, size_t size)
{
    void *ptr = my_malloc(size * nmemb);

    if (!ptr)
        return NULL;
    return memset(ptr, 0, (size *nmemb));
}


void *my_realloc(void *ptr, size_t size)
{
	void *new_ptr;
	if(!ptr)
	{
		return my_malloc(size); 
	}
	if(!size)
	{
		my_free(ptr);
        return NULL;
	}
	new_ptr=my_malloc(size);
	if(!new_ptr)
	{
		return NULL;
	}
	my_memcpy(new_ptr,ptr,size);
	my_free(ptr);
	return new_ptr;

}


void my_free(void *ptr)
{
	pthread_mutex_lock(&mtx1);
	if (!ptr)
	{
		pthread_mutex_unlock(&mtx1);
		return;

	}
	 int *size = ((int*)ptr - ExtraBytes);
	sbrk(-(*size+ExtraBytes));
	#if defined(DEBUG)
	printf("free: %d\n",*size+ExtraBytes);
	#endif
	pthread_mutex_unlock(&mtx1);
	
}


void* my_memcpy(void* dest,  void* src, size_t num_bytes)
{
  char *destPtr =(char*) dest;
  char  *srcPtr = (char*)src;

  while (num_bytes-- > 0) 
  {
    *destPtr++ = *srcPtr++;
  }
    
  return (void*)dest;
}