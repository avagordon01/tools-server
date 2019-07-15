#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int tool_harness(char tool[], char *tool_args[], int in_fd, int out_fd, int err_fd) {
    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);
    dup2(err_fd, STDOUT_FILENO);

    close(in_fd);
    close(out_fd);
    close(err_fd);

    execvp(tool, tool_args);
}

int main() {
    char tool[] = "gdb";
    char *tool_args[] = {"gdb", "-p", "4733", "-ex", "c", NULL};

    int in_fd = open(STDIN_FILENO, O_RDONLY);
    int out_fd = open("out", O_WRONLY | O_TRUNC);
    int err_fd = open("err", O_WRONLY | O_TRUNC);

    tool_harness(tool, tool_args, in_fd, out_fd, err_fd);
}
