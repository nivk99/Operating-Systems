
/**
 * @file guard.cpp
 * AUTHORS: Niv Kotek - 208236315
 * 
 */


/**
 * char * strtok(char str[], const char *delims);
 * char *strtok_r(char *str, const char *delim, char **saveptr);
 * 
 * Just like strtok() function in C, strtok_r() does the same task of parsing a string into a sequence of tokens.
 *  strtok_r() is a reentrant version of strtok()
 * Thus by guard strtok not can be protected by a global pointer
 */

#include "guard.hpp"

int c;
void *print_message_function( void *ptr )
{
    Guard g(&lock2);
    for (int i = 0; i < 10; i++)
    {
        c++;
        printf("%s : %d\n",(char*) ptr,c);
    } 
	return NULL;
}


//_______________________test_guard_____________________________________

int main(int argc, char const *argv[])
{
     pthread_t thread1, thread2;
     char * message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
     
     printf("%d\n",iret2);

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 

     printf("Thread 1 returns: %lu\n",thread1);
     printf("Thread 2 returns: %lu\n",thread2);
     exit(0);
}