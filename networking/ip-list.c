/**
 * This program lists all the network adapters on windows
 */

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // link exe with winsock lib for microsoft visual C compiler

int main() {

    WSADATA d;

    if(WSAStartup(MAKEWORD(2,2), &d)) {
        printf("Failed to initialize. \n");
        return -1;
    }

    WSACleanup();
    printf("Ok. \n");
    return 0;
    
}