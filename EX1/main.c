
/**
 * To start, type make all
 * 
 *  AUTHORS: Niv Kotek - 208236315
 */

// It is important! In the link you can see a running video of the program

//  https://drive.google.com/file/d/1h1FPDvBY18lMXIlaFJANEU2BUvK2cEb0/view?usp=sharing




# include "main.h"

int main(int argc, char const *argv[])
{
    char *str;
    while (true)
    {  
        //___________ Q.A(א)_______________________//
        str=input();
        if(!strcmp(str,"EXIT"))
        {
            free(str);
            return 0;
        }
        //___________Q.B(ב)_______________________//
        if(!strcmp(str,"CWD"))
        {
        printf("%s\n",pwd());
        free(str);
         continue;
        }

         //___________Q.C(ג)_______________________//
         if (strstr(str,"ECHO"))
        {
            echo(str);
              free(str);
            continue;
        }

        //___________Q.D(ד)_______________________//
        if (!strcmp(str,"TCP PORT"))
        {
            tcp_port();
              free(str);
            continue;

        }

       //___________Q.D(ה)_______________________//
        if(!strcmp(str,"LOCAL"))
        {
            printf("LOCAL");
            //sleep(2);
            // close the socket
            close(sockClient);
           dup2(STDOUT_FILENO,sockClient);
            free(str);
            exit(EXIT_SUCCESS);
            //continue;

        }

        //___________Q.F(ו)_______________________//
        if(strstr(str,"DIR"))
        {
              dir();
              free(str);
            continue;
        }

        //___________Q.G(ז)_______________________//
        if(strstr(str,"CD"))
        {
            cd (str);
            free(str);
            continue;
        }

        //___________Q.J(י)_______________________//
        if(strstr(str,"COPY"))
        {
            char str1[strlen(str)];
            strcpy(str1,str);
            char * token = strtok(str1, " ");
            token = strtok(NULL, " ");
            char *src=(char*)calloc(strlen(token)+1,sizeof(char));
            if(!src)
            {
                printf("Failed calloc");
                exit(1);
            }
            strcpy(src,token);
            src[strlen(token)]='\0';
            token = strtok(NULL, " ");
            char *det=(char*)calloc(strlen(token)+1,sizeof(char));
              if(!det)
            {
                printf("Failed calloc");
                exit(1);
            }
            strcpy(det,token);
            det[strlen(token)]='\0';
            file(src ,det);
            free(src);
            free(det);
            free(str);
            continue;

        }
      //___________Q.k(יא)_______________________//
        if(strstr(str,"DELETE"))
        {
            del(str);
            free(str);
            continue;


        }

        //___________Q.H(ח)_______________________//
        // else
        // {
        //     sys(str);
        //     free(str);
        //     continue;

        // }
        
         //___________Q.I(ט)_______________________//
        else
        {
            int buf=2;
            for(int i=0;i<strlen(str);i++)
            {
                if(str[i]==' ')
                {
                    buf++;
                }
            }
            char* argument_list[buf];
            argument_list[buf-1]=NULL;
            char str1[strlen(str)];
             strcpy(str1,str);
            char * token = strtok(str1, " ");
            int i=0;
              while( token != NULL ) {

               argument_list[i++]=token;
    
               token = strtok(NULL, " ");
           }

             execArgs(argument_list);
        }
          free(str);



    }


    return 0;
}


/**
 * Q.A(א)
 * 
 *  ***The code was taken from the test in systems programming***
 * 
 */
char* input() { 
 printf("yes master?\n");
 int count = 0;
 char c;
 char* str = (char*)calloc((count+1),sizeof(char));
 char* cpy = str;
 while(scanf("%c",&c) && c != '\n')
 {
    count++;
    str = realloc(str, (count+1)*sizeof(char));
    if(!str)
    {
        free(cpy);
        return NULL;
    }
    cpy = str;
    str[count-1] = c;
 }
 str[count] = '\0';
 return str; 
}


