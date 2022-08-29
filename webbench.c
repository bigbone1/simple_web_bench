// #include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdio.h> 

#include "webbench.h"



/*values*/
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;

/* globals */
int http10=1;
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
int method=METHOD_GET;
int clients=1;
int force=0;
int force_reload=0;
int benchtime=30;
int proxyport=80;
char *proxyhost=NULL; 

char host[MAXHOSTNAMELEN];
#define REQUEST_SIZE 2048
char request[REQUEST_SIZE];

int usage(void)
{
	fprintf(stdout, 
			            "webbench [option]... URL\n"
            "  -f|--force               Don't wait for reply from server.\n"
            "  -r|--reload              Send reload request - Pragma: no-cache.\n"
            "  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
            "  -p|--proxy <server:port> Use proxy server for request.\n"
            "  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
            "  -9|--http09              Use HTTP/0.9 style requests.\n"
            "  -1|--http10              Use HTTP/1.0 protocol.\n"
            "  -2|--http11              Use HTTP/1.1 protocol.\n"
            "  --get                    Use GET request method.\n"
            "  --head                   Use HEAD request method.\n"
            "  --options                Use OPTIONS request method.\n"
            "  --trace                  Use TRACE request method.\n"
            "  -?|-h|--help             This information.\n"
            "  -V|--version             Display program version.\n"
	);
	printf("usage func was called\n");
	return 0;
}

static const struct option long_options[]=
{
	    {"force",no_argument,&force,1},
    {"reload",no_argument,&force_reload,1},
    {"time",required_argument,NULL,'t'},
    {"help",no_argument,NULL,'?'},
    {"http09",no_argument,NULL,'9'},
    {"http10",no_argument,NULL,'1'},
    {"http11",no_argument,NULL,'2'},
    {"get",no_argument,&method,METHOD_GET},
    {"head",no_argument,&method,METHOD_HEAD},
    {"options",no_argument,&method,METHOD_OPTIONS},
    {"trace",no_argument,&method,METHOD_TRACE},
    {"version",no_argument,NULL,'V'},
    {"proxy",required_argument,NULL,'p'},
    {"clients",required_argument,NULL,'c'},
    {NULL,0,NULL,0}

};
 

int main(int argc, char *argv[])
{
	int opt=0;
	int options_index=0;
	char *tmp=NULL;
	// setbuf(stdout,NULL);

	//参数个数识别
	if (argc==1)
		usage();
	printf("argc is %d\n", argc);
	while((opt=getopt_long(argc, argv, "912Vfrt:p:c:?h", long_options, &options_index)) != EOF)
	{
		switch (opt)
		{
		case 0:	
			printf("0 was input! /n");
			break;
		case 'f': 
			force=1; 
			printf("%d/n", force);
			break;
		case 'r': force_reload=1; break;
		case '9': http10=0;break;
		case '1': http10=1;break;
		case '2': http10=2;break;
		case 'V': printf(PROGRAM_VERSION"\n");exit(0);
		case 't': 
			benchtime=atoi(optarg);
			break;	     
		case 'p':
			tmp = strrchr(optarg, ':');
			proxyhost=optarg;
			if(tmp==NULL)
			{
				printf("exit from tmp==NULL. \n");
				break;
			}
			else if (tmp==optarg)
			{
				fprintf(stderr, "%s: missing hostname. \n", optarg);
				break;
			}
			else if (tmp==optarg+strlen(optarg)-1)
			{	
				fprintf(stderr, "%s: missing port number. \n", optarg);
				break;
			}
		default:
			break;
	}
	}
	if(optind==argc)
	{
		fprintf(stderr, "missing URL\n");
		// usage();
		return 2;
	}

	// default options
	if(clients==0) clients=1;
	if(benchtime==0) benchtime==30;

	int i=0;
	for(i;i<=optind;i++)
		printf("Arg %d is:  %s\n", i, argv[i]);
	printf("Args end: %s\n", argv[optind]);
	build_request(argv[optind]);
	printf("Build request: over\n");
	exit(0);
}

void build_request(const char *url)
{
	int i;

	memset(host, 0, MAXHOSTNAMELEN);
	memset(request, 0, REQUEST_SIZE);

	http10=1;
	switch (method)
	{
	case METHOD_GET: 
		strcpy(request, "GET"); break;
	case METHOD_HEAD: strcpy(request, "HEAD"); break;
	case METHOD_OPTIONS: strcpy(request, "OPTIONS"); break;
	case METHOD_TRACE: strcpy(request, "TRACE"); break;
	
	default:
		break;
	}
	strcat(request, " ");	
	printf("Request: %s\n", request);
	// make sure: http url is valid.
	if(NULL==strstr(url,"://"))
	{
		fprintf(stderr, "%s is not a valid URL", url);
		exit(2);
	}
	if (strlen(url)>1500)
	{
		fprintf(stderr, "%s more than 1500 bytes", url);
		exit(2);
	}
	if (0!=strncasecmp(url, "http://", 7))
	{
		fprintf(stderr, "must use a http protcol");
		exit(2);
	}
	i=strstr(url,"://")-url+3;
	if (strstr(url, "/")==NULL)
	{
		fprintf(stderr, "URL: %s not ends with '/'", url);
	}


	
}












