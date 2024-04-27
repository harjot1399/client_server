#include "server.h"

void Trans( int n ) {
	long i, j;

	// Use CPU cycles 
	j = 0;
	for( i = 0; i < n * 100000; i++ ) {
		j += i ^ (i+1) % (i+n);
	}
	TransSave += j;
	TransSave &= 0xff;
}

// Function get epoch time 
double epoch_time(){
    double e_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()/(double)1000;
	return e_time;
}

// Function which create the socket of the server and enables listening
void socket_create(int port){
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Unable to create a socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Allows socket to be reusable
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        cerr << "setsockopt() failed" << endl;
        exit(EXIT_FAILURE);  
    } 

    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(port);

    // setting up the socket to be non-blocking
    // Note: I ran the program without this if statement and i didn't observe much performance difference, overall i think it is not required
    if (ioctl(server_socket, FIONBIO, (char *)&opt) < 0) {
        cerr << "ioctl() failed" << endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Binding the socket
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Binding Failed" << endl;
        exit(EXIT_FAILURE);
    }


    if (listen(server_socket, 32) < 0) {
        cerr << "Unable to listen" << endl;
        exit(EXIT_FAILURE);
    }
}

// Function which performs the transcation and logs the activity in server.log
void transcation_task(int n, string s){
    transcationcount++;
    
    double start_time = epoch_time();
    fprintf(ptr,"%.2f: #%3d (%s%3d) from %s\n",epoch_time(),transcationcount,"T",n,s.c_str());
    Trans(n);
    fprintf(ptr,"%.2f: #%3d (%s) from %s\n",epoch_time(),transcationcount,"Done",s.c_str());
    double finish_time = epoch_time();
    
    // When first trancation takes place it will store the start time 
    if (transcationcount == 1){
        first_transcation_time = start_time;
    }

    // Every finish time is stored and last trancation finisg time will serv as finish time of whole program
    last_trancation_time = finish_time;
    trans_count_from_client.at(s)++;
}

void recieve_and_process(int client_id){
    
    int rc = recv(client_id, recvbuff, sizeof(recvbuff), 0);
    if (rc < 0){
        cerr << "Reading from client failed" << endl;
        exit(EXIT_FAILURE);
    }else if (rc == 0){
        close(client_id);
        FD_CLR(client_id,&all_sockets);
        if (client_id == max_sd){
            while (FD_ISSET(max_sd, &all_sockets) == false){
                max_sd -= 1;
            }
        }
    }else{
        int n;
        char client_identity [100000];
        sscanf(recvbuff, "%d %s",&n,client_identity);
        string c(client_identity);
        trans_count_from_client.emplace(c,0);
        transcation_task(n,c);
        sprintf((char *)sendbuff,"%d", transcationcount);
        // sending response back to clients
        if (send(client_id, sendbuff, sizeof(sendbuff), 0) < 0) {
            cerr << "Sending data to client failed" << endl;
            exit(EXIT_FAILURE);
        }
    }
    
}

void server_summary(){
    fprintf(ptr,"\nSummary\n");
    for (auto const &pair: trans_count_from_client) {
        fprintf(ptr,"   %d transcations from %s\n",pair.second,pair.first.c_str());
    }
    double time_taken = last_trancation_time - first_transcation_time;
    double transcation_rate = (double) transcationcount/time_taken;
    fprintf(ptr, "%.1f transactions/sec (%d/%.2f)\n", transcation_rate, transcationcount, time_taken);
}

     
int main(int argc , char *argv[]) { 
    if (argc < 2) {
        cerr << "Please add a port number to connect the server";
        return 0;
    }
    int portnum = atoi(argv[1]);
    if (portnum > 64000 || portnum < 5000) {
        cerr << "Please enter a port number in valid range" << endl;
        return 0;
    }
    ptr = fopen("server.log","w");
    fprintf(ptr,"Using port %d\n",portnum);
    socket_create(portnum);
    FD_ZERO(&all_sockets);
    int max_sd = server_socket;
    FD_SET(server_socket,&all_sockets);
    // Intializing the struct for timeout requirement
    struct timeval timeout;

    while(1){
        active_sockets = all_sockets;
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        int sel_id = select(max_sd + 1, &active_sockets, NULL, NULL, &timeout);
        if (sel_id < 0)
        {
            cerr << "select failed()" << endl;
            break;
        }  
        if (sel_id == 0)
        {
            cerr << "Server Timed out" << endl;
            break;
        }
        for (int i=0; i <= max_sd; i++)
        {
            if (FD_ISSET(i, &active_sockets)){
                // if i in Fd_set is equal to server_socket that means some client is trying to connect to server
                if (i == server_socket){
                    int new_sd = accept(server_socket, NULL, NULL);
                    if (new_sd < 0){
                        cerr << "Accept Call Failed" << endl;
                    }
                    FD_SET(new_sd, &all_sockets);
                    if (new_sd > max_sd){
                        max_sd = new_sd;
                    }
               }else{
                    recieve_and_process(i);
               }
            } 
        } 
    } 
    server_summary();

    // closing all the sockets
    for (int j=0; j <= max_sd; j++){
        if (FD_ISSET(j, &all_sockets)){
            close(j);
        }
    }
   fclose(ptr);
   return 0;
}