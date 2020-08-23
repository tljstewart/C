/** @file sfunctions.h
 *  @brief Function Prototypes for cfunctions.c
 *
 *  this file contains function protoypes, data structures and global vars
 *  for sfunctions.c
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */
#ifndef sfunctions_h
#define sfunctions_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>


#define BYTES 1024
#define DEF_PORT "60023"
#define UPDATE 'U'
#define JOIN 'J'
#define LEAVE 'L'
#define LOCAL "127.0.0.1"

/* Struct*/
typedef struct{
    char action;
    int token;
    uint16_t port_self;
    uint16_t port_other;
}udp_msg;



 /** @brief initilization server
 *
 *  @param argv commandline arguemnts
 *  @param argc commandline arguemnts
 *  @param int socketfd UDP socket
 *  @return void
 */
void initSelf(int argc, char** argv,int *socketfd);


 /** @brief listen for each peer to form ring with clients
 *
 *  @param argv commandline arguemnts
 *  @param clients the peers to be put in the ring
 *  @param int socketfd UDP socket
 *  @param port_list all peer ports
 *  @return void
 */
void processClients(char  **argv, int *socketfd, int *clients, uint16_t **port_list);


 /** @brief gives each peer its neighbors port and ip to form ring with clients
 *
 *  @param clients the peers to be put in the ring
 *  @param int socketfd UDP socket
 *  @param port_list all peer ports
 *  @return void
 */
void createRing(int socketfd, int clients, uint16_t *port_list);
#endif
