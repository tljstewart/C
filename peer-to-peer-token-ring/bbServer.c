/** @file bbserver.c
 *  @brief a simple Server
 *
 *  this file contains the entry point for bbserver, main()
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */
#include "sfunctions.h"

int main(int argc, char **argv)
{
    int peers = 0;
    int s;
    uint16_t *peerPorts = NULL;


    initSelf(argc,argv,&s);
    processClients(argv,&s,&peers,&peerPorts);
    createRing(s,peers,peerPorts);
    printf("Shuting Down Gracefully...\n");
    return 0;
}
