#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tcp.hh"

int tool_harness(char tool[], char *tool_args[], int in_fd, int out_fd, int err_fd) {
    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);
    //dup2(err_fd, STDOUT_FILENO);

    close(in_fd);
    close(out_fd);
    close(err_fd);

    execvp(tool, tool_args);
}

int main() {
    char tool[] = "gdb";
    char *tool_args[] = {"gdb", "-q", "-p", "7900", "-ex", "c", NULL};

    int in = aether::tcp::connect_to_host_port_with_timeout("127.1", "19000", 10);
    int out = aether::tcp::connect_to_host_port_with_timeout("127.1", "19001", 10);
    int err = aether::tcp::connect_to_host_port_with_timeout("127.1", "19002", 10);

    tool_harness(tool, tool_args, in, out, err);
}
