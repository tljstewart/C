/** @file sfunctions.h
 *  @brief Function Prototypes for sfunctions.c
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


#define MAX_CONNECTIONS 5	/**<max connections stored via listen()*/
#define BYTES 4096		/**<bytes to produce*/
#define DEF_PORT "60099"	/**<default port to bind server to*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>


/** @brief initializes Server program, creates, bind, listen to Socket
 *
 *
 *	@param argc argument count on command-line
 *	@param argv argument values on command-line
 *	@param socketfd a pointer to file descriptor of sockets
 *	@param root root directory of server
 *  @return void
 */
void InitializeServer(int argc, char** argv,int *socketfd,char **rootDir);

/** @brief sends message via socket to client
 *
 *
 *	@param sock socket identifier
 *	@param root root directory of server
 *  @return void
 */
void ServerResponse(int sock,char * rootDir);


#endif
