/** @file sfunctions.c
 *  @brief httpServer controls
 *
 *  this file contains the fucntions that control httpServer
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */

#include "sfunctions.h"

void InitializeServer(int argc, char** argv,int *socketfd,char **rootDir)
{
	char port[6];
	struct addrinfo serv_criteria, *serv_addr, *temp_addr;
	memset(&serv_criteria, 0, sizeof(serv_criteria));

  *rootDir = calloc(strlen(getenv("PWD")),1);
	*rootDir = getenv("PWD");

	if(argc == 2)
		strcpy(port,argv[1]);
	else
		strcpy(port,DEF_PORT);

	serv_criteria.ai_family = AF_INET;
	serv_criteria.ai_socktype = SOCK_STREAM;
	serv_criteria.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &serv_criteria, &temp_addr) != 0)
	{
		printf("Unable to get correct address\n");
		exit(1);
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
		exit(1);
	}

	if ( listen (*socketfd, MAX_CONNECTIONS) != 0 )
	{
		printf("Unable to start listening\n");
		exit(1);
	}

	printf("\n Server started:\n\n Using Port: %s \n\n Server Root Directory: %s.\n\n Awaiting connections...\n",port,*rootDir);
	freeaddrinfo(temp_addr);
}

void ServerResponse(int sock,char *rootDir)
{
  struct stat fileSize;
  char* sendHeader;
	char msg[99999], *reqline[3], data_to_send[BYTES], path[99999];
	int rcv_flag, file, bytes_read;
	strcpy(path, rootDir);
	char* textheader = "Content-Type: text/html; charset=UTF-8\n";
	char* notfoundmsg = "<!DOCTYPE html><h1><b>Error 404</b></h1><p> File not found.</p>\n";
  char* okmsg = "HTTP/1.1 200 OK\n";

	memset((void*)msg, (int)'\0', 99999 );

	rcv_flag=recv(sock, msg, 99999, 0);

	if (rcv_flag<0)
		printf("Receive error\n");
	else if (rcv_flag==0)
		fprintf(stderr,"Client disconnected upexpectedly.\n");
	else
	{
		printf("\nNew Connection with Request Message:\n%s\n\n", msg);
		reqline[0] = strtok (msg, " \t\n");
		if (strncmp(reqline[0], "GET\0", 4) == 0)
		{
			reqline[1] = strtok (NULL, " \t");
			reqline[2] = strtok (NULL, " \t\n");
			if (strncmp(reqline[2], "HTTP/1.1", 8) != 0)
			{
                printf("Invalid request made\n");
				write(sock, "HTTP/1.1 400 Bad Request\n", 25);
			}
			else
			{
				if ( strncmp(reqline[1], "/\0", 2)==0)
					reqline[1] = "/index.html";

				strcat(path,reqline[1]);

				if ( (file=open(path, O_RDONLY))!=-1)
				{
                    stat(path, &fileSize);
                    sendHeader = calloc(strlen(okmsg)+strlen("Content-Length: \n\n")+sizeof(fileSize.st_size),1);
                    sprintf(sendHeader,"%sContent-Length: %d\n\n",okmsg,(int)fileSize.st_size);
     				printf("Sending File: %s\n", path);
                    send(sock,sendHeader,strlen(sendHeader),0);
					while ((bytes_read = read(file, data_to_send, BYTES)) > 0)
						write (sock, data_to_send, bytes_read);
                    free(sendHeader);
				}
				else{
                    printf("Requested file not found\n");
					write(sock, "HTTP/1.1 404 Not Found\n", 23);
					write(sock, textheader, strlen(textheader));
                    sendHeader = calloc(strlen("Content-Length: \n\n")+ strlen(notfoundmsg),1);
                    sprintf(sendHeader,"Content-Length: %ld\n\n",strlen(notfoundmsg));
                    write(sock,sendHeader,strlen(sendHeader));
					write(sock, notfoundmsg, strlen(notfoundmsg));
                    free(sendHeader);
				}
			}
		}
	}
	close(sock);
	printf("File sent, socket closed\n");
}
