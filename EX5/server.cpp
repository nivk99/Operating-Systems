
/**
 * compile: gcc -Wall -pthread  -lpthread  my_stach.cpp server.cpp heap.cpp -o server
 * 
 * To start, type make all
 * To exit the program you have to write yes.
 *  @file server.cpp
 *  AUTHORS: Niv Kotek - 208236315
 */
#include "server.hpp"
 struct flock lock;
 int sockfd;
 PStack stach;
#define PORT "8584"  // the port users will be connecting to

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

int main(void)
{
    pid_t childpid;
     int  new_fd;
      // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
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
     stach=(PStack)my_malloc(sizeof(Stack),sockfd);
    if(!stach)
    {
        exit(1);
    }
    
    stach->size_Node=0;

    printf("server: waiting for connections...\n");
       pthread_t _exit ;
		srand((unsigned)time(NULL)) ;
		pthread_create(&_exit, NULL, exit_to, &sockfd) ;

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


         int i, pid; 
        void *child_stack; 
        srand( (unsigned) time(NULL)) ; 
        /* allocate memory for other process to execute in */ 
        if((child_stack = (void *) malloc(4096)) == NULL) { 
        perror("Cannot allocate stack for child"); 
        exit(EXIT_FAILURE); 
        }
        if ((pid = clone(inc_second, 
            child_stack+4096, 
            CLONE_VM | SIGCHLD,&new_fd)) < 0) { 
            perror("clone failed."); 
            exit(EXIT_FAILURE); 
                }
         close(new_fd);  

        //  childpid=fork();
        // if (childpid < 0) 
        // {
        //      fprintf(stderr, "Could not create process: %s\n", strerror(errno));
        // }


        // if (childpid!=0) { // this is the child process
        // close(sockfd); 
        //   send_to(new_fd, &stach);
        //     //close(sockfd); // child doesn't need the listener
        //     // if (send(new_fd, "Hello, world!", 13, 0) == -1)
        //     //     perror("send");
        //      exit(0);
        // }
        // close(new_fd);  // parent doesn't need this
    }
 //close(new_fd);
    return 0;
}


//-------------------------------------------------------// process
int inc_second(void * socket)
{
  int sock = *(int*)socket;
    char buff[_MAX_BUFF]={0};
	printf("process = %d thread= %u \n", getpid(),(unsigned int) pthread_self());
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
       
           pop_dequeve(sock,'p');
         

        }
        if (strstr(buff,"DEQUEUE"))
        {
            pop_dequeve(sock,'d');
      

        }



        if (strstr(buff,"PUSH"))
        {
     
             push_enqueve(sock,buff,'p');
   
        }

        if (strstr(buff,"ENQUEUE"))
        { 
            push_enqueve(sock,buff,'e');
          

        }
         if(strstr(buff,"EXIT TEST"))
         {
            fflush(stdout);
	        close(sock);
           printf("Thread %u exit\n",(unsigned)pthread_self());
          
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
            printf("process = %d exit\n",getpid());
            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;

}

//------------------------------------------------------

void pop_dequeve(int sock,char ch)
{
    memset (&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    fcntl (sock, F_SETLKW, &lock);
  
    if(ch=='p')
    {
        
        POP(stach);
        
    }
    else
    {
      
        DEQUEUE(&stach);
       

    }
    lock.l_type = F_UNLCK;
    fcntl (sock, F_SETLKW, &lock);

}

void push_enqueve(int sock,char *buff,char ch)
{
     memset (&lock, 0, sizeof(lock));
     lock.l_type = F_WRLCK;
    fcntl (sock, F_SETLKW, &lock);
    
    char text[_MAX_BUFF]={0};
    if(ch=='p')
    {
        for(size_t i=5,j=0;i<strlen(buff);i++,j++)
        {
            text[j]=buff[i];
        }
        
         PUSH(stach,text,sock);
        
         


    }
    else
    {
        for(size_t i=8,j=0;i<strlen(buff);i++,j++)
        {
            text[j]=buff[i];
        }
        
         ENQUEUE(&stach,text,sock);
       

    }
    lock.l_type = F_UNLCK;
    fcntl (sock, F_SETLKW, &lock);

}

void top_peek(int sock,char ch)
{
     memset (&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    fcntl (sock, F_SETLKW, &lock);
    
    if((stach)->size_Node==0)
    {
        send(sock,"NULL", sizeof("NULL"),0);
    }
    else
    {
        char text[_MAX_BUFF]={0};
        if(ch=='t')
        {
           
            strcpy(text,TOP(stach));
          
        }
        else
        {
            
             strcpy(text,PEEK(stach));
        }
        int write_size= send(sock, text, sizeof(text),0);
        if(write_size == 0)
        {
            puts("Client disconnected");
        }

    }
    lock.l_type = F_UNLCK;
    fcntl (sock, F_SETLKW, &lock);
    

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
          
            close(sock);
            puts("main thred exits");
            //freeAll(stach);
            //my_free(stach);
            exit(EXIT_SUCCESS);

        }
    }
    
}