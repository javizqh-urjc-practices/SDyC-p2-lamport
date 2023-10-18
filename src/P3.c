#include "stub.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHUTDOWN 9

void usage() {
    fprintf(stderr, "Usage: P3 <ip> <port>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000;

    argc--; argv++;

    if (argc != 2) {
        usage();
    }

    if (!init_process(3, argv[0], argv[1])) {
        fprintf(stderr, "The process could not be started\n");
    }

    ready_to_shutdown();
    while (get_clock_lamport() < SHUTDOWN) nanosleep(&sleep_time, &sleep_time);
    shutdown_proc();
    return 0;
}

