/**
 * @file stub.h
 * @author Javier Izquierdo Hernandez (javizqh@gmail.com)
 * @brief stub de j.izquierdoh.2021@alumnos.urjc.es
 * @version 1.0
 * @date 2023-10-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// Obtiene el valor del reloj de lamport.
int get_clock_lamport();

/**
 * @brief Starts and loads all the networking libraries for the process
 * 
 * @param id Proccess id
 * @param ip
 * @param port 
 * @return int 0 = error, otherwise success
 */
int init_process(int id, const char *ip, const char *port);

/**
 * @brief The calling process warns P2 about its shutdown
 * 
 * @return int 0 = error, otherwise success
 */
int ready_to_shutdown();

/**
 * @brief Shuts down the calling process
 * 
 * @return int 0 = error, otherwise success
 */
int shutdown_proc();

/**
 * @brief Shuts down the process that corresponds with the passed id
 * 
 * @param id Process to shut down
 * @return int 0 = error, otherwise success
 */
int shutdown_to(int id);

/**
 * @brief Checks if the process supposed to be shutdown are shuted down
 * 
 * @return int 0 = error, otherwise success
 */
int is_all_shutdown();