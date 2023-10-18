/**
 * @file P2.c
 * @author Javier Izquierdo Hernandez (javizqh@gmail.com)
 * @brief P2 de j.izquierdoh.2021@alumnos.urjc.es
 * @version 1.0
 * @date 2023-10-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
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
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000;
    argc--; argv++;

    if (argc != 2) {
        usage();
    }

    if (!init_process(2, argv[0], argv[1])) {
        fprintf(stderr, "The process could not be started\n");
    }

    while (get_clock_lamport() < SHUTDOWN_ONE) {
        nanosleep(&sleep_time, &sleep_time);
    }
    shutdown_to(1);
    while (get_clock_lamport() < SHUTDOWN_THREE) {
        nanosleep(&sleep_time, &sleep_time);
    }
    shutdown_to(3);
    while (get_clock_lamport() < ALL_SHUTDOWN) {
        nanosleep(&sleep_time, &sleep_time);
    }

    if (is_all_shutdown()) {
        printf("Los clientes fueron correctamente apagados en t(lamport) = %d\n"
               , get_clock_lamport());
    }
    return 0;
}