/**
 * Q.B(ב)
 * https://pubs.opengroup.org/onlinepubs/007904975/functions/getcwd.html
 * 
 */
char* pwd()
{
    char *buf;
    char *ptr;
    int size = pathconf(".", _PC_PATH_MAX);
        
    if ((buf = (char *)malloc((size_t)size)) != NULL)
    {
      ptr = getcwd(buf, (size_t)size);
    }
    free(buf);

     return ptr;


}

/**
 * Q.C(ג)
 * 
 */
void echo(char *str)
{
    for(int i=5;i<strlen(str);i++)
        {
            printf("%c",str[i]);

        }
    printf("\n");

}

/**
 * Q.C(ד)
 */
void tcp_port()
{
    pid_t fork1,fork2;
    fork1=fork();
    if (fork1<0)
    {
        perror("cannot fork");
        exit(EXIT_FAILURE);
    }
    if (fork1==0)
    {
        sys("gcc tcp_server.c -o server");
        sys("./server");
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
        sleep(2);
        client();
    }
    else 
    {
        wait(NULL);
		wait(NULL);

    }



}

//https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
void client()
{
	struct sockaddr_in servaddr;

	// socket create and verification
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
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
    // dup2 -  Standard output. 
    dup2(sockClient,STDOUT_FILENO);

}


/**
 * Q.F
 * 
 * https://stackoverflow.com/questions/3554120/open-directory-using-c
 * 
 * 
 */
void dir()
{
    DIR *dir;
    struct dirent *dent;
    dir = opendir(pwd()); 
    if(dir!=NULL)
    {
        while((dent=readdir(dir))!=NULL)
        {
             if((strcmp(dent->d_name,".")!=0 && strcmp(dent->d_name,"..")!=0 && (*dent->d_name) != '.' ))
            {
                 printf("%s\n",dent->d_name);
            }
        }
    }
    else
    {
        printf ("Cannot open directory");
    }
   closedir(dir);

}


/**
 * Q.G(ז)
 * 
 * https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
 * 
 *  The function uses system calls
 * 
 */
void cd (char *str)
{ 
    //CD file -->file
       str+=3;
      if(chdir(str) != 0)
        {
           perror("chdir() failed");
        }
}

/**
 * Q.H(ח)
 * 
 *  The function uses system calls
 */
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


/**
 * Q.(ט)
 * https://github.com/Aske21/operating-systems-project/blob/main/shell.c
 * The function uses a library function
 * 
 */
void execArgs(char** parsed)
{

//fork 
pid_t pid = fork();

//check for err
if (pid==-1){
	//Print err message
	printf("\nError, failed forking...\nResetting");
	return;

}else if(pid ==0){
	//if successfull fork but err executing
if(execvp(parsed[0],parsed)<0)
{
		printf("\nError, failed executing...\nResetting");
		}
		
exit(0);
}else{
//wait for chiled
wait(NULL);
return;
 }
}


/**
 *  Q.(י)
 * The function uses a library function
 * 
 * 
 */
void file(char *src , char *dst)
{
    FILE *fsrc,*fdest;
    char c;
    printf("%s\n",src);
    printf("%s\n",dst);

    fsrc=fopen(src,"r");
    fdest=fopen(dst,"w");
    if(!fsrc||!fdest)
    {
        perror("failed to open");
        exit(EXIT_FAILURE);
    }
    fputc('c',fdest);

    while (true)
    {
      c = fgetc(fsrc);
      if( feof(fsrc) )
       { 
         break ;
        }
    fputc(c,fdest);
        // print to stdout
        //fputc(c,stdout);
    }
     fclose(fsrc);
     fclose(fdest);
    

}

/**
 * Q.(י.א)
 *  The function uses system calls  (https://linux.die.net/man/2/unlink)
 * 
 */

void del(char *str)
{
    str+=7;
   unlink(str);
}




