# client_server

## Description
This project consists of two main components: a client and a server. The server is designed to handle multiple client connections simultaneously, processing transactions sent by clients. Clients send transactions to the server, which performs computations and logs the activities. The server keeps track of transactions per client and overall system performance.

## Features
- **Client Application**: Sends transactions to a specified server and receives transaction IDs in response.
- **Server Application**: Handles incoming connections and transactions from multiple clients using socket programming.
- **Concurrency**: The server can handle multiple clients concurrently without blocking.
- **Logging**: Both the client and server log their activities to the console and specified log files, detailing every transaction and system state change.
- **Transaction Processing**: Simulates transaction processing by performing computational tasks.

## References:
1. https://www.ibm.com/docs/en/i/7.1?topic=designs-example-nonblocking-io-select (for server code)
2. https://www.softprayog.in/programming/socket-programming-using-the-select-system-call (for server code)
3. https://www.geeksforgeeks.org/socket-programming-cc/ (for client code)
4. https://www.techiedelight.com/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/ (for epoch time)
6. https://stackoverflow.com/questions/27914311/get-computer-name-and-logged-user-name (to get the host name)
