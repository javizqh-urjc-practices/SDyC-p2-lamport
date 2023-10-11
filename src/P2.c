#include "stub.h"
#include <unistd.h>
#include <stdlib.h>

#define SEND_MSG_TO_ONE 3
#define SEND_MSG_TO_THREE 7
#define ALL_CLIENTS_CLOSED 11

void usage() {
    fprintf(stderr, "Usage: P1 <ip> <port>\n");
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

    while (get_clock_lamport() < SEND_MSG_TO_ONE) sleep(1);
    send_to_shutdown(1);
    while (get_clock_lamport() < SEND_MSG_TO_THREE) sleep(1);
    send_to_shutdown(3);
    while (get_clock_lamport() < ALL_CLIENTS_CLOSED) sleep(1);

    if (is_all_shutdown()) {
        printf("Los clientes fueron correctamente apagados en t(lamport) = %d\n", get_clock_lamport());
    }
    return 0;
}