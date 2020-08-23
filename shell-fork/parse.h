/** @file parse.h
 *  @brief Function Prototypes for parse.c
 *
 *  this file contains function protoypes, data structures and global vars
 *  for parse.c
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */


#ifndef parse_h
#define parse_h
#define MAXARGS 32  /**<dont test program with more than this many tokens for input */

#define MAXCOMMANDLENGTH 256
#define EXIT "exit"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>


/** @struct PARAM
 *  @brief a structure for user input
 * 
 *  PARAM stores user command line input to execute with myshell
 *
 */
struct PARAM{
	char *inputRedirect;             /**< file name or NULL */
	char *outputRedirect;            /**< file name or NULL */
	int background;                  /**< either 0 (false) or 1 (true) */
	int  argumentCount;              /**< number of tokens in argument vector */
	char *argumentVector[MAXARGS];   /**< array of strings */
 	char prompt[MAXCOMMANDLENGTH];	 /**< array for user input */
	
};

/** @var Param_t 
 *  @brief type definition of PARAM
 * 
 */
typedef struct PARAM Param_t;

/** @var arguments
 *  @brief global var of struct PARAM
 * 
 *  arguments is an instants of struct PARAM
 */
Param_t arguments;		/**< an instance of PARAM */

/** @var debug 
 *  @brief global var for debug mode
 * 
 *  debug is set in debugMode function with switch
 *  by starting myshell -Debug, defeault is false
 */
bool debug;			/**< either 0 (false) or 1 (true) */

/** @brief initializes vars
 * 
 *  set all memory locations to initial values, NULL, O
 *
 *  @param void
 *  @return void 
 */
void initMyShell();

/** @brief prompts and stores users input 
 *  
 *  prompts and stores users input
 *   
 *  @param void
 *  @return void 
 */
void getUserInput();

/** @brief parse user input
 * 
 *  during mysell execution parseUserInput will parse the input
 *  into PARAM struct 
 * 
 *  @param void
 *  @return void 
 */
void parseUserInput();

/** @brief check for -Debug
 *
 *  when users calls ./myshell -Debug, debugMode  will set global debug to true
 *
 *  @param argc argument count for myshell call
 *  @param argv argument vector for myshell call
 *  @return void 
 */
void debugMode(int argc, char **argv);

/** @brief prints user parameters
 * 
 *  if in debug mode printParams will print out the PARAM struct
 *
 *  @param param a pointer to the PARAM struct
 *  @return void 
 */
void printParams(Param_t * param);

/** @brief run the user commands
 * 
 *  runs the parsed user input handles redirects and backgrounding,
 *  implements forks, exec and wait system calls
 *
 *  @param void
 *  @return void 
 */
void runUserInput();

/** @brief checks for exit command
 * 
 *  if exit command is given, checks for child process before termination
 *
 *  @param void
 *  @return bool 
 */
bool exitMyShell();








#endif
