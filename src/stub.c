#include "stub.h"
#include <err.h>
#include <unistd.h>

#define SERVER_ID 2
#define N_CLIENTS 2

// ----------- Private functions and structs ------------
int proc_id;
int socket_fd;
pthread_t recv_thread;
int lamport_clock = 0;
int n_threads = 0;
enum operations curr_operation;

// Info to manage multiple connections from server
struct client_info {
    int id;
    int is_real_id;
    int socket_fd;
    pthread_t recv_thread;
    enum operations curr_operation;
};

struct client_info server_fd[N_CLIENTS];
// ------------------------------------------------------

struct message {
    char origin[20];
    enum operations action;
    unsigned int clock_lamport;
};

enum socket_operation {
    SEND,
    RECEIVE
};

void update_clock_lamport(unsigned int clock_lamport);
uint16_t get_port_from_string(const char *input);
int init_client(int sockfd, struct sockaddr *__addr, socklen_t __len);
int init_server(int sockfd, struct sockaddr *__addr, socklen_t __len);
void * recv_message_thread(void *arg);
void print_info(char message[20], enum socket_operation operation, int action);
// -------------- End of private functions --------------


int get_clock_lamport() {
    return lamport_clock;
}

int init_network(int id, const char *ip, const char *port_in) {
    struct in_addr addr;
    struct sockaddr_in servaddr;
    int tcp_socket;
    uint16_t port;
    socklen_t len;

    proc_id = id;

    port = get_port_from_string(port_in);

    if (inet_pton(AF_INET, ip, &addr) < 0) {
        return 0;
    }

    setbuf(stdout, NULL);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = addr.s_addr;
    servaddr.sin_port = htons(port);

    len = sizeof(servaddr);

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcp_socket < 0) {
        fprintf(stderr, "Socket failed\n");
        return 0;
    }
    printf("Socket successfully created...\n");

    if (id == SERVER_ID) {
        socket_fd = init_server(tcp_socket, (struct sockaddr *)&servaddr, len);
    } else {
        socket_fd = init_client(tcp_socket, (struct sockaddr *)&servaddr, len);
    }

    return socket_fd > 0;
}

int close_network(int id) {
    // TODO: check for threads
    if (id == SERVER_ID) {
        for (int i = 0; i < N_CLIENTS; i++) {
            if (close (server_fd[i].socket_fd) < 0) {
                fprintf(stderr, "Failed to close network\n");
                return 0;
            }
        }
    } else {
        if (close (socket_fd) < 0) {
            fprintf(stderr, "Failed to close network\n");
            return 0;
        }
    }
    printf("Network connection closed\n");
    return 1;
}

int send_message(int id, enum operations action) {
    ssize_t bytes_sent;
    struct message msg;

    msg.action = action;
    msg.origin[0] = 'P';
    msg.origin[1] = 48 + proc_id;
    msg.origin[2] = '\0';
    lamport_clock ++;
    msg.clock_lamport = lamport_clock;
    print_info(msg.origin, SEND, msg.action);

    if (proc_id == SERVER_ID) {
        for (size_t i = 0; i < N_CLIENTS; i++) {
            if (server_fd[i].id == id) {
                bytes_sent = send(server_fd[i].socket_fd, &msg, sizeof(msg), MSG_WAITALL);
                break;
            }
        }
    } else {
        bytes_sent = send(socket_fd, &msg, sizeof(msg), MSG_WAITALL);
    }
    if (bytes_sent < 0) {
        fprintf(stderr, "send failed");
    }

    return bytes_sent;
}

int recv_message(int dest_id) {
// Create a thread for each one
    //if (n_threads != 0) {
    //    fprintf(stderr, "Waiting data to be recover: use get_message_info(dest_id).\n");
    //    return 0;
    //}

    n_threads++;
    if (n_threads > N_CLIENTS) {
        fprintf(stderr, "More threads than clients connected\n");
        return 0;
    }

    if (proc_id == SERVER_ID) {
        // Get corresponding socket_fd from server_fd, if no ids wait for response to load
        for (size_t i = 0; i < N_CLIENTS; i++) {
            if (server_fd[i].id == dest_id) {
                server_fd[i].is_real_id = 1;
                pthread_create(&server_fd[i].recv_thread, NULL, recv_message_thread, (void *) &server_fd[i].id);
                return 1;
            }
        }
        // At the beggining we do not know the order, so we wait for both, 
        // we suppose that client_id 1 is server_id[0], if we fail 
        // as we need to listen for both, then later we will
        // have it correct for when it matters
        if (dest_id == 1) {
            server_fd[0].id = dest_id;
            pthread_create(&server_fd[0].recv_thread, NULL, recv_message_thread, (void *) &server_fd[0].id);
        } else {
            server_fd[1].id = dest_id;
            pthread_create(&server_fd[1].recv_thread, NULL, recv_message_thread, (void *) &server_fd[1].id);
        }
    } else {
        pthread_create(&recv_thread, NULL, recv_message_thread, (void *) &dest_id);
    }

    return 1;
}

