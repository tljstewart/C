/** @file functions.c
 *  @brief matrix controls
 *
 *  this file contains the fucntions that controls matrix
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */

#include "functions.h"

void writeRow(){

	long taskTime = 0;
	int i = 0;

	fprintf(stderr,"writeRow\n");
	clock_gettime(CLOCK_REALTIME, &start);/*start clock*/

	while(i<RUNS){
		int j=0;
		int k=0;
	
                clock_gettime(CLOCK_REALTIME, &inStart);/*start clock*/
	
		for(;j<TOOLARGEROW; j++){
			
			for(k=0;k<TOOLARGECOLUMN;k++){
				
				tooLargeArray[j][k]= 'X';

			}
		}

		i++;

		clock_gettime(CLOCK_REALTIME, &inStop);/*end clock*/

                fprintf(stderr,"%.9f\n",(float)(BILLION*(inStop.tv_sec - inStart.tv_sec) + inStop.tv_nsec - inStart.tv_nsec)/(BILLION));

	}
	
	clock_gettime(CLOCK_REALTIME, &stop);/*end clock*/
	taskTime = (BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
	printf("writeRow,%.9f\n", (float)taskTime/(RUNS*BILLION));


}


void readRow(){

        long taskTime = 0;
        int i = 0;
	char temp;

	fprintf(stderr,"\nreadRow\n");
        clock_gettime(CLOCK_REALTIME, &start);/*start clock*/

        while(i<RUNS){
		int j=0;
		int k=0;
	
                clock_gettime(CLOCK_REALTIME, &inStart);/*start clock*/
	
		for(;j<TOOLARGEROW; j++){
			
			for(k=0;k<TOOLARGECOLUMN;k++){
				
				temp = tooLargeArray[j][k];
				temp++;

			}
		}

		i++;
		clock_gettime(CLOCK_REALTIME, &inStop);/*end clock*/

                fprintf(stderr,"%.9f\n",(float)(BILLION*(inStop.tv_sec - inStart.tv_sec) + inStop.tv_nsec - inStart.tv_nsec)/(BILLION));

        }
        
        clock_gettime(CLOCK_REALTIME, &stop);/*end clock*/
        taskTime = (BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
        printf("readRow,%.9f\n", (float)taskTime/(RUNS*BILLION));


}

void writeColumn(){

        long taskTime = 0;
        int i = 0;

	fprintf(stderr,"\nwriteColumn\n");
        clock_gettime(CLOCK_REALTIME, &start);/*start clock*/

        while(i<RUNS){
		int j=0;
		int k=0;
	
                clock_gettime(CLOCK_REALTIME, &inStart);/*start clock*/
	
		for(;j<TOOLARGECOLUMN; j++){
			
			for(k=0;k<TOOLARGEROW;k++){
				
				tooLargeArray[k][j]= 'Y';

			}
	
		}

		i++;
		clock_gettime(CLOCK_REALTIME, &inStop);/*end clock*/

                fprintf(stderr,"%.9f\n",(float)(BILLION*(inStop.tv_sec - inStart.tv_sec) + inStop.tv_nsec - inStart.tv_nsec)/(BILLION));

        }

        clock_gettime(CLOCK_REALTIME, &stop);/*end clock*/
        taskTime = (BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
        printf("writeColumn,%.9f\n", (float)taskTime/(RUNS*BILLION));


}

void readColumn(){

        long taskTime = 0;
	int i = 0;
	char temp;

	fprintf(stderr,"\nreadColumn\n");
        clock_gettime(CLOCK_REALTIME, &start);/*start clock*/

        while(i<RUNS){
		int j=0;
		int k=0;

		clock_gettime(CLOCK_REALTIME, &inStart);/*start clock*/
		
		for(;j<TOOLARGECOLUMN; j++){
			
			for(k=0;k<TOOLARGEROW;k++){
				
				temp = tooLargeArray[k][j];
				temp++;
			}
		
		}
		clock_gettime(CLOCK_REALTIME, &inStop);/*end clock*/

        	fprintf(stderr,"%.9f\n",(float)(BILLION*(inStop.tv_sec - inStart.tv_sec) + inStop.tv_nsec - inStart.tv_nsec)/(BILLION));

		i++;

        }

        clock_gettime(CLOCK_REALTIME, &stop);/*end clock*/
        taskTime = (BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
        printf("readColumn,%.9f\n", (float)taskTime/(RUNS*BILLION));


}

