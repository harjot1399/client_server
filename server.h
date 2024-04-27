//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <iostream>
#include <stdio.h> 
#include <string.h>   //strlen 
#include <string>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <chrono>
#include <map>
#include <time.h>

using namespace std;
using namespace chrono;

FILE *ptr;
int server_socket = 0;
struct sockaddr_in address;
char sendbuff[1024] = { 0 };
char recvbuff[1024] = { 0 };
int transcationcount{0};
fd_set all_sockets, active_sockets;
int max_sd;
double first_transcation_time {0};
double last_trancation_time {0};
map <string,int> trans_count_from_client;
int TransSave = 0;
int opt {1};