#include "webbench.h"
#include "unistd.h"

#include <cstdio>
 
int bench(const int clients, const char* host, const int port, const char* request)
{
    pid_t pid=0;
    int mypipe[2];
	// test host port
    char *url=NULL;
    int	i=Socket(url);
    if(i<0) {printf("url error\r\n"); return 1;}

    close(i);

    // create pipe;
    if(pipe(mypipe)) 
    {
        printf("pipe failed");
        return 3;
    }

    // cas=time(NULL);
    // while(time(NULL)==cas)
    // sched_yield;   
    for(i=0; i<clients;i++)
    {
        pid=fork();
        if(pid <= (pid_t) 0)
        {
            // I am a child process
            printf("Process failed\n");
            sleep(1);
        }
    }

    if(pid==(pid_t)0)
    {
        printf("Child Process\n");
    }
    else if(pid>(pid_t)0)
    {
        printf("Father Process, ID: %d\n", pid);
    }
    else
    {
        perror("fork error");
    }
    return 0;
}

int bench_core(const char* host, const int* port, const char* request)
{
    printf("bench_core.....\r\n");
    return 0;
}

int Socket(const char *url)
{
    printf("socket is running!");
    return 0;
}


