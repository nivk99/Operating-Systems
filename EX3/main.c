
/**
 * compile: gcc -Wall -pthread  -lpthread  server_threads.c -o server
 * server threads
 * 
 * To start, type make all
 *  @file server_threads.c
 *  AUTHORS: Niv Kotek - 208236315
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
# include <stdbool.h>
void* send_to(void *) ;



#define PORT "8995"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}









//-------------------------------------------------------

int main() { 
    

    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo *servinfo, *p,hints;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
       

	     
		// thread
			//thread
	    pthread_t thread_data ;
         int status ;
		srand((unsigned)time(NULL)) ;
		status = pthread_create(&thread_data, NULL, send_to, &new_fd) ;
		if (status != 0) {
			perror("pthread_create failed in main") ;
			exit(EXIT_FAILURE) ;
		}
        //close(new_fd);  // parent doesn't need this
    }

    return 0;
}


//-------------------------------------------------------// thread
void* send_to(void * socket)
{
	   printf("process = %d thread= %u \n", getpid(),(unsigned int) pthread_self());
	   int sock = *(int*)socket;
	   int read_size=send(sock, "Hello, world!", 13, 0);

	if(read_size == 0)
    {
        puts("Client disconnected");
    }
    else if(read_size == -1)
    {
      perror("send");
    }
	fflush(stdout);
	close(sock);
	pthread_exit(NULL) ;
}

//------------------------------------------------------
