#include "client.h"

void Sleep( int n ) {
	struct timespec sleep;

	// Make sure pass a valid nanosecond time to nanosleep
	if( n <= 0 || n >= 100 ) {
		n = 1;
	}

	// Sleep for less than one second
	sleep.tv_sec  = 0;
        sleep.tv_nsec = n * 10000000 + TransSave;
	if( nanosleep( &sleep, NULL ) < 0 ) {
		perror ("NanoSleep" );
	}
}

// Function to get epoch time
double epoch_time(){
    double e_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()/(double)1000;
	return e_time;
}

// Function used to create socket
void create_socket(int port, char* s_address){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Unable to create socket" << endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(s_address);
    serv_addr.sin_port = htons(port);

    if ((connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))< 0) {
        cerr << "Connection failed" << endl;
        exit(EXIT_FAILURE);
    }

}

// Function which sends request to server and recieve a unique transcation id
int send_to_server(int n, string f){
    sprintf((char*) sendbuff,"%d %s",n,f.c_str());
    if (send(sock, sendbuff, sizeof(sendbuff), 0) < 0) {
        cerr << "Failed to send data" << endl;
        exit(EXIT_FAILURE);
    }

    if (recv(sock, recvbuff, sizeof(recvbuff), 0) < 0) {
        cerr << "Failed to recive data from server" << endl;
        exit(EXIT_FAILURE);
    }
    // Recieving transcation from server
    int transaction_id = atoi(recvbuff);
    return transaction_id;
}

// This function takes input from file or user, almost same as assignment 2
void input(){
    string str;
    while (!cin.eof()){
        getline(cin, str);
        if (str[0] == 'T'){
            int n = stoi(str.substr(1,str.length()));
            transcationcount++;
            fprintf(stdout,"%.2f: %s (%s%3d)\n",epoch_time(),"Send","T", n);
            int trans_id = send_to_server(n,x);
            fprintf(stdout,"%.2f: %s (%s%3d)\n",epoch_time(),"Recv","D", trans_id);
        }else if (str[0] == 'S'){
            int n = stoi(str.substr(1,str.length()));
            fprintf(stdout,"Sleep %d units\n",n);
            Sleep(n);
        }
    }
}

int main(int argc, char const* argv[])
{
    
    if (argc < 3){
        cerr << "Please enter the port number or IP adrress" << endl;
        return 0;
    }
    int portnum = atoi(argv[1]);
    if (portnum > 64000 || portnum < 5000){
        cerr << "Please enter a port number in valid range" << endl;
        return 0;
    }
    char host_name[HOST_NAME_MAX];
    char server_address[1000];
    pid_t pid = getpid();
    if (gethostname(host_name,HOST_NAME_MAX)< 0){
        cerr << "Error: Unable to get the name of the host" << endl;
        return 0;
    }
    sprintf(server_address,"%s",argv[2]);
    string outfile = string(host_name) + "." + to_string(pid);
    x = outfile;
    fp = freopen(outfile.c_str(),"w",stdout);
    fprintf(stdout,"Using port %d\n",portnum);
    fprintf(stdout,"Using server address %s\n",server_address);
    fprintf(stdout,"Host %s\n",outfile.c_str());
    create_socket(portnum, server_address);
    input();
    fprintf(stdout,"Sent %d transcations",transcationcount);
    // cloing the socket
    close(sock);
    fclose(fp);
    return 0;
}