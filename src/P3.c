#include "stub.h"
#include <unistd.h>
#include <stdlib.h>

#define SEND_MSG_TO_TWO 9

void usage() {
    fprintf(stderr, "Usage: P1 <ip> <port>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    argc--; argv++;

    if (argc != 2) {
        usage();
    }

    if (!init_process(3, argv[0], argv[1])) {
        fprintf(stderr, "The process could not be started\n");
    }

    ready_to_shutdown();
    while (get_clock_lamport() < SEND_MSG_TO_TWO) sleep(1);
    shutdown_proc();
    return 0;
}

