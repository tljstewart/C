/** @file functions.c
 *  @brief myshell controls
 *
 *  this file contains the fucntions that control mt-collatz.c
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */

#include "functions.h"

void init(){
	int i;	
	
	/*init*/	
	lock = true;
	if(pthread_mutex_init(&mutex,NULL)){
		perror("pthread_mutex_init:");
		exit(EXIT_FAILURE);
	}
	nextAvaliable_n = 2;

	for(i = 0; i<MAXSTOPPINGTIME; i++){
		a_sub_i[i].frequencyCount = 0;
		a_sub_i[i].avgTime = 0;
		a_sub_i[i].totalTime=0;
	}

	for(i = 0; i<MAXLENGTH_N; i++){
		thee_n[i].na_sub_i = 0;
		thee_n[i].n_time=0;
		thee_n[i].n= i;
		thee_n[i].visitedCount =0;
	}
	for(i = 0; i<MAXTHREAD; i++){
		tid[i]=0;

	}
	
}

void getArgs(int argc, char **argv){

	if(argc > 2){
		if(argc > 3){
			if(strcmp(argv[3], "-nolock")==0){
				lock = false;
			}
		}
			
		if((atoi(argv[1]) > MAXLENGTH_N) || (atoi(argv[1]) < 2)){
			
			printf("Usage: First, a number between 2-100000 for the number of collatz to calculate, second the number of threads to create between 1-50, Third you may supply -nolock to run without mutex thread locks\n\n"); /*and exit program does perror do this? no it only prints to stderr*/
			exit(EXIT_FAILURE);
		
		}else{
			/*save*/
			userinput_N_range = atoi(argv[1]);
		}
		if((atoi(argv[2]) == 0) || (atoi(argv[2]) > 50)){
			
		printf("Usage: First, a number between 2-100000 for the number of collatz to calculate, second the number of threads to create between 1-50, Third you may supply -nolock to run without mutex thread locks\n\n"); /*and exit program does perror do this? no it only prints to stderr*/
			exit(EXIT_FAILURE);
	
		}else{
			/*save*/
			userinput_T_thread = atoi(argv[2]);
		}

	}else{
		printf("Usage: First, a number between 2-100000 for the number of collatz to calculate, second the number of threads to create between 1-50, Third you may optionaly supply -nolock to run without mutex thread locks\n\n");
		exit(EXIT_FAILURE);	
	}
}

void* doMath(void *n){/*the work done by each thread*/
	
	int tmp_n=0;
	int current_a=0;
	int previous_a=0;
	int current_i=0;
	time_t taskTime = 0;
	struct timespec start, stop; /*data structure req. for clock_gettime, defined in man page with two vars, tv_sec and tv_nsec struct timespec {
               time_t   tv_sec;        seconds 
               long     tv_nsec;       nanoseconds 
           };
 */
	while(true){
	/*start critical section*/
		if(lock){
			if(pthread_mutex_lock(&mutex)){
                	        perror("pthread_mutex_lock:");
                        	exit(EXIT_FAILURE);
	        	}
		}
		if(current_i > 0){/*save computations*/
			a_sub_i[current_i].frequencyCount++;
			a_sub_i[current_i].totalTime += taskTime;
			a_sub_i[current_i].avgTime = a_sub_i[current_i].totalTime/a_sub_i[current_i].frequencyCount;
	
			thee_n[tmp_n].n_time = taskTime;
			thee_n[tmp_n].na_sub_i = current_i;	
			thee_n[tmp_n].n = tmp_n;
			thee_n[tmp_n].visitedCount++;/*race condition? test to see if repeating work*/
		
		}

		current_i=0;/*reset i for next computation*/
		tmp_n = nextAvaliable_n;/*n to compute on*/	
		current_a = nextAvaliable_n;/*values of a during compute*/
		previous_a = current_a;
		nextAvaliable_n++;

		if(tmp_n > userinput_N_range){/*are we done yet?*/
	 
			if(lock){
                        	if(pthread_mutex_unlock(&mutex)){
        	                        perror("pthread_mutex_unlock:");
                	                exit(EXIT_FAILURE);
                       		 }
			}
	
		pthread_exit(NULL);
		}
		
		if(lock){
                       	if(pthread_mutex_unlock(&mutex)){
        	       		perror("pthread_mutex_unlock:");
                		exit(EXIT_FAILURE);
                       	 }
		}
		/*end critical section*/
		/*start timer*/
		clock_gettime(CLOCK_REALTIME, &start);
		while(current_a != 1){

			if(current_a % 2){/*odd*/
				current_a = 3*previous_a +1;
				previous_a = current_a;
			}else{/*even*/
				current_a = previous_a/2;	
				previous_a = current_a;
			}
		
			current_i++;
		}
	
		/*stop timer*/
		clock_gettime(CLOCK_REALTIME, &stop);
		taskTime = BILLION*(stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec;
	

	}

return EXIT_SUCCESS;
}

void makeThreads_doMath(){
	int i=0;

	for(i=0; i<userinput_T_thread; i++){
            if((pthread_create(&tid[i], NULL, doMath, &nextAvaliable_n))==EXIT_FAILURE){/*EXIT_FAILURE is 1*/
			perror("pthread_create():");
			exit(EXIT_FAILURE);			       
	       }
	}
}

void waitOnThreads(){
	
	int i;
	for(i=0; i<userinput_T_thread; i++){
               if(( pthread_join(tid[i], NULL))== EXIT_FAILURE){ /*EXIT_FAILURE is 1*/
			perror("pthread_join():");
			exit(EXIT_FAILURE);			       
	       }
        }
}

void printHistogram(){

	int i=0;
	while(i<MAXSTOPPINGTIME){
		if(a_sub_i[i].frequencyCount){ 
/*data feilds: the a_sub_i collatz stopping time, number of times a thread reduced a number n to this a_sub_i, avgerage time it took to compute a_sub_i*/
		 
			printf("%d,%d,%.9f\n",i,a_sub_i[i].frequencyCount,(float)a_sub_i[i].avgTime/BILLION);                     
                }
		i++;
        }
}

void printNRANGE(){

	int i=2;/*2 is the starting number n for the collatz computation*/
	FILE *fp;
       	char fileName[50];
	struct stat st;
	
	if(stat("./results", &st)==0){
			sprintf(fileName, "./results/%s-NRANGE-STATS-%d-%d.csv", (lock == true) ? "lock":"nolock", userinput_N_range, userinput_T_thread);
	}else{
			sprintf(fileName, "%s-NRANGE-STATS-%d-%d.csv", (lock == true) ? "lock":"nolock", userinput_N_range, userinput_T_thread);

	}
	
	fp = fopen(fileName,"w");/*create comma separated value file *NRANGE-STATS**/ 
	
	while(i<=userinput_N_range ){
		/*data feilds: the number n, the a_sub_i collatz stopping times, number of times a thread used the number n, time it took to compute a_sub_i on the number n*/
		fprintf(fp,"%d,%d,%d,%.9f\n",thee_n[i].n, thee_n[i].na_sub_i, thee_n[i].visitedCount, (float)thee_n[i].n_time/BILLION);
		i++;
        }
}
