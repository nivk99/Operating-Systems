
/**
 * 
 *  @file heap.hpp
 *  AUTHORS: Niv Kotek - 208236315
 */
#pragma once
#include <unistd.h>
# include <string.h>
# define ExtraBytes 8
 void * my_malloc(unsigned int  byte_num);
 void *my_calloc(size_t nmemb, size_t size);
 void *my_realloc(void *ptr, size_t size);
 void my_free(void *ptr);
 void* my_memcpy(void* dest,  void* src, size_t num_bytes);