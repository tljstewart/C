/** @file sfunctions.c
 *  @brief client controls
 *
 *  this file contains the fucntions that control bbserver
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */
#include "sfunctions.h"



void initSelf(int argc, char** argv,int *socketfd)
{
    char port[6];
    struct addrinfo serv_criteria, *serv_addr, *temp_addr;
    memset(&serv_criteria, 0, sizeof(serv_criteria));

    if (argc < 3)
    {
        printf("Usage: %s <portNum> <numberHosts>\n",argv[0]);
        exit(-1);
    }
    else
       strcpy(port,argv[1]);
    serv_criteria.ai_family = AF_INET;
    serv_criteria.ai_socktype = SOCK_DGRAM;
    serv_criteria.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &serv_criteria, &temp_addr) != 0)
    {
        printf("Unable to get correct address\n");
        exit(-1);
    }

    for (serv_addr = temp_addr; serv_addr!=NULL; serv_addr=serv_addr->ai_next)
    {
        *socketfd = socket (serv_addr->ai_family, serv_addr->ai_socktype, 0);
        if (*socketfd == -1){
            printf("Failed to create socket with the chosen address. Trying next address\n");
            continue;
        }
        if (bind(*socketfd, serv_addr->ai_addr, serv_addr->ai_addrlen) != 0)
            printf("Failed to bind socket on current address\n");
        else
            break;
    }

    if (serv_addr == NULL)
    {
        printf("Unable to find an address to create a socket and bind\n");
        exit(-1);
    }

    printf("Server started on port number: %s\n",port);
    printf("Waiting on %s peers to form bulletinboard ring...\n",argv[2]);
    freeaddrinfo(temp_addr);
}
void processClients(char  **argv,int *socketfd, int *clients, uint16_t **port_list)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    char buff[BYTES];
    int waiting_on,
        client = 0;
    ssize_t bytes_read;

    waiting_on = *clients = atoi(argv[2]);
    *port_list = malloc(waiting_on * sizeof(uint16_t));

    while(client < waiting_on)
    {
        client_addrlen = sizeof(struct sockaddr_in);
        bytes_read = recvfrom(*socketfd, buff, BYTES, 0, (struct sockaddr *) &client_addr, &client_addrlen);
        if (bytes_read == -1)
            continue;
        (*port_list)[client] = client_addr.sin_port;
        client++;
    }
}
void createRing(int socketfd, int clients, uint16_t *port_list)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    udp_msg out_msg;
    int count;
    ssize_t bytes_sent;


    inet_pton(AF_INET, LOCAL, &client_addr.sin_addr);
    client_addr.sin_family = AF_INET;

    for (count = 0; count < clients; count++)
    {
        client_addr.sin_port = port_list[count];
        client_addrlen = sizeof(client_addr);
        if(count != clients-1)
        {
            out_msg.action = 'U';
            out_msg.token = 0;
            out_msg.port_other = port_list[count+1];
            out_msg.port_self = 0;

            bytes_sent = sendto(socketfd, &out_msg, sizeof(out_msg), 0,(struct sockaddr *) &client_addr,client_addrlen);
            if (bytes_sent <= 0)
            {
                fprintf(stderr, "Error sending response\n");
                exit(-1);
            }
        }
        else
        {
            out_msg.action = 'U';
            out_msg.token = 1;
            out_msg.port_other = port_list[0];
            out_msg.port_self = 0;

            bytes_sent = sendto(socketfd, &out_msg, sizeof(out_msg), 0,(struct sockaddr *) &client_addr,client_addrlen);
            if (bytes_sent == 0)
            {
                fprintf(stderr, "Error sending response\n");
                exit(-1);
            }
        }

    }
}
