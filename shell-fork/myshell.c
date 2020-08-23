/** @file myshell.c
 *  @brief a simple shell
 *
 *  this file contains the entry point for myshell, main()
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "parse.h"

/*MAIN*/
int main(int argc, char **argv)
{

	debugMode(argc, argv);/*check debug is set*/
	do{
		initMyShell();/*initialze vars*/
		getUserInput();/*print prompt and save user input*/
		parseUserInput();/*tokenize*/
		if(debug)printParams(&arguments);
		if(exitMyShell()) break;
		runUserInput();

	}while(true);
	
	return 0;
}

