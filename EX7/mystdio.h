
/**
 * @file mystdio.h
 * AUTHORS: Niv Kotek - 208236315
 * 
 * https://www.cprogramming.com/tutorial/c/lesson17.html
 */

#pragma once
#include "myfs.h"
#include <stdarg.h>
typedef struct _myFILE
{
    char *name;
    int fd;
    char* data;
    int fseek;
    char mode[2];
    int size;
    bool isOpen;

}myFILE,*PmyFILE;



myFILE *myfopen(const char *pathname, const char *mode);

int myfclose(myFILE *stream);

size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream);

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream);

int myfseek(myFILE *stream, long offset, int whence);

int myfscanf(myFILE *stream, const char *format, ...);

int myfprintf(myFILE *stream, const char *format, ...);


////Help functions
void myMemCpy_fread(void *dest, size_t n, myFILE *stream);

void myMemCpy_write(void *dest, void *src, size_t n);