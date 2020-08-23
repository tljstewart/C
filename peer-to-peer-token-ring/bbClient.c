/** @file bbclient.c
 *  @brief a simple token passing client
 *
 *  this file contains the entry point for bbclient, main()
 *  Copyright (c) 2018 All rights reserved.
 *
 *  @author Timothy L.J. Stewart
 *  @bug No Known Bugs
 */
#include "cfunctions.h"

int main(int argc, char* argv[])
{
    int s;
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    argCheck(argc,argv);
    initSelf(argc,argv,&s);
    pthread_create(&thread,&attr,(void *) printUserOptionsInit,NULL);

    while(1)
    {

        if(TOKEN)
        {
            NEXT_MSG.token = 1;
            if(LOCAL_MSG.action != CLEAR){
                if(LOCAL_MSG.action == WRITE)
                    writeBulletin();

                else if(LOCAL_MSG.action == LEAVE)
                {
                    exitRing(s);
                    break;
                }
                else if(LOCAL_MSG.action == READ)

                    readBulletin(LOCAL_MSG.msg_number);

                else if(LOCAL_MSG.action == LIST)
                {
                    printf("The valid range is from 1 to %d\n",bulletinMessageCount());
                    threadUnlock(0);
                }
            }

            sendToNeighbor(s);
            TOKEN = 0;
            NEXT_MSG.token = 0;
        }
        recieveMessage(s);
    }

    pthread_join(thread,NULL);
    pthread_mutex_destroy(&LOCAL_MSG.busy);
    printf("Shuting Down Gracefully...\n");

	return 0;
}
