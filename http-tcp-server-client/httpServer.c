/** @file httpServer.c
 *  @brief a simple http Server
 *
 *  this file contains the entry point for httpServer, main()
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */

#include "sfunctions.h"


int main(int argc, char* argv[])
{
    char *rootDir;
    int socketfd;
	  struct sockaddr_in clientaddr;
	  socklen_t addrlen;
	  InitializeServer(argc,argv,&socketfd,&rootDir);

	while (1)
	{
		ServerResponse(accept (socketfd, (struct sockaddr *) &clientaddr, &addrlen),rootDir);
	}

return 0;
}
