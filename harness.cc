#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tcp.hh"

#include "protocol.hh"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s pid\n", argv[0]);
        return 1;
    }
    char tool[] = "gdb";
    char *pid = argv[1];
    char *tool_args[] = {"gdb", "-q", "-p", pid, "-ex", "c", NULL};

    int in_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "19000", 10);
    int out_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "19000", 10);
    int err_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "19000", 10);

    connect_message m = {
        good_token,
        tools::gdb,
        streams::in,
        0,
        good_token,
    };
    m.stream_id = streams::in;
    send(in_fd, &m, sizeof(m), 0);
    m.stream_id = streams::out;
    send(out_fd, &m, sizeof(m), 0);
    m.stream_id = streams::err;
    send(err_fd, &m, sizeof(m), 0);

    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);
    dup2(err_fd, STDERR_FILENO);

    close(in_fd);
    close(out_fd);
    close(err_fd);

    execvp(tool, tool_args);
}
