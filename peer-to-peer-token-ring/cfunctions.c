/** @file cfunctions.c
 *  @brief client controls
 *
 *  this file contains the fucntions that control bbclient
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */

#include "cfunctions.h"

void initSelf(int argc, char** argv,int *socketfd)
{
  char port[6];
	struct addrinfo serv_criteria, *serv_addr, *temp_addr;
	memset(&serv_criteria, 0, sizeof(serv_criteria));

    unsetNext();
    threadUnlock(1);


	if(argc == 4)
		strcpy(port,argv[1]);
	else
		strcpy(port,argv[2]);

	serv_criteria.ai_family = AF_INET;
	serv_criteria.ai_socktype = SOCK_DGRAM;
	serv_criteria.ai_flags = AI_PASSIVE;

    MYPORT = htons(atoi(port));
	if (getaddrinfo(NULL, port, &serv_criteria, &temp_addr) != 0)
	{
		printf("Unable to get correct address\n");
		exit(1);
	}

	for (serv_addr = temp_addr; serv_addr!=NULL; serv_addr=serv_addr->ai_next)
	{
		*socketfd = socket (serv_addr->ai_family, serv_addr->ai_socktype, 0);
		if (*socketfd == -1){
			printf("Failed to create socket. Trying next address\n");
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
	printf("Client started on port number: %s\n",port);

	freeaddrinfo(temp_addr);

    if(argc == 5)
    {
        printf("Server Connection Established.\n");
        strcpy(LOCAL_MSG.filename,argv[4]);
        initRing(argv,*socketfd);
        createBulletinFile();
    }
    else
    {
        printf("Peer Connection Established.\n");
        strcpy(LOCAL_MSG.filename,argv[3]);
        joinExistingRing(argv,*socketfd);
    }
}


void initRing(char **argv,int socketfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    udp_msg msg_in;
    ssize_t bread;

    inet_pton(AF_INET, LOCAL, &client_addr.sin_addr);
    client_addr.sin_port = htons(atoi(argv[3]));
    client_addr.sin_family = AF_INET;
    client_addrlen = sizeof(client_addr);

    bread = sendto(socketfd, "Hi!", strlen("Hi!"), 0,(struct sockaddr *) &client_addr,client_addrlen);
    if (bread <= 0)
    {
        printf("Failed to send\n");
        exit(-1);
    }

    bread = recvfrom(socketfd, &msg_in, sizeof(msg_in), 0, (struct sockaddr *) &client_addr, &client_addrlen);
    if (bread <= 0)
    {
        printf("Failed to receive properly\n");
        exit(-1);
    }

    if(msg_in.token == 1){
        TOKEN = 1;
        printf("\nYay I Win, I have the first Token\n\n");
      }
    NEIGHBOR = msg_in.port_other;
}

void recieveMessage(int socketfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    udp_msg msg_in;
    ssize_t bytes_read;

    bytes_read = recvfrom(socketfd, &msg_in, sizeof(msg_in), 0, (struct sockaddr *) &client_addr, &client_addrlen);

    if(bytes_read <= 0)
    {
        printf("Error in received message\n");
        exit(-1);
    }
    processMessage(socketfd, &msg_in);
}

void sendToNeighbor(int socketfd)
{
  struct sockaddr_in client_addr;
  socklen_t client_addrlen;
  ssize_t bytes_sent;

  client_addr.sin_family = AF_INET;
  client_addr.sin_port = NEIGHBOR;
  inet_pton(AF_INET, LOCAL, &client_addr.sin_addr);
  client_addrlen = sizeof(client_addr);

  bytes_sent = sendto(socketfd, &NEXT_MSG, sizeof(NEXT_MSG), 0,(struct sockaddr *) &client_addr,client_addrlen);

  if(bytes_sent <= 0)
  {
    printf("Failed to send\n");
    exit(-1);
  }
}

void processMessage(int socketfd,udp_msg *new_msg)
{
    TOKEN = new_msg->token;
    switch(new_msg->action)
    {
        case '-':
          break;

        case 'U':
          NEIGHBOR = new_msg->port_other;
          unsetNext();
          break;

        case 'L':
          exitPeer(socketfd, new_msg);
          break;

        case 'J':
          newPeer(socketfd, new_msg);
          break;

        default:
            printf("Error parsing UDP message.\n");
  }
}

void exitPeer(int socketfd, udp_msg *new_msg)
{
    if(new_msg->port_self == NEIGHBOR)
        NEIGHBOR = new_msg->port_other;
    else
    {
        NEXT_MSG.action = new_msg->action;
        NEXT_MSG.port_self = new_msg->port_self;
        NEXT_MSG.port_other = new_msg->port_other;
        sendToNeighbor(socketfd);
    }
    unsetNext();
}

void newPeer(int socketfd, udp_msg *new_msg)
{
    do
    {
        recieveMessage(socketfd);
    }while(TOKEN != 1);
    NEXT_MSG.token = 1;
    NEXT_MSG.action = UPDATE;
    NEXT_MSG.port_other = NEIGHBOR;
    NEIGHBOR = new_msg->port_self;
    sendToNeighbor(socketfd);
    unsetNext();
}

void unsetNext(void)
{
    NEXT_MSG.token = 0;
    NEXT_MSG.action = CLEAR;
    NEXT_MSG.port_self = 0;
    NEXT_MSG.port_other = 0;
}
void threadUnlock(int init)
{
    LOCAL_MSG.action = CLEAR;
    LOCAL_MSG.msg_number = -1;
    strcpy(LOCAL_MSG.msg,"\0");
    if(init)
        pthread_mutex_init(&LOCAL_MSG.busy,NULL);
    else
        pthread_mutex_unlock(&LOCAL_MSG.busy);

}

void printUserOptionsInit(void)
{
    selected option;
    option = (selected) printUserOptions();
    while(option != EXIT)
    {
        pthread_mutex_trylock(&LOCAL_MSG.busy);
        optionSelect(option);
        option = (selected) printUserOptions();
    }
    optionSelect(EXIT);
    printf("Exiting...\n");
    pthread_exit(NULL);
}

int printUserOptions(void)
{
    int option;

    do {
        printf("[+]   Enter %i to Write to the bulletinboard.\n", WRITE);
        printf("[+]   Enter %i to Read from the bulletinboard.\n", READ);
        printf("[+]   Enter %i to Displays a range of messages.\n", LIST);
        printf("[+]   Enter %i to Exit the token ring.\n\n", EXIT);
        printf("Option (example, enter 0 then hit return): ");
        option = userInput(1);
    } while (option < WRITE || option > EXIT);
    return option;
}
int userInput(int flag)
{
        char buffer[10];
        char *p;
        long int i;
int j = 10;

       while(j){
		 buffer[j-1] = '\0';
		 --j;
	}


	if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
               i = strtol(buffer, &p, 10);

                if (buffer[0] != '\n' && (*p == '\n' || *p == '\0')
                                && (i >= WRITE && i <= EXIT) && flag)
                {
                        return (int) i;
                }

                else if(buffer[0] != '\n' && (*p == '\n' || *p == '\0'))
                {
                        return (int) i;
                }
        }
        return -1;
}

void optionSelect(selected option)
{
    switch(option)
    {
        case WRITE:
            getNewBulletinMessage();
            break;

        case READ:
            PromptUser();
            break;

        case LIST:
            LOCAL_MSG.action = LIST;
            pthread_mutex_lock(&LOCAL_MSG.busy);
            break;

        case EXIT:
            LOCAL_MSG.action = LEAVE;
            break;

        default:
            break;
    }
}
void exitRing(int socketfd)
{
    while(NEXT_MSG.action != CLEAR)
        recieveMessage(socketfd);

    NEXT_MSG.action = LEAVE;
    NEXT_MSG.token = 1;
    NEXT_MSG.port_self = MYPORT;
    NEXT_MSG.port_other = NEIGHBOR;
    sendToNeighbor(socketfd);
}
void joinExistingRing(char **argv, int socketfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    udp_msg msg_in;
    udp_msg msg_out;
    ssize_t bread;

    inet_pton(AF_INET, LOCAL, &client_addr.sin_addr);
    client_addr.sin_port = htons(atoi(argv[2]));
    client_addr.sin_family = AF_INET;
    client_addrlen = sizeof(client_addr);

    msg_out.action = JOIN;
    msg_out.token = 0;
    msg_out.port_self = MYPORT;
    msg_out.port_other = 0;

    bread = sendto(socketfd, &msg_out, sizeof(msg_out), 0,(struct sockaddr *) &client_addr,client_addrlen);
    if (bread <= 0)
    {
        printf("Failed to send\n");
        exit(-1);
    }

    bread = recvfrom(socketfd, &msg_in, sizeof(msg_in), 0, (struct sockaddr *) &client_addr, &client_addrlen);
    if (bread <= 0)
    {
        printf("Failed to receive\n");
        exit(-1);
    }

    if(msg_in.token == 1)
        TOKEN = 1;

    if(msg_in.action == UPDATE)
        NEIGHBOR = msg_in.port_other;
}
int bulletinMessageCount()
{

    FILE *fp;
    int ch,len,i,count = 0;
    char *word = "<message n=";

    if(NULL==(fp=fopen(LOCAL_MSG.filename, "r")))
    {
        printf("Error opening file\n");
        return -1;
    }
    len = strlen(word);
    while(1){
        if(EOF==(ch=fgetc(fp))) break;
        if((char)ch != *word) continue;
        for(i=1;i<len;++i){
            if(EOF==(ch = fgetc(fp))) goto end;
            if((char)ch != word[i]){
                fseek(fp, 1-i, SEEK_CUR);
                goto next;
            }
        }
        ++count;
        next: ;
    }
    end:
        fclose(fp);
    return count;
}

void getNewBulletinMessage(void)
{
    char buff[500];
    printf("[+]Enter your message below, when you are finish hit enter then ctrl-d for the end of file charater:\n");

    while(NULL != fgets(buff, 500,stdin))
    {
        strcat(LOCAL_MSG.msg,buff);
    }

    fflush(stdin);
    fflush(stdout);
	ungetc(0,stdin);
	getchar();
    LOCAL_MSG.action = WRITE;
    pthread_mutex_lock(&LOCAL_MSG.busy);
}
void writeBulletin(void)
{


    FILE *file = fopen(LOCAL_MSG.filename, "a");
    int cntr = bulletinMessageCount(),
        charsToDelete,
        position;

    cntr++;

    if(cntr == 1) {
        fprintf(file, "<messages>");
        fprintf(file, "<message n=\"%i\">",cntr);
    }
    else {
        charsToDelete = 12;
        fseeko(file,-charsToDelete,SEEK_END);
        position = ftello(file);
        ftruncate(fileno(file), position);
        fprintf(file, "<message n=\"%i\">",cntr);
    }
    fprintf(file, "%s",LOCAL_MSG.msg);
    fprintf(file, "</message>");
    fprintf(file, "</messages>\n");
    fclose(file);
    threadUnlock(0);
}
void createBulletinFile(void)
{
    FILE *fp = NULL;
    if(access(LOCAL_MSG.filename, F_OK) != 0)
    {
        if((fp = fopen(LOCAL_MSG.filename, "w")) == NULL)
        {
            printf("Unable to create the file %s",LOCAL_MSG.filename);
        }
    }
    if(fp != NULL)
        fclose(fp);
}
void PromptUser(void)
{
    while(LOCAL_MSG.msg_number == -1)
    {
        fprintf(stdout,"Message number to read:");
        LOCAL_MSG.msg_number = userInput(0);
    }
    LOCAL_MSG.action = READ;
    pthread_mutex_lock(&LOCAL_MSG.busy);
}

void argCheck(int argc, char **argv)
{
    if(argc < 4 || argc >= 6)
    {
        printf("Usage: %s [-new] <PortSelf> <PortOther> <BulletinBoardFileName>\noptional: [-new]\n",argv[0]);
        exit(-1);
    }
    if(argc == 5)
    {
        if(strcmp("-new",argv[1]))
        {
            printf("Did you mean -new?.\n");
            exit(-1);
        }
        if((atoi(argv[2]) > 65535 || atoi(argv[2]) <= 0) || (atoi(argv[3]) > 65535 || atoi(argv[3]) <= 0))
        {
            printf("Ports greater than 60000 Please.\n");
            exit(-1);
        }
        if(strlen(argv[4]) > 99)
        {
            printf("The path is too long.\n");
            exit(-1);
        }
        if(!strcmp(argv[2],argv[3]))
        {
            printf("Ports must be different\n");
            exit(-1);
        }
    }
    else
    {
        if((atoi(argv[1]) > 65535 || atoi(argv[1]) <= 0) || (atoi(argv[2]) > 65535 || atoi(argv[2]) <= 0))
        {
            printf("The ports are wrong.\n");
            exit(-1);
        }
        if(strlen(argv[3]) > 99)
        {
            printf("The path is too long.\n");
            exit(-1);
        }
        if(!strcmp(argv[1],argv[2]))
        {
            printf("Can't connect to myself. Ports must be different\n");
            exit(-1);
        }
    }

}
void readBulletin(int num)
{
    char msg_buffer[5000] = "\0";
    FILE *fp;
    int ch,len,i,count = 0;
    char *word = "<message n=";
    char *word2= "</message>";
    char *word3= ">";

    if(NULL==(fp=fopen(LOCAL_MSG.filename, "r")))
    {
        printf("Error opening file\n");
        return;
    }
    len = strlen(word);
    while(1){
        if(EOF==(ch=fgetc(fp))) break;
        if((char)ch != *word) continue;
        for(i=1;i<len;++i){
            if(EOF==(ch = fgetc(fp))) goto end;
            if((char)ch != word[i]){
                fseek(fp, 1-i, SEEK_CUR);
                goto next;
            }
        }
        if (num == ++count)
            break;
        next: ;
    }

    do
    {
        if(EOF==(ch=fgetc(fp)))
            goto end;
    }while(*word3 != (char)(ch = fgetc(fp)));
        len = strlen(word2);
        while(1)
        {
            if(EOF==(ch=fgetc(fp)))
                break;
            if((char)ch != *word2)
            {
                strcat(msg_buffer,(char*)&ch);
                continue;
            }
            for(i = 1; i < len; ++i)
            {
                if(EOF==(ch = fgetc(fp)))
                    goto end;
                if((char)ch != word2[i])
                {
                    fseek(fp, 1-i, SEEK_CUR);
                    break;
                }
                if(i == len -1)
                    goto end;
            }
        }

    end:
        fclose(fp);
    if(!strcmp(msg_buffer,"\0"))
        printf("Message number %d NOT FOUND!\n",num);
    else
    {
        printf("The message is:\n\t%s\n|end of message|\n",msg_buffer);
    }
    threadUnlock(0);
}
