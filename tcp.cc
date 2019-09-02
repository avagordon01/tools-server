#include "tcp.hh"

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include "timer.hh"

static void *get_in_addr(sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &((sockaddr_in*)sa)->sin_addr;
    }
    return &((sockaddr_in6*)sa)->sin6_addr;
}

namespace aether {

namespace tcp {

void recv_all(const os_socket sockfd, void *data, size_t size) {
    size_t recvbytes;
    int n;
    for (recvbytes = 0; recvbytes < size; recvbytes += n) {
        n = recv(sockfd, (char*)data+recvbytes, size-recvbytes, MSG_WAITALL);
        if (n == -1) {
            perror("recv");
            exit(1);
        }
    }
    assert(recvbytes == size);
}

void socket_set_nonblocking(const os_socket sockfd) {
#if defined(__linux__)
    const int flags = fcntl(sockfd, F_GETFL, 0);
    assert(flags != -1);
    const int ret = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    assert(ret == 0);
#elif defined(_WIN32)
    unsigned long i_mode = 1;
    const auto result = ioctlsocket(sockfd, FIONBIO, &i_mode);
    assert(result == NO_ERROR);
#endif
}

void close_socket(const os_socket sockfd) {
#if defined(__linux__)
    const auto err = close(sockfd);
#elif defined(_WIN32)
    const auto err = closesocket(sockfd);
#endif
    assert(err != SOCKET_ERROR);
}

template<typename T>
static void setsockopt_abort(int fd, int level, int optname, const T value, const char *msg) {
#if defined(__linux__)
    const auto opt_ptr = &value;
#elif defined(_WIN32)
    const auto opt_ptr = static_cast<const char*>(static_cast<const void*>(&value));
#endif

    const auto ret = setsockopt(fd, level, optname, opt_ptr, sizeof(value));
    if (ret != 0) {
        perror(msg);
        abort();
    }
}


os_socket connect_to_host_port(const char* host, const char* port) {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *servinfo;
    int rv = getaddrinfo(host, port, &hints, &servinfo);
    if (rv != 0) {
        printf("getaddrinfo: %s", gai_strerror(rv));
        return INVALID_SOCKET;
    }
    addrinfo *p;
    os_socket sockfd;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == INVALID_SOCKET) {
            perror("socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            perror("connect");
            close_socket(sockfd);
            continue;
        }
        break;
    }
    if (!p) {
        printf("failed to connect");
        return INVALID_SOCKET;
    }
    char host_str[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), host_str, sizeof(host_str));

    // enable TCP NoDelay
    setsockopt_abort<int>(sockfd, IPPROTO_TCP, TCP_NODELAY, 1, "setsockopt(TCP_NODELAY) failed");

#if defined(__linux__)
    //enable the KeepAlive option
    setsockopt_abort<int>(sockfd, SOL_SOCKET,  SO_KEEPALIVE, 1, "setsockopt(SO_KEEPALIVE) failed");

    //seconds until we start sending probes
    setsockopt_abort<int>(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, 120, "setsockopt(TCP_KEEPIDLE) failed");

    //seconds between each probe
    setsockopt_abort<int>(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, 10, "setsockopt(TCP_KEEPINTVL) failed");

    //number of probes before we assume the connection is dead
    setsockopt_abort<int>(sockfd, IPPROTO_TCP, TCP_KEEPCNT, 6, "setsockopt(TCP_KEEPCNT) failed");
#endif

    freeaddrinfo(servinfo);
    return sockfd;
}

os_socket connect_to_host_port_with_timeout(const char* host, const char* port, int seconds) {
    using namespace std::chrono_literals;
    const auto end = timer::add(timer::get(), 1s * seconds);
    bool first_try = true;
    while (first_try || timer::diff(timer::get(), end) < 0) {
        const auto r = connect_to_host_port(host, port);
        if (r != INVALID_SOCKET) { return r; }
        first_try = false;
        std::this_thread::sleep_for(1s);
    }
    printf("timed out connecting to %s: %s", host, port);
    return INVALID_SOCKET;
}

}

}
