/** @file mt-collatz.c
 *  @brief a multi-threading program
 *
 *  this file contains the entry point for mt-collatz, main()
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "functions.h"

/*MAIN*/
int main(int argc, char **argv)
{
	struct timespec start, stop;
	long taskTime = 0;/*long is signed 32 bits, 31 bits for number last bit for sign. 1billion or nanoseconds requires 30 bits.*/
	int i =0;
	int runs = 10;/*runs used to compute average computation time*/
	
	while(i<runs){/*runs multiple times to compute average time*/

		init();/*initialize all vars*/
		getArgs(argc, argv);/*check user args passed to program*/

		clock_gettime(CLOCK_REALTIME, &start);/*start clock*/
		makeThreads_doMath();/*start computation*/
		waitOnThreads();/*end computation*/
		clock_gettime(CLOCK_REALTIME, &stop);/*end clock*/
	
		i++;	
		taskTime += (BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec);
	
	}
	printHistogram();
	printNRANGE();
	
	fprintf(stderr,"%d,%d,%.9f\n", userinput_N_range, userinput_T_thread, (float)taskTime/(runs*BILLION));
	
return 0;/*EXIT_SUCCESS*/
}

