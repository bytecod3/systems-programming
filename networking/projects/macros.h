#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <windows.h>
#include <winerror.h>
#include <winsock2.h>

// socket data type
#ifndef _WIN32
#define SOCKET int
#endif

// invalid sockets
#ifdef _WIN32
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET )
#else
#define ISVALIDSOCKET(s) ((s) >= 0) // for UNIX
#endif

// closing sockets
#ifdef _WIN32
#define CLOSESOCKET(s)  closesocket(s)
#else
#define CLOSESOCKET(s) close(s) // UNIX
#endif

// error handling
#ifdef _WIN32
#define GETSOCKETERRNO() WSAGetLastError()
#else
#define GETSOCKETERRNO() (errno)

#endif
