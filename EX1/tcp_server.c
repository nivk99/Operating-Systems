// /*
//  TCP/IP-server
// */
//https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
# include <stdbool.h>
#define MAX 256
#define PORT   5567 //The port that the server listens

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
	// infinite loop for chat
	
	while(true) {
		bzero(buff, MAX);

		// read the message from client 
		read(connfd, buff, sizeof(buff));
		// if msg contains "Exit" then server exit and chat ended.
		if (!strncmp( buff,"LOCAL", 5) ||strstr(buff,"LOCAL")) {
			//printf("Server Exit...\n");
			break;
		}

		// print buffer 
		printf("From client: %s\n", buff);
	}
}

int main(int argc, char const *argv[])
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	//printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
	
	return 0;
}
