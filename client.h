#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <climits>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;


int sock = 0;
char recvbuff[1024];
char sendbuff[1024];
struct sockaddr_in serv_addr;
int transcationcount {0};
int TransSave = 0;
FILE *fp;
string x;