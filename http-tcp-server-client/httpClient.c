/** @file httpClient.c
 *  @brief a simple http client
 *
 *  this file contains the entry point for httpClient, main()
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */

#include "cfunctions.h"


int main(int argc,char** argv){

    int socketfd;
    char *request;
    char prompt;

    while(1)
    {
        if (InitializeClient(&socketfd,&request,argc,argv))
            SendClientRequest(socketfd,request);
            printf("\n\nCreate another Socekt Connection? [y/n]:");
            scanf(" %s",&prompt);
        if (strcmp(&prompt,"y")) 
            exit(0);
            argc = 1;
    };

return 0;                                                    
}
