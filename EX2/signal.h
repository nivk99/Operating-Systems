
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h> 
void SignalOne(int signum);
void SignalTwo(int signum);
void SignalThree(int signum);
void SignalFour(int signum);
void SignalFive(int signum);