/** @file functions.h
 *  @brief Function Prototypes for functions.c
 *
 *  this file contains function protoypes, data structures and global vars
 *  for functions.c
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */


#ifndef functions_h
#define functions_h

#define MAXLENGTH_N 100001
#define MAXSTOPPINGTIME 600
#define MAXTHREAD 50
#define BILLION 1000000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>

extern int errno;

/** @struct HISTO
 *  @brief a structure for collatz computations
 * 
 *  PARAM stores user command line input to execute with myshell
 *
 */
struct HISTO{
	int frequencyCount;                  /**<the number of times a stopping time a_sub_i has occured */
	time_t avgTime;		/**< average time it took to reduce to stopping time a_sub_i*/

	time_t totalTime;	/**< total time it took to reduce to stopping time a_sub_i */
	
};

/** @var Histo_t 
 *  @brief type definition of HISTO
 * 
 */
typedef struct HISTO Histo_t;


/** @struct NRANGE
 *  @brief a structure for characteristics of each number
 * 
 *  PARAM stores user command line input to execute with myshell
 *
 */
struct NRANGE{
	int n;				/**<the number*/
	int na_sub_i;			/**< the a_sub_i (i.e., stopping time) the number of times it take to iterate n to 1*/
	int visitedCount;		/**< if not equal to 1, race condition must of occured*/
	time_t n_time;			/**< time it took to reduce n to stopping time a_sub_i*/
};

/** @var Nrange_t 
 *  @brief type definition of NRANGE
 * 
 */
typedef struct NRANGE Nrange_t;

/** @var global a_sub_i[MAXSTOPPINGTIME] an instance of HISTO
 *  @var global thee_n[MAXLENGTH_N] an instance of NRANGE data structure 
 *  @var global lock use mutex locking for thread syncronization or not
 *  @var userinput_N_range argument passed by user, N range of numbers to computer on
 *  @var userinput_T_thread argument passed by user, T number of threads to make
 *  @var global tid[MAXTHREAD] array of thread ids
 *  @var global nextAvaliable_n the next value n to compute collatz on
 *  @var global mutex to lock and unlocks for thread syncronization
 * 
 *  debug is set in debugMode function with switch
 *  by starting myshell -Debug, defeault is false
 */

Histo_t a_sub_i[MAXSTOPPINGTIME];		/**< an instance of HISTO*/
Nrange_t thee_n[MAXLENGTH_N];			/**< an instance of NRANGE*/
bool lock;					/**< either 0 (false) or 1 (true) */
int userinput_N_range;
int userinput_T_thread;
pthread_t tid[MAXTHREAD];
int nextAvaliable_n;
pthread_mutex_t mutex;                  	/**<NOT WORKING: a mutex lock for critical section, note that a right hand value is fine as PTHREAD_MUTEX_INITIALIZER is special, this is the same as calling pthread_init(&mutex,NULL) somewhere in code */

/** @brief initializes vars
 * 
 *  set all memory locations to initial values, NULL, O
 *
 *  @param void
 *  @return void 
 */
void init();

/** @brief compute collatz starting times
 * 
 *
 *  @param void* n the address passed to the Thread
 *  @return void* returned with pthread_exit()
 */
void* doMath(void *n);

/** @brief check argumnets passed on command line
 *
 *  call ./myshell N T [-nlock]
 *  N range of numbers to compute on up to 100000
 *  T number of threads to create
 *  [-nolock] do not use locking on critical sections
 *
 *  @param argc argument count for myshell call
 *  @param argv argument vector for myshell call
 *  @return void 
 */
void getArgs(int argc, char **argv);

/** @brief create T threads do compute doMath() function
 *
 *  @param void
 *  @return void 
 */
void makeThreads_doMath();

/** @brief join all threads before exiting program
 * 
 *
 *  @param void
 *  @return void 
 */
void waitOnThreads();

/** @brief print data structure HISTO
 * 
 *  print to stdout data structure HISTO
 *
 *  @param void
 *  @return void 
 */
void printHistogram();

/** @brief print the data structure NRANGE
 * 
 *  create and write to a file datastructure NRANGE
 *
 *  @param void
 *  @return void 
 */
void printNRANGE();

#endif
