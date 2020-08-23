/** @file cfunctions.c
 *  @brief client controls
 *
 *  this file contains the fucntions that control httpClient
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */

#include "cfunctions.h"


int InitializeClient(int *socketfd, char **request,int argc, char** argv)
{
		struct addrinfo client_criteria, *client_addr, *temp_addr;
    memset(&client_criteria, 0, sizeof(client_criteria));

    char userInput[100];
    char path[100];
    char *address[2];

    if (argc != 3)
    {
        printf("HTTP Client\nUsage:<address:port> <path>\nExample: 127.0.0.1:60099 /images/yesimage.jpg\n:");
        scanf("%s %s",userInput,path);
        address[0] = strtok(userInput,":");
        address[1] = strtok(NULL," ");
    }
    else
    {
        address[0] = strtok(argv[1],":");
        address[1] = strtok(NULL," ");
        if (sizeof(path) >= sizeof(argv[2])){
            strcpy(path,argv[2]);
            strcat(path,"\0");
        }
        else
        {
            printf("The Input Path has Exceeded Max Path Length. Exiting.\n");
            return 0;
        }
    }

    printf("Connecting Socket to %s on port %s and retrieve %s\n",address[0],address[1],path);
    *request = calloc(strlen("GET ")+strlen(path)+strlen("HTTP/1.1\n\n  "),1);
    sprintf(*request,"GET %s HTTP/1.1\r\n\r\n",path);

    client_criteria.ai_family = AF_INET;
    client_criteria.ai_socktype = SOCK_STREAM;
    client_criteria.ai_flags = 0;

    if (getaddrinfo(address[0],address[1],&client_criteria,&temp_addr) != 0)
    {
        printf("Address Information Incorrect\n");
        return 0;
    }
    for (client_addr = temp_addr; client_addr != NULL; client_addr = client_addr->ai_next)
    {
        *socketfd = socket(client_addr->ai_family, client_addr->ai_socktype, 0);
        if (*socketfd == -1)
        {
            printf("Failed to create socket with chosen address. Trying next address\n");
            continue;
        }
      
        if (connect(*socketfd,client_addr->ai_addr, client_addr->ai_addrlen) != -1)
        {
        printf("Successful Socket Connection\n");
        break;
        }
    }
    if (client_addr == NULL)
    {
        printf("Unable to connect to remote host\n");
        return 0;
    }
    freeaddrinfo(temp_addr);
    return 1;
}

void SendClientRequest(int socketfd,char *request)
{
    int bytes_consumed = 1;
    char receive_buff[999999];

    if (write(socketfd,request,strlen(request)) > 0)
    printf("Request Sent\n");
    shutdown(socketfd,SHUT_WR);
    while (bytes_consumed > 0)
    {
        bytes_consumed = read(socketfd,receive_buff,BYTES);
        write(1,receive_buff,bytes_consumed);
    }
    printf("\n");
    close(socketfd);
}
