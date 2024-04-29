#if defined(_WIN32)
#ifndef  _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#endif

#include <stdio.h>

int main() {
#if defined (_WIN32)
    // initialize the windows WINSOCK API
    WSADATA d;
    if(WSAStartup(MAKEWORD(2,2), &d) ) { // winsock version 2.2
        fprintf(stderr, "failure to initialize\n");
        return -1;
    }
#endif

    printf("Ready to use sockets API!\n");

#if defined(_WIN32)
    WSACleanup();
#endif

    return 0;

}