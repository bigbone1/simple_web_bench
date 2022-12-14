#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdio.h> 
#include <cstddef>

#include "webbench.h"





/* globals */
int http10=1;   // 0:http0.9   1:http1.0  2:http1.1

int method=METHOD_GET;
int clients=1;
int force=0;
int force_reload=0;
int benchtime=30;
int proxyport=80;
char *proxyhost=NULL; 

char host[MAXHOSTNAMELEN];

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

	//??????????????????
	if (argc==1)
		usage();
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
		case 'c':
            clients=atoi(optarg);
            break;
        case ':':
        case '?':
        case 'h':usage();
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
	printf("Args list:\r\n");
	for(i;i<=optind;i++)
	{
		printf("Arg %d is:  %s\n", i, argv[i]);
	}
	printf("\r\n");

	build_request(argv[optind]);
	 
	printf("Running info:\r\n");
	printf("clients: %d\r\n", clients);
	printf("benchtime: %d\r\n", benchtime);
	if(host==NULL) bench(clients, proxyhost, proxyport, request);
	else bench(clients, host, proxyport, request); 
}

void build_request(const char *url)
{
	int i;
	char *tmp;

	memset(host, 0, MAXHOSTNAMELEN);
	memset(request, 0, REQUEST_SIZE);

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
	// make sure: http url is valid.
	if(NULL==strstr(url,"://"))
	{
		fprintf(stderr, "%s is not a valid URL", url);
		exit(1);
	}
	if (strlen(url)>1499)
	{
		fprintf(stderr, "%s more than 1499 bytes", url);
		exit(1);
	}
	if (0!=strncasecmp(url, "http://", 7))
	{
		fprintf(stderr, "must use a http protcol");
		exit(1);
	}
	i=strstr(url,"://")-url+3;
	if (strstr(url, "/")==NULL)
	{
		fprintf(stderr, "URL: %s not ends with '/'", url);
	}

	// get proxyhost from url
	if (proxyhost==NULL)
	{	
		
		if (NULL!=index(url+i, ':') && index(url+i, ':') < index(proxyhost, '/'))
		{	
			strncpy(host, url, index(url+i, ':')-url);
			strncpy(tmp, index(url+i, ':')+0, index(url+i, '/')-index(url+i, ':')-1);
			proxyport=atoi(tmp);
			if (proxyport==0) proxyport=80;
		}
		// default port number and hostname.
		else
		{
			proxyport=80;
			strncpy(host, url+i, strcspn(url+i, "/"));
		}

		strcat(request+strlen(request), url+i+strcspn(url+i, "/"));

	}
	else
	{
		strcat(request, url);
	}

	// strcat(request, " ");
	if (http10==1)
		strcat(request, " HTTP/1.0");
	else if (http10==2)
	{
		strcat(request, " HTTP/1.1");
	}
	// printf("Request: %s\n", request);
 
	strcat(request, "\r\n");
	if(http10>0)
		strcat(request, "User-Agent: SimpleWebBench" PROGRAM_VERSION "\r\n");

	
	if (proxyhost==NULL && http10>0)
	{
		strcat(request, "Host: ");
		strcat(request, host);
		strcat(request, "\r\n");
	}
	// printf("%s", request);
	if (force_reload && proxyhost!=NULL) 
		strcat(request, "Pragme: no-cache\r\n");
	if(http10>1) strcat(request, "Connection: close\n");	

	// add blank line, and pirnt request.
	strcat(request, "\r\n");
	printf("Request:\r\n%s", request);
}













