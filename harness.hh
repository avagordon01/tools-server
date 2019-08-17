#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tcp.hh"

#include "protocol-internal.hh"

void harness(tools tool, int pid);

void harness_all_tools(int pid) {
    harness(tools::gdb, pid);
}

void harness_this_process(tools tool) {
    harness(tool, getpid());
}

void harness_this_process_all_tools() {
    harness_this_process(tools::gdb);
}

void harness(tools tool, int pid) {
    int in_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "1025", 10);
    int out_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "1025", 10);
    int err_fd = aether::tcp::connect_to_host_port_with_timeout("127.1", "1025", 10);

    connect_message m {};
    m.tool_id = tool;
    m.pid = pid;
    m.stream_id = streams::in;  send(in_fd, &m, sizeof(m), 0);
    m.stream_id = streams::out; send(out_fd, &m, sizeof(m), 0);
    m.stream_id = streams::err; send(err_fd, &m, sizeof(m), 0);

    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);
    dup2(err_fd, STDERR_FILENO);

    close(in_fd);
    close(out_fd);
    close(err_fd);

    char pid_text[32];
    sprintf(pid_text, "%d", pid);

    char* tool_command;
    char** tool_args;
    char *gdb_args[] = {"gdb", "-q", "-p", pid_text, NULL};
    switch (tool) {
        case tools::gdb:
            tool_command = "gdb";
            tool_args = gdb_args;
            break;
    }
    execvp(tool_command, tool_args);
}