enum operations get_message_info(int dest_id) {
    struct message msg;
    void * out_data;
    if (proc_id == SERVER_ID) {
        for (size_t i = 0; i < N_CLIENTS; i++) {
            if (server_fd[i].id == dest_id) {
               pthread_join(server_fd[i].recv_thread ,&out_data);
               n_threads--;
               return server_fd[i].curr_operation;
            }
        }
    } else {
        pthread_join(recv_thread ,&out_data);
        n_threads--;
        return curr_operation;
    }
}

// Private functions
void update_clock_lamport(unsigned int clock_lamport) {
    if (clock_lamport > lamport_clock) {
        lamport_clock = clock_lamport + 1;
    } else {
        lamport_clock++;
    }
}

int init_client(int sockfd, struct sockaddr *__addr, socklen_t __len) {
    if (connect(sockfd, __addr, __len) < 0){
        fprintf(stderr, "Unable to connect.\n");
        return 0;
    }
    sleep(3);
    printf("Connected to the server...\n");
    return sockfd;
}

int init_server(int sockfd, struct sockaddr *__addr, socklen_t __len) {
    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        &enable, sizeof(int)) < 0) {
        fprintf(stderr, "Setsockopt(SO_REUSEADDR) failed\n");
        return 0;
    }

    if (bind(sockfd, __addr, __len) < 0) {
        fprintf(stderr, "Bind failed\n");
        return 0;
    }

    printf("Socket successfully binded...\n");

    if (listen(sockfd, 5) < 0) {
        fprintf(stderr, "Listen failed\n");
        return 0;
    }
    printf("Server listening...\n");

    // Wait for both clients to connect
    for (int i = 0; i < N_CLIENTS; i++) {
        if ((server_fd[i].socket_fd = accept(sockfd, __addr, &__len)) < 0) {
            fprintf(stderr, "Accept failed\n");
            return 0;
        }
        server_fd[i].is_real_id = 0;
    }

    return sockfd;
}

uint16_t get_port_from_string(const char *input) {
    char *end;
    long val;
    
    val = strtol(input, &end, 10);
    if (end == input || *end != '\0' || val < 0 || val >= 0x10000) {
        return 0;
    }
    return (uint16_t)val;
}

void * recv_message_thread(void *arg) {
    // TODO: try to pass valgrind
    ssize_t bytes_recv;
    struct message msg;
    int dest_id = *(int *)arg;

    if (proc_id == SERVER_ID) {
        // Get corresponding socket_fd from server_fd, if no ids wait for response to load
        for (size_t i = 0; i < N_CLIENTS; i++) {
            if (server_fd[i].is_real_id && server_fd[i].id == dest_id) {
                bytes_recv = recv(server_fd[i].socket_fd, &msg, sizeof(msg), MSG_WAITALL);
                server_fd[i].curr_operation = msg.action;
                update_clock_lamport(msg.clock_lamport);
                print_info(msg.origin, RECEIVE, msg.action);
                pthread_exit(NULL);
            }
        }
        // At the beggining we do not know the order, so we wait for both, 
        // we suppose that client_id 1 is server_id[0], if we fail 
        // as we need to listen for both, then later we will
        // have it correct for when it matters
        if (!server_fd[0].is_real_id && dest_id == 1) {
            bytes_recv = recv(server_fd[0].socket_fd, &msg, sizeof(msg), MSG_WAITALL);
            server_fd[0].id = msg.origin[1] - 48;
            printf("listening id0: %d\n", server_fd[0].id);
            server_fd[0].curr_operation = msg.action;
        } else if (!server_fd[1].is_real_id) {
            bytes_recv = recv(server_fd[1].socket_fd, &msg, sizeof(msg), MSG_WAITALL);
            server_fd[1].id = msg.origin[1] - 48;
            printf("listening id1: %d\n", server_fd[1].id);
            server_fd[1].curr_operation = msg.action;
        }
    } else {
        bytes_recv = recv(socket_fd, &msg, sizeof(msg), MSG_WAITALL);
        curr_operation = msg.action;
    }

    if (bytes_recv < 0) {
        fprintf(stderr, "receive failed");
    } else {
        update_clock_lamport(msg.clock_lamport);
        print_info(msg.origin, RECEIVE, msg.action);
    }

    pthread_exit(NULL);
}

void print_info(char message[20], enum socket_operation operation, int action) {
    switch (operation) {
    case SEND:
        printf("%s, %d, SEND, ", message, lamport_clock);
        break;
    case RECEIVE:
        printf("P%d, %d, RECV (%s), ", proc_id, lamport_clock, message);
        break;
    default:
        printf("NOT SUPPORTED OPERATION, ");
        break;
    }
    switch (action) {
    case READY_TO_SHUTDOWN:
        printf("READY_TO_SHUTDOWN\n");
        break;
    case RECEIVE:
        printf("SHUTDOWN_NOW\n");
        break;
    default:
        printf("SHUTDOWN_ACK\n");
        break;
    }
}

// End of private functions