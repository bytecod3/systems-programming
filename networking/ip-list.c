/**
 * This program lists all the network adapters on windows
 */

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 // allows inclusion of the right header
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <winerror.h>
#include <stdio.h>
#include <stdlib.h>

// link exe with winsock lib for microsoft visual C compiler
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

int main() {

    WSADATA d;

    if(WSAStartup(MAKEWORD(2,2), &d)) {
        printf("Failed to initialize. \n");
        return -1;
    }

    printf("Success\n");

//    // request adapter addresses from windows
    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;

    do {
        adapters = (PIP_ADAPTER_ADDRESSES) malloc(asize);

        if(!adapters) {
            printf("Could not allocate %ld bytes for adapters.\n", asize);
            WSACleanup();
            return -1;
        }

        int r = GetAdaptersAddresses(
                AF_UNSPEC, // get both ipv4 and ipv6 addresses
                GAA_FLAG_INCLUDE_PREFIX,
                0,
                adapters,
                &asize
                );
        if (r == ERROR_BUFFER_OVERFLOW)  {
            printf("GetAdapterAddresses wants %ld bytes.\n", asize);
            free(adapters);
        } else if (r == ERROR_SUCCESS) {
            break;
        } else {
            printf("Error from GetAdapterAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        };

    } while (!adapters);

    // loop through the linked list returned by GetAdapterAddress and print
    // the adapter addresses
    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            printf("\t%s",
                   address->Address.lpSockaddr->sa_family == AF_INET ? "ipv4":"ipv6"
                   );

            char ap[100];

            getnameinfo(address->Address.lpSockaddr,
                        address->Address.iSockaddrLength,
                        ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

            printf("\t%s\n", ap);

            address = address->Next;
        }

        adapter = adapter->Next;
    }

    free(adapters);
    WSACleanup();
    return 0;


}