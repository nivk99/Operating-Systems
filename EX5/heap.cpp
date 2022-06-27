
/**
 * 
 *  @file heap.cpp
 *  AUTHORS: Niv Kotek - 208236315
 */
# include<stdio.h>
# include "heap.hpp"
#include <stdlib.h>
void * my_malloc(unsigned int  byte_num,int fd) 
{

	if( byte_num ==0)
	{
		return NULL;
	}
void *new_ptr;
	
new_ptr=mmap ( NULL, byte_num+ExtraBytes,
   PROT_READ | PROT_WRITE,
     MAP_SHARED | MAP_ANONYMOUS,
     -1,0);   

if(new_ptr == MAP_FAILED)
{
    printf("Mapping Failed\n");
    return NULL;
}
int* size = (int*)new_ptr;
*size = byte_num;
return (void*)((int*)new_ptr+ExtraBytes);
}


void *my_calloc(size_t nmemb, size_t size,int fd)
{
    void *ptr = my_malloc(size * nmemb,fd);

    if (!ptr)
        return NULL;
    return memset(ptr, 0, (size *nmemb));
}


void *my_realloc(void *ptr, size_t size,int fd)
{
	void *new_ptr;
	if(!ptr)
	{
		return my_malloc(size,fd); 
	}
	if(!size)
	{
		my_free(ptr);
        return NULL;
	}
	new_ptr=my_malloc(size,fd);
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
	if (!ptr)
	{
		return;
	}
int *size = ((int*)ptr - ExtraBytes);
int err = munmap(ptr,*size+ExtraBytes);
if(err  == -1){
    return ;
}
	
	
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