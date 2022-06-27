
/**
 * @file main1.c
 * AUTHORS: Niv Kotek - 208236315
 * 
 */



#include "main1.h"

pthread_mutex_t mtx= PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cv = PTHREAD_COND_INITIALIZER; 
Pqueue AO_one;
Pqueue AO_two;
Pqueue AO_three;
Pqueue createQ()
{
	Pqueue que=(Pqueue)malloc(sizeof(Queue));
	if (!que) {
		fputs("createQ: cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}

	que->size = 0;
	que->head = NULL;
	que->tail = NULL;
	return que;
}

void destoryQ(Pqueue que)
{
	 pthread_mutex_lock(&mtx);
	if (!que) {
		pthread_mutex_unlock(&mtx);
		return;
	}

	while (que->head != NULL) {
		Pnode tmp = que->head;
		que->head = que->head->next;
		if (tmp->value != NULL) {
			free(tmp->value);
		}
		free(tmp);
	}

	if (que->tail != NULL) {
		free(que->tail);
	}

	free (que);
	pthread_mutex_unlock(&mtx);

}
void enQ(Pqueue que,void *value)
{
	pthread_mutex_lock(&mtx);
	 Pnode node = (Pnode)malloc(sizeof(Node));

	if (!node) {
		fputs("enQ: cannot allocate memory\n",stderr); 
		pthread_mutex_unlock(&mtx);
		pthread_cond_signal(& cv);
		exit(EXIT_FAILURE);
	}

	node->value = value;
	node->next = NULL;

	if (que->head == NULL) {
		que->head = node;
		que->tail = node;
		que->size = 1;
		pthread_mutex_unlock(&mtx);
		pthread_cond_signal(& cv);
		return;
	}
	que->tail->next = node;
	que->tail = node;
	que->size += 1;
	pthread_mutex_unlock(&mtx);
	pthread_cond_signal(& cv);
}
void* deQ(Pqueue que)
{
	 pthread_mutex_lock(&mtx);
	 while (true)
	 {
		 if (que->size != 0)
		 {
			void *value = NULL;
			Pnode tmp = NULL;

			value = que->head->value;
			tmp = que->head;
			que->head = que->head->next;
			que->size -= 1;
			free(tmp);
			pthread_mutex_unlock(&mtx);
			return value;

		 }
		 else
		 {
			pthread_cond_wait(&cv, &mtx);

		 }
		 
	 }
	 pthread_mutex_unlock(&mtx);
	 return NULL;


}

void start_queue_ao()
{
	AO_one=createQ();
	AO_two=createQ();
	AO_three=createQ();
	void (*Function_before1)(char *)=&(before_activ_object_1);
	 void (*Function_before2)(char *)=&(before_activ_object_2);
	 void (*Function_before3)(char *)=&(before_activ_object_3);

	void (*Function_after1)(char *)=&after_activ_object_1;
	void (*Function_after2)(char *)=&(after_activ_object_2);
	void (*Function_after3)(char *)=&(after_activ_object_3);

	activ_object(AO_one,Function_before1,Function_after1);
	activ_object(AO_two,Function_before2,Function_after2);
	activ_object(AO_three,Function_before3,Function_after3);

}



void activ_object(Pqueue que,void (*Function_before)(char *),void (*Function_after)(char *))
{
	PAO ao=(PAO)malloc(sizeof(AO));
	if (!ao) {
		fputs("createQ: cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	ao->que=que;
	ao->Function_after=Function_after;
	ao->Function_before=Function_before;
	pthread_t thread_data;
    int status ;
	status = pthread_create(&thread_data, NULL, newAO, ao) ;
	if (status != 0) 
    {
		perror("pthread_create failed in main") ;
		exit(EXIT_FAILURE) ;
	}
	//pthread_join(thread_data,NULL);
}



void* newAO(void* val)
{
	PAO ao=(PAO)val;
	while (true)
	 {
		 char * data =(char *)deQ(ao->que);
		 ao->Function_before(data);
		 ao->Function_after(data);
	}
	return NULL;
	
}
void destoryAO(Pqueue que,int id)
{
	pthread_cancel(id);
	destoryQ(que);
}

void before_activ_object_1(char * str)
{

	for(int i=0;i<strlen(str);i++)
	{
		str[i]+=1;
		if(str[i]>'z')
		{
			str[i]='a';
			continue;
		}
		if(str[i]>'Z'&&str[i]>='A'&&!(str[i]>='a'&&str[i]<='z'))
		{
			str[i]='A';
			continue;

		}
	}

}
void after_activ_object_1(char * str)
{
	enQ(AO_two,str);
}

void before_activ_object_2(char * str)
{
	for(int i=0;i<strlen(str);i++)
    {
        if(str[i] >= 'a' && 'z' >= str[i])
         {
            str[i]-=32;
			continue;
         }
		if(str[i] >= 'A' && 'Z' >= str[i])
        {
            str[i]+=32;
			continue;
        }


    }

}
void after_activ_object_2(char * str)
{
	enQ(AO_three,str);

}

void before_activ_object_3(char * str)
{
	if(strlen(str)==0)
	{
		exit(1);

	}

}
void after_activ_object_3(char * str)
{
	int connfd=((int)*(str-ExtraBytes));
	write(connfd, str, sizeof(str));

}





//_______________________________________server________________________________________________

/**
 * compile: gcc -Wall -pthread  -lpthread  server_threads.c -o server
 * server threads
 * 
 * To start, type make all
 *  @file server_threads.c
 *  AUTHORS: Niv Kotek - 208236315
 */


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









//-------------------------------------------------------

int main() { 
    start_queue_ao();

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
	pthread_t _exit;
	pthread_create(&_exit,NULL,exit_to,&sockfd);

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
	const int  MAX=1024;
	int sock = *(int*)socket;
	char *buff=(char *)malloc(sizeof(char)*(MAX+ExtraBytes));
	buff[0]=(char)sock;
	buff=buff+ExtraBytes;

	while(true) 
	{

        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(sock, buff, MAX);
		if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
			write(sock, "exit", sizeof("exit"));
			fflush(stdout);
			close(sock);
			pthread_exit(NULL) ;
            break;
        }
		if(strstr(buff,"EXIT TEST"))
		{
			close(sock);
			puts("main thred exits");
			exit(EXIT_SUCCESS);
		}
		enQ(AO_one,buff);
        printf("From client: %s\t To client : ", buff);
		sleep(2);
    }
	return NULL;


}


void* exit_to(void* sockfd1)
{
	int sock=*(int*)sockfd1;
	printf("Do you want to get out of the server (YES OR NO)?\n");
	while (true)
	{
		char ans[1024]={0};
		scanf("%s",ans);
		if(strstr(ans,"YES"))
		{
			close(sock);
			puts("main thred exits");
			exit(EXIT_SUCCESS);
		}


	}
	return NULL;
	

    

}



