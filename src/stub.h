#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define MESSAGE_MAX_SIZE 20


enum operations {
    READY_TO_SHUTDOWN = 0,
    SHUTDOWN_NOW,
    SHUTDOWN_ACK
};

// Obtiene el valor del reloj de lamport.
// Utilízalo cada vez que necesites consultar el tiempo.
// Esta función NO puede realizar ningún tiempo de comunicación (sockets)
int get_clock_lamport();    
//Sockets
int init_process(int id, const char *ip, const char *port);
int close_network(int id);

int send_message(int dest_id, enum operations action);
int recv_message(int dest_id);
enum operations get_message_info(int dest_id);

// HDYUEGUYHNm
int ready_to_shutdown();
int shutdown_proc();
int wait_to_shutdown();
int send_to_shutdown(int id);
int is_all_shutdown();