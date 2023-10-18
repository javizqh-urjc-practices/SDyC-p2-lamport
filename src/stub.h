#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

// Obtiene el valor del reloj de lamport.
// Utilízalo cada vez que necesites consultar el tiempo.
// Esta función NO puede realizar ningún tiempo de comunicación (sockets)
int get_clock_lamport();    
int init_process(int id, const char *ip, const char *port);

// HDYUEGUYHNm
int ready_to_shutdown();
int shutdown_proc();
int wait_to_shutdown();
int shutdown_to(int id);
int is_all_shutdown();