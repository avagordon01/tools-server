#pragma once
#if defined(__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

static const int INVALID_SOCKET = -1;
static const int SOCKET_ERROR = -1;
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
// Windows8 is needed for InetNtop
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace aether {

namespace tcp {

struct service_location {
    char host[256];
    char port[16];
};

#if defined(__linux__)
typedef int os_socket;
#elif defined(_WIN32)
typedef SOCKET os_socket;
#endif

void recv_all(const os_socket sockfd, void *data, size_t size);
void socket_set_nonblocking(const os_socket sockfd);
void close_socket(const os_socket sockfd);
os_socket connect_to_host_port(const char* host, const char* port);
os_socket connect_to_host_port_with_timeout(const char* host, const char* port, int seconds = 600);

}

}
