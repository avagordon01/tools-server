#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "harness.hh"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s pid0 pid1 ...\n", argv[0]);
        return 1;
    }
    for (size_t i = 1; i < argc; i++) {
        uint64_t pid = atoi(argv[i]);
        harness_start_pid(pid);
    }

    waitpid(-1, NULL, 0);
}
