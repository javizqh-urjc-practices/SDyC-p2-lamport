#include "stub.h"
#include <unistd.h>
#include <stdlib.h>

#define SHUTDOWN_ONE 3
#define SHUTDOWN_THREE 7
#define ALL_SHUTDOWN 11

void usage() {
    fprintf(stderr, "Usage: P2 <ip> <port>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    argc--; argv++;

    if (argc != 2) {
        usage();
    }

    if (!init_process(2, argv[0], argv[1])) {
        fprintf(stderr, "The process could not be started\n");
    }

    while (get_clock_lamport() < SHUTDOWN_ONE) sleep(1);
    shutdown_to(1);
    while (get_clock_lamport() < SHUTDOWN_THREE) sleep(1);
    shutdown_to(3);
    while (get_clock_lamport() < ALL_SHUTDOWN) sleep(1);

    if (is_all_shutdown()) {
        printf("Los clientes fueron correctamente apagados en t(lamport) = %d\n", get_clock_lamport());
    }
    return 0;
}