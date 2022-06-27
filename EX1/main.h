# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <dirent.h>
#include <time.h>
#include <sys/wait.h>  // for wait()
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 256
#define PORT 5567
int sockClient;

char* input();
void echo(char *str);
void dir();
void cd (char *str);
void sys(char *str);
void tcp_port();
void del(char *str);
void client();
char* pwd();
void file(char *src , char *dst);
void execArgs(char** parsed);