/**
 * @file selectclient_reactor_test.cpp
 * AUTHORS: Niv Kotek - 208236315
 * 
 * This is class test 
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
#include <pthread.h>

#define MAX 1024
#define PORT 8584
void sys(char *str);
void tcp_port();
void client1();
void client2();
void client3();
int main(int argc, char const *argv[])
{
	tcp_port();
	client3();
	sys("rm selectserver_test");
	return 0;
}


void* read_from(void* socket)
{
	char buff[MAX];
	int sock = *(int*)socket;
	while (true)
	{
		bzero(buff, sizeof(buff));
   		read(sock, buff, sizeof(buff));
		printf("OUTPUT --: %s\n", buff);
		sleep(1);
	}
	

}



void sys(char *str)
{
    char *command=(char*)calloc(strlen(str),sizeof(char));
    if(!command)
    {
        perror("file calloc");
        exit(1);
    }

   strcpy( command, str );
   system(command);
   free(command);

}



void tcp_port()
{
    pid_t fork1,fork2, fork3;
    fork1=fork();
    if (fork1<0)
    {
        perror("cannot fork");
        exit(EXIT_FAILURE);
    }
    if (fork1==0)
    {
        sys("clang++-9 -Wall -pthread  -lpthread  reactor.cpp -o selectserver_test");
        sys("./selectserver_test");
        exit(EXIT_SUCCESS);
    }
    fork2=fork();
    if (fork2<0)
    {
        perror("cannot fork");
        exit(EXIT_FAILURE);
    }
    if (fork2==0)
    {
		sleep(4);
        client1();
		 exit(EXIT_SUCCESS);
    }
	fork3=fork();
	if(fork3==0)
	{
		sleep(4);
        client2();
	 exit(EXIT_SUCCESS);
	}
    else 
    {
		wait(NULL);
		wait(NULL);


    }



}

void client1()
{
	char buff[MAX];
	struct sockaddr_in servaddr;

	// socket create and verification
	int sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	//printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =INADDR_ANY;
     //inet_addr(SERVER_IP_ADDRESS);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockClient, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}

	pthread_t _read;
	pthread_create(&_read,NULL,read_from,&sockClient);
	// exit(2);
	char ch1[]="from niv ::Hello, my name is Niv";
	sleep(1);
    send(sockClient, ch1, strlen(ch1), 0);
	sleep(1);

    char ch2[]="from niv::What's going on?";
	send(sockClient, ch2, strlen(ch2), 0);
	sleep(1);

	char ch3[]="from niv::Ok thank you";
	send(sockClient, ch3, strlen(ch3), 0);
	sleep(1);

    char ch4[]="from niv::what day is it today";
    	send(sockClient, ch4, strlen(ch4), 0);
	sleep(1);

    char ch6[]="from niv::higklmnop";
	send(sockClient, ch6, strlen(ch6), 0);
	sleep(1);

	 char ch7[]="from niv::what is the time now";
  	send(sockClient, ch7, strlen(ch7), 0);
	sleep(1);

    char ch8[]="exit";
	pthread_cancel(_read);
	send(sockClient, ch8, strlen(ch8), 0);
	sleep(1);
    printf("Client Exit...\n");

	close(sockClient);

}




void client2()
{
	char buff[MAX];
	struct sockaddr_in servaddr;

	// socket create and verification
	int sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	//printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =INADDR_ANY;
     //inet_addr(SERVER_IP_ADDRESS);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockClient, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	pthread_t _read;
	pthread_create(&_read,NULL,read_from,&sockClient);
	//exit(2);
	char ch1[]="from omer::Hello, my name is omer";
    send(sockClient, ch1, strlen(ch1), 0);
	sleep(1);

    char ch2[]="from omer::What's going on?";
	send(sockClient, ch2, strlen(ch2), 0);
	sleep(1);

	char ch3[]="from omer::Ok thank you";
	send(sockClient, ch3, strlen(ch3), 0);
	sleep(1);

    char ch4[]="from omer::what day is it today";
    	send(sockClient, ch4, strlen(ch4), 0);
	sleep(1);

    char ch6[]="from omer::higklmnop";
	send(sockClient, ch6, strlen(ch6), 0);
	sleep(1);

	 char ch7[]="from omer::what is the time now";
  	send(sockClient, ch7, strlen(ch7), 0);
	sleep(1);

    char ch8[]="exit";
	pthread_cancel(_read);
	send(sockClient, ch8, strlen(ch8), 0);
	sleep(1);
    printf("Client Exit...\n");

	close(sockClient);

}

void client3()
{
	char buff[MAX];
	struct sockaddr_in servaddr;

	// socket create and verification
	int sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	//printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =INADDR_ANY;
     //inet_addr(SERVER_IP_ADDRESS);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockClient, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
    char ch8[]="EXIT TEST";
	send(sockClient, ch8, strlen(ch8), 0);
	sleep(1);
    printf("Client Exit...\n");
	close(sockClient);

}