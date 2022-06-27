 
 /**
 * To start, type make all
 * 
 *  AUTHORS: Niv Kotek - 208236315
 */
 
 #include "signal.h"

int main(int argc, char const *argv[])
{
	 int status;
    signal (SIGCHLD,  SignalOne);
	 if (!(fork())) {
        exit(1);
    }
    wait(&status);
	return 0;
}


void SignalOne(int signum)
{
	printf("Signal One\n");
	fflush(stdout);
	signal (SIGUSR1, SignalTwo);
	raise(SIGUSR1);

}
void SignalTwo(int signum)
{
    printf("Signal Two\n");
	fflush(stdout);
	signal (SIGFPE, SignalThree);
	int i=3;
	int j=0;
	int k;
    k=i/j;
	fflush(stdout);



}
void SignalThree(int signum)
{
	printf("Signal Three\n");
	fflush(stdout);
	 signal (SIGSEGV, SignalFour);
	  char *ch;
     *ch = 'h';


}
void SignalFour(int signum)
{
	printf("Signal Four\n");
	fflush(stdout);
	signal (SIGILL,SignalFive);
	asm("ud2");
	 

}
void SignalFive(int signum)
{
	printf("Signal Five\n");
	fflush(stdout);
	signal (SIGVTALRM, NULL);
	time_t my_t, fire_t;
	my_t = time(0);
	exit(1);

}

