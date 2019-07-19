#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

int listen_on_port(char* port) {
    struct addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    getaddrinfo(nullptr, port, &hints, &res);

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, 1);

    return sockfd;
}

int main() {
    int in = accept(listen_on_port("19000"), nullptr, nullptr);
    int out = accept(listen_on_port("19001"), nullptr, nullptr);
    int err = accept(listen_on_port("19002"), nullptr, nullptr);

    char input[] = "bt\n";
    send(in, input, sizeof(input), 0);
    char buf[1024 * 1024] {};
    while (true) {
        memset(buf, 0, sizeof(buf));
        int len = recv(out, buf, sizeof(buf), 0);
        if (len != 0) {
            printf("%s", buf);
        }
    }
}
