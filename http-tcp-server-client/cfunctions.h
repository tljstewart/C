/** @file cfunctions.h
 *  @brief Function Prototypes for cfunctions.c
 *
 *  this file contains function protoypes, data structures and global vars
 *  for cfunctions.c
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */


#ifndef cfunctions_h
#define cfunctions_h

#define BYTES 4096	/**<bytes to consume at a time*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

/** @brief initializes client program and creates Socket
 *
 *
 *  @param socketfd a pointer to file descriptor of sockets
 *  @param request two-dimensional array containing message to send
 *  @param argc argument count on command-line
 *  @param argv argument values on command-line
 *  @return int
 */
int InitializeClient(int *socketfd, char **request,int argc, char** argv);

/** @brief sends message via socket to Server
 *
 *
 *  @param socketfd a pointer to file descriptor of sockets
 *  @param request two-dimensional array containing message to send
 *  @return void
 */
void SendClientRequest(int socketfd,char *request);

#endif
