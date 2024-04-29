#include <windows.h>
#include <winerror.h>
#include <winsock2.h>

// socket data type
#ifndef _WIN32
#define SOCKET int
#endif

// invalid sockets
#ifdef _WIN32
#define ISVALIDSOCKET(s) ((s) != INVALIDSOCKET)
#else
#define ISVALIDSOCKET(s) ((s) >= 0) // for UNIX
#endif

// closing sockets
#ifdef _WIN32
#define CLOSESOCKET  closesocket()
#else
#define CLOSESOCKET close() // UNIX
#endif

// error handling
#ifdef _WIN32
#define GETSOCKETERRNO() WSALastError()
#else
#define GETSOCKETERRNO() (errno)

#endif
