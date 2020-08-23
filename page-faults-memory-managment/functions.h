/** @file functions.h
 *  @brief Function Prototypes for functions.c
 *
 *  this file contains function protoypes, data structures and global vars
 *  for parse.c
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */


#ifndef functions_h
#define functions_h

#define BILLION 1000000000L 	/**< BILLION for dealing with timespec nanoseconds**/
#define TOOLARGEROW 20480	/**< intentionally number of pages**/
#define TOOLARGECOLUMN 4096	/**< intentionally equal to system page size**/
#define RUNS 10L /*L for Long int to shut complier up about int overflow warning*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>

/** @var start stop inStart inStop
 *  @brief global var of type timespec used for system call CLOCKTIME()
 * 
 */
struct timespec start, stop, inStart, inStop;

/** @var tooLargeArray[][]
 *  @brief global var char array 83MB large with rows equal to page size 4096 bytes
 * 
 */
char tooLargeArray[TOOLARGEROW][TOOLARGECOLUMN];

/** @brief writes large array by rows
 * 
 * @return void
 */
void writeRow();

/** @brief reads large array by rows
 * 
 *  @return void 
 */
void readRow();


/** @brief writes large array by column
 * 
 *  @return void 
 */
void writeColumn();


/** @brief reads large array by column
 * 
 *  @return void 
 */
void readColumn();

#endif
