#include <stdio.h>
#include <stdlib.h>

#include "harness.hh"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s pid\n", argv[0]);
        return 1;
    }
    uint64_t pid = atoi(argv[1]);
    harness(tools::gdb, pid);
}
