/** @file matrix.c
 *  @brief compares reads/writes times of matrix by rows and by columns
 *
 *  this file contains the entry point for matrix, main()
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

//MAIN
int main(int argc, char **argv)
{
	writeRow();
	readRow();
	writeColumn();
	readColumn();

return 0;
}

