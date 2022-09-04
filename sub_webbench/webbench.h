#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.0"
#define REQUEST_SIZE 2048


extern void build_request(const char *url);
extern int usage(); 
// extern int printf_info(const char *names, ); 
extern int bench(const int clients, const char* host, const int port, const char* request);
extern int Socket(const char *url);
extern int bench_core(const char* host, const int port, const char* request);
