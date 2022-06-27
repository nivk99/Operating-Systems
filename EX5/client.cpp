//https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

/**
 *  To start, type  make client
 * 
 *  To exit the program you have to write EXIT.
 * 
 * 
 * 
 *  AUTHORS: Niv Kotek - 208236315
 * 
 */
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
#define MAX 1024
#define PORT  8584
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX];
    int n;
    while(true) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        send(sockfd, buff, sizeof(buff),0);
        if (strstr(buff,"TOP")||strstr(buff,"PEEK"))
        {
             bzero(buff, sizeof(buff));
            recv(sockfd, buff, sizeof(buff),0);
           printf("OUTPUT: %s\n", buff);
        }
        if ((strncmp(buff, "EXIT", 4)) == 0) {
            sleep(2);
            printf("Client Exit...\n");
            break;
        }
    }
}
   
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =INADDR_ANY;
     //inet_addr(SERVER_IP_ADDRESS);
	servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    // function for chat
    func(sockfd);
   
    // close the socket
    close(sockfd);
}