
/**
 * compile: gcc -Wall -pthread  -lpthread  my_stach.cpp server.cpp heap.cpp -o server
 * 
 * To start, type make all
 * To exit the program you have to write yes.
 *  @file server.cpp
 *  AUTHORS: Niv Kotek - 208236315
 */
#include "server.hpp"
//pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t lock;
PStack stach;

int sockfd;

#define PORT "8589"  // the port users will be connecting to

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

if (pthread_rwlock_init(&lock, NULL) != 0)
 { 
 puts("pthread_rwlock_init(() failed\n") ; 
 exit(EXIT_FAILURE) ; 
 } 

    stach=(PStack)my_malloc(sizeof(Stack));
    if(!stach)
    {
        exit(1);
    }
    stach->size_Node=0;

    int new_fd;  // listen on sock_fd, new connection on new_fd
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
   	    pthread_t _exit ;
		srand((unsigned)time(NULL)) ;
		pthread_create(&_exit, NULL, exit_to, &sockfd) ;
    while(1) { 
         // main accept() loop
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
		if (status != 0) 
        {
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
  
    char buff[_MAX_BUFF]={0};
	printf("process = %d thread= %u \n", getpid(),(unsigned int) pthread_self());
	int sock = *(int*)socket;
    while (true)
    {
      bzero(buff, sizeof(buff));
      read(sock, buff, sizeof(buff));
        if (strstr(buff,"TOP"))
        {
            top_peek(sock,'t');
        
        }
        if (strstr(buff,"PEEK"))
        {
            
            top_peek(sock,'p');
     

        }


        if (strstr(buff,"POP"))
        {
       
           pop_dequeve('p');
         

        }
        if (strstr(buff,"DEQUEUE"))
        {
            pop_dequeve('d');
      

        }



        if (strstr(buff,"PUSH"))
        {
     
             push_enqueve(buff,'p');
   
        }

        if (strstr(buff,"ENQUEUE"))
        { 
            push_enqueve(buff,'e');
          

        }
         if(strstr(buff,"EXIT TEST"))
         {
            fflush(stdout);
	        close(sock);
           printf("Thread %u exit\n",(unsigned)pthread_self());
            pthread_rwlock_destroy(&lock) ;
            //pthread_mutex_destroy(&mtx);
            close(sockfd);
            puts("main thred exits");
            freeAll(stach);
            my_free(stach);
            exit(EXIT_SUCCESS);

         }


        if(strstr(buff,"EXIT"))
        {
            fflush(stdout);
	        close(sock);
            printf("Thread %u exit\n",(unsigned)pthread_self());
	        pthread_exit(NULL) ;
        }
    }

}

//------------------------------------------------------

void pop_dequeve(char ch)
{
    pthread_rwlock_wrlock( &lock) ; 
    if(ch=='p')
    {
        //pthread_mutex_lock(&mtx);
        POP(stach);
         //pthread_mutex_unlock(&mtx);
    }
    else
    {
       // pthread_mutex_lock(&mtx);
        DEQUEUE(&stach);
        // pthread_mutex_unlock(&mtx);

    }
    pthread_rwlock_unlock( &lock) ; 
}

void push_enqueve(char *buff,char ch)
{
    pthread_rwlock_wrlock( &lock) ; 
    char text[_MAX_BUFF]={0};
    if(ch=='p')
    {
        for(size_t i=5,j=0;i<strlen(buff);i++,j++)
        {
            text[j]=buff[i];
        }
        //pthread_mutex_lock(&mtx);
         PUSH(stach,text);
         //pthread_mutex_unlock(&mtx);
         


    }
    else
    {
        for(size_t i=8,j=0;i<strlen(buff);i++,j++)
        {
            text[j]=buff[i];
        }
        //pthread_mutex_lock(&mtx);
         ENQUEUE(&stach,text);
        // pthread_mutex_unlock(&mtx);

    }
    pthread_rwlock_unlock( &lock) ; 
}

void top_peek(int sock,char ch)
{
    pthread_rwlock_rdlock( &lock) ; 
    if(stach->size_Node==0)
    {
        write(sock,"NULL", sizeof("NULL"));
    }
    else
    {
        char text[_MAX_BUFF]={0};
        if(ch=='t')
        {
           // pthread_mutex_lock(&mtx);
            strcpy(text,TOP(stach));
          //  pthread_mutex_unlock(&mtx);
        }
        else
        {
            //pthread_mutex_lock(&mtx);
             strcpy(text,PEEK(stach));
             //pthread_mutex_unlock(&mtx);
        }
        int write_size= write(sock, text, sizeof(text));
        if(write_size == 0)
        {
            puts("Client disconnected");
        }

    }
     pthread_rwlock_unlock( &lock) ;

}




void* exit_to(void * sockfd1)
{
    int sock = *(int*)sockfd1;
    printf("Do you want to get out of the server? (YES OR NO)\n");
    while (true)
    {
        char ans[_MAX_BUFF]={0};
        scanf("%s",ans);
        if(strstr(ans,"YES"))
        {
           // pthread_mutex_destroy(&mtx);
            pthread_rwlock_destroy(&lock) ;
            close(sock);
            puts("main thred exits");
            freeAll(stach);
            my_free(stach);
            exit(EXIT_SUCCESS);

        }
    }
    
}