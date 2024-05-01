/**
 * A simple web server that serves date and time
 */

#include  "../macros.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

// NOTE: DEFINE THIS BEFORE INCLUDING ANY WINDOWS HEADER
#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif

#include <winsock2.h>
#include <ws2tcpip.h>

#else // UNIX headers
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>

#endif


int main() {

    // initialize winsock if on windows
#ifdef _WIN32
    WSADATA d;
    if(WSAStartup(MAKEWORD(2,2), &d)) {
        fprintf(stderr, "failed to initialize\n");
        return 1;
    }
#endif


    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "9090", &hints, &bind_address);

    printf("Setting up sockets...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // check for successful socket creation
    if(!(ISVALIDSOCKET(socket_listen))) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n ");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf( stderr,"bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    // allow up to 10 connections to be queued
    if(listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);

    if(!ISVALIDSOCKET(socket_client)) {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    //  a client is connected at this point - log their address
    printf("Client is connected...\n");
    char address_buffer[100];
    getnameinfo((struct sockaddr*) &client_address,
            client_len,
            address_buffer,
            sizeof(address_buffer),
            0,
            0,
            NI_NUMERICHOST
            );

    printf("%s\n", address_buffer);

    // reads the HTTP request sent by the client
    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0); // TODO: check if recv() >0
    printf("Received %d bytes. \n", bytes_received);

    // log the HTTP request
//    printf("%.*s", bytes_received, request);

    // SEND RESPONSE
    printf("Sending response...\n");
    const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Local time is: ";

    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes. \n", bytes_sent, (int) strlen(response));

    // get the time
    time_t timer;
    time(&timer);
    char* time_msg = ctime(&timer);
    printf("Time: %s\n",  time_msg);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes. \n", bytes_sent, (int)strlen(time_msg));

    // CLOSE THE CLIENT CONNECTION
    printf("Closing connection");
    CLOSESOCKET(socket_client);

    // CLOSE THE LISTENING CONNECTION
    printf("Closing listening socket...");
    CLOSESOCKET(socket_listen);

#ifdef _WIN_32
    WSACleanup();
#endif

    printf("Finished.\n");

    return 0;


}