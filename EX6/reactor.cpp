/**
 * @file reactor.cpp
 * AUTHORS: Niv Kotek - 208236315
 * 
 */


#include "reactor.hpp"


reactor::reactor():fdmax(-1)
{

    pthread_create( &this->_thread, NULL, reactor::selectserver, (void*) this);
    pthread_create( &this->_run, NULL, reactor::exit_to, (void*) this);
    pthread_join(this->_thread,NULL);
    pthread_join(this->_run,NULL);

}

int reactor::get_fdmax()
{
    return this->fdmax;
}
pthread_t reactor::get_thread()
{
    return this->_thread;

}
pthread_t reactor::get_thread_run()
{
    return this->_run;

}
std::unordered_map<int,std::function<void(reactor)>> reactor::get_functions_list()
{
    return this->_functions_list;

}
fd_set reactor::get_file_descriptor_list()
{
    return this->_file_descriptor_list;

}

reactor reactor::newReactor()
{
	return reactor{};
}


void reactor::InstallHandler(int fil,void (*functptr)(reactor),reactor Reactor )
{
    if (!FD_ISSET(fil, &Reactor._file_descriptor_list)) 
    {
        FD_SET(fil, &Reactor._file_descriptor_list); // add to master set
        Reactor._functions_list[fil]=nullptr;
        if (fil > Reactor.fdmax)
	    {    // keep track of the max
          Reactor.fdmax = fil;
        }

    }
    Reactor._functions_list[fil]=functptr;

}

void reactor::RemoveHandler(reactor Reactor)
{
	pthread_cancel(Reactor._thread);
    pthread_cancel(Reactor._run);
	for(int i = 0; i <= Reactor.fdmax; i++) 
	{
		close(i);
		FD_CLR(i, &Reactor._file_descriptor_list); // remove from master set
	}

}

void *reactor::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) 
	{
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void * reactor::selectserver(void *ptr)
{
	reactor *reac=(reactor*)ptr;
    fd_set read_fds;  // temp file descriptor list for select()

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&reac->_file_descriptor_list);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &reac->_file_descriptor_list);
	reac->fdmax=listener;

    // main loop
    while(true) 
	{
        read_fds =reac->_file_descriptor_list; // copy it
        if (select(reac->fdmax+1, &read_fds, NULL, NULL, NULL) == -1) 
		{
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= reac->fdmax; i++) 
		{
            if (FD_ISSET(i, &read_fds)) 
			{ // we got one!!
                if (i == listener) 
				{
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
                       											
                    if (newfd == -1) 
					{
                        perror("accept");
                    } 
					else 
					{
                        FD_SET(newfd, &reac->_file_descriptor_list); // add to master set
                        if(reac->_functions_list[i]==nullptr)
                        {
                            reac->_functions_list[i]=nullptr;

                        }
                        if (newfd > reac->fdmax)
						{    // keep track of the max
                            reac->fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on ""socket %d\n",inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),newfd);
                    }
                } 
				else 
				{
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
					 {
                        // got error or connection closed by client
                        if (nbytes == 0) 
						{
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } 
						else 
						{
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &reac->_file_descriptor_list); // remove from master set
                    } 
					else 
					{
                        if(reac->_functions_list[i]!=nullptr)
                        {
                            reac->_functions_list[i](*reac);
                        }
                        else
                        {
                            if(strstr(buf,"EXIT TEST"))
                            {
                                RemoveHandler(*reac);
                                
                            }
                             // we got some data from a client
                            for(j = 0; j <= reac->fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &reac->_file_descriptor_list)) {
                                // except the listener and ourselves
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }

                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
	return nullptr;

}

void* reactor::exit_to(void* ptr)
{
    reactor *reac=(reactor*)ptr;
    std::cout<<"DO you wat tp get out of the server? (YES OR NO)?"<<std::endl;
    std::string ans;
    while (true)
    {
        std::cin>>ans;
        if(ans=="YES")
        {
           RemoveHandler(*reac);
           return nullptr;
        }


    }
    return nullptr;
    

}


int main(int argc, char const *argv[])
{
	reactor r=reactor::newReactor();
	return 0;
}










