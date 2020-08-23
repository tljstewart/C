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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BYTES 1024
#define CLEAR '-'
#define UPDATE 'U'
#define JOIN 'J'
#define LEAVE 'L'
#define LOCAL "127.0.0.1"
#define MSG_SIZE 5000


/* Struct*/
typedef struct{
    char action;
    int token;
    uint16_t port_self;
    uint16_t port_other;
}udp_msg;

/* Struct*/
typedef struct{
    char action;
    int msg_number;
    pthread_mutex_t busy;
    char msg[MSG_SIZE];
    char filename[100];
}local_msg;

/* enumerated list*/
typedef enum{
        WRITE,                                                  // 0
        READ,                                                   // 1
        LIST,                                                   // 2
        EXIT                                                    // 3
    }selected;

    /* GLOBAL VARIABLES */
    uint16_t NEIGHBOR;
    uint16_t MYPORT;
    int TOKEN;
    udp_msg NEXT_MSG;
    local_msg LOCAL_MSG;


 /** @brief initilization client
 *
 *  @param argv commandline arguemnts
 *  @param argc commandline arguemnts
 *  @param int socketfd UDP socket
 *  @return void
 */
void initSelf(int argc, char** argv,int *socketfd);


 /** @brief initilization with server
 *
 *  @param argv commandline arguemnts
 *  @param int socketfd UDP socket
 *  @return void
 */
void initRing(char **argv,int socketfd);


 /** @brief recieve udp message
 *
 *
 *  @param int socketfd UDP socket
 *  @return void
 */
void recieveMessage(int socketfd);


 /** @brief process message to neighbor
 *
 *  @param int socketfd UDP socket
 *  @return void
 */
void sendToNeighbor(int socketfd);

 /** @brief process message
 *
 *  @param int socketfd UDP socket
 *  @param new_msg to be received
 *  @return void
 */
void processMessage(int socketfd, udp_msg *new_msg);

 /** @brief process exiting peer
 *
 *
 *  @param int socketfd UDP socket
 *  @param new_msg to be received
 *  @return void
 */
void exitPeer(int socketfd, udp_msg *new_msg);


 /** @brief process new peer joining ring
 *
 *
 *  @param int socketfd UDP socket
 *  @param new_msg to be received
 *  @return void
 */
void newPeer(int socketfd, udp_msg *new_msg);


 /** @brief completes next UDP message
 *
 *
 *  @return void
 */
void unsetNext();


 /** @brief initilize and Mutex threads
 *
 *
 *  @param init initilization
 *  @return void
 */
void threadUnlock(int init);


 /** @brief prints user parameters
 *
 *
 *  @return void
 */
void printUserOptionsInit();

 /** @brief decode user input
 *
 *
 *  @return int
 */
int userInput(int flag);


 /** @brief prints user options
 *
 *
 *  @return int
 */
int printUserOptions();

 /** @brief select option
 *
 *
 *  @param option the option selected
 *  @return void
 */
void optionSelect(selected option);

 /** @brief exit ring
 *
 *
 *  @param socketfd UDP socket
 *  @return void
 */
void exitRing(int socketfd);

 /** @brief joins exsisting peer to peer ring
 *
 *
 *  @param argv server port
 *  @param socketfd UDP socket
 *  @return void
 */
void joinExistingRing(char **argv, int socketfd);

 /** @brief messages in the bulletin board
 *
 *
 *  @return int number of messages in bulletin
 */
int bulletinMessageCount();

 /** @brief Prompts user for message input
 *
 *  @return void
 */
void getNewBulletinMessage();

 /** @brief write to bulletin bulletinboard
 *
 *
 *  @return void
 */
void writeBulletin();

 /** @brief check and/or create bulletinboard file
 *
 *
 *  @return void
 */
void createBulletinFile();

 /** @brief prompt user for input option
 *
 *
 *  @return void
 */
void PromptUser();

 /** @brief Checks the arguments
 *
 *
 *
 *  @param argc argument Counts
 *  @param argv argument value
 *  @return void
 */
void argCheck(int argc, char **argv);


 /** @brief Reads a message from the bulletin
 *
 *
 *
 *  @param num a sequence number for message to read
 *  @return void
 */
void readBulletin(int num);



#endif
