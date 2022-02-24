#include <sys/types.h>
#include <signal.h>
#include<stdio.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    if(3 != argc)
    {
        printf("[Arguments ERROR!]\n");
        printf("\tUsage:\n");
        printf("\t\t%s <Target_PID> <Signal_Number>\n", argv[0]);
        return -1;
    }
    int pid = atoi(argv[1]);
    int sig = atoi(argv[2]);
    //int kill(pid_t pid, int sig);
    if(pid > 0 && sig > 0)
    {
        kill(pid, sig);
    }
    else
    {
        printf("Target_PID or Signal_Number MUST bigger than 0!\n");
    }
    
    return 0;
}