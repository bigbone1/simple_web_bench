#include "webbench.h"
#include "unistd.h"

#include <cstdio>



/*values*/
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;

int bench(int clients, const char* host, const int port, const char* request)
{
    pid_t pid=0;
    int mypipe[2];
    FILE *f;
    int i, j, k; 

    // test host port
    char *url=NULL;
    i=Socket(url);
    if(i<0) {printf("url error\r\n"); return 1;}

    close(i);

    // create pipe;
    if(pipe(mypipe)) 
    {
        printf("pipe failed\r\n");
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
            // printf("Process failed\n");
            sleep(1);
        }
    }

    if(pid==(pid_t)0) 
    { 
        bench_core(host, port, request);

        // write results into pipes.
        f=fdopen(mypipe[1], "w");
        if(f==NULL)
        {
            perror("Pipe error\r\n");
            return 3;
        }
        

        fprintf(f, "%d %d %d\r\n", speed, failed, bytes);
        fclose(f);
        return 0;
    }
    else if(pid>(pid_t)0)
    {
        printf("Father process, son PID: %d\r\n", pid);
        f=fdopen(mypipe[0], "r");
        if(f==NULL)
        {
            perror("Pipe error\r\n");
            return 3;
        }

        setvbuf(f, NULL, _IONBF, 0);
        
        speed=0;
        failed=0;
        bytes=0;
        
        // 父程序时刻监听子程序发送内容
        while(1)
        {
            pid=fscanf(f, "%d %d %d", &i, &j, &k);

            if(pid<2) 
            {
                fprintf(stderr, "some child process is dead");
                break;
            }

            speed+=i;
            failed+=j;
            bytes+=k;

            if(--clients==0) break;
                    
        }
        return i;
    }
    else
    {
        perror("fork error");
        return 3;
    }
    return 0;
}

int bench_core(const char* host, const int port, const char* request)
{
    printf("Child process is running, bench_core.....\r\n");
    return 0;
}

int Socket(const char *url)
{
    printf("socket is running!\r\n");
    return 0;
}


