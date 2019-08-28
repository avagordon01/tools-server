#include <sys/wait.h>

#include "server.hh"

int main() {
    server_start();
    waitpid(-1, NULL, 0);
}
