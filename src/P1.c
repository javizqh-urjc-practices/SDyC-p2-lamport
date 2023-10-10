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

    if (!init_network(1, argv[0], argv[1])) {
        // Socket error
    }

    send_message(2,READY_TO_SHUTDOWN);
    recv_message(2);
    while (get_clock_lamport() < 5) {
        continue;
    }
    if (get_message_info(2) != SHUTDOWN_NOW) {
        printf("Error\n");
    }
    send_message(2,SHUTDOWN_ACK);
    close_network(1);
    return 0;
}