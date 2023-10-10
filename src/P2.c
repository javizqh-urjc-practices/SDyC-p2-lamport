#include "stub.h"
#include <unistd.h>
#include <stdlib.h>

void usage() {
    fprintf(stderr, "Usage: P1 <ip> <port>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    argc--; argv++;

    if (argc != 2) {
        usage();
    }

    if (!init_network(2, argv[0], argv[1])) {
        // Socket error
    }

    recv_message(1);
    recv_message(3);
    while (get_clock_lamport() < 3) {
        continue;
    }
    if (get_message_info(1) == READY_TO_SHUTDOWN) {
        printf("Correct 1\n");
    }
    if (get_message_info(3) == READY_TO_SHUTDOWN) {
        printf("Correct 3\n");
    }
    send_message(1,SHUTDOWN_NOW);
    recv_message(1);
    while (get_clock_lamport() < 7) {
        continue;
    }
    if (get_message_info(1) == SHUTDOWN_ACK) {
        printf("Correct 2\n");
    }
    send_message(3, SHUTDOWN_NOW);
    recv_message(3);
    while (get_clock_lamport() < 11) {
        continue;
    }
    if (get_message_info(3) != SHUTDOWN_ACK) {
        printf("Correct 4\n");
    }
    printf("Los clientes fueron correctamente apagados en t(lamport) = %d\n", get_clock_lamport());
    close_network(2);
    return 0;
}