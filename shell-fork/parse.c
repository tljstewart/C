/** @file parse.c
 *  @brief myshell controls
 *
 *  this file contains the fucntions that control myshell
 *  Copyright (c) 2018 All rights reserved.
 *  
 *  @author Timothy L.J. Stewart
 *  @author Jeff Murphy
 *  @bug No Known Bugs
 */

#include "parse.h"

/*get user input*/
void initMyShell(){

	int i;

	/*init*/
	arguments.inputRedirect = NULL;
	arguments.outputRedirect = NULL;
	arguments.background = 0;
	arguments.argumentCount = 0;

	for(i=0; i<MAXARGS; i++){
		arguments.argumentVector[i] = NULL;
	}
	
	for(i=0; i<MAXCOMMANDLENGTH; i++){
		arguments.prompt[i] = '\0'; /*must use single quotes, double quotes means char+\0, must use strcpy to change array string
*/	}
	/*ok ready*/
	
}


/*done*/
void getUserInput(){
	
	printf("$$$ ");	
	fgets(arguments.prompt,sizeof(arguments.prompt),stdin);

}


void parseUserInput(){
	
	int i=0;
	
	/*handle whitespace only user inputs*/
	while(!(arguments.argumentVector[i]=strtok(arguments.prompt, " \t\n"))){
		printf("Nice Try Hacker\n");
	
		getUserInput();	
	}
	
	i++;
	while((arguments.argumentVector[i]=strtok(NULL, " \t\n"))){
		
		
		/* handle redirects with whitespace*/
		if(strcmp(arguments.argumentVector[i-1],"<")==0){
			arguments.inputRedirect = arguments.argumentVector[i];
			arguments.argumentVector[i]= NULL;
			arguments.argumentVector[--i] = NULL;
			continue;
			/*wipe out < and redirect pointer in argumentVector and do not increment i*/
		}
		if(strcmp(arguments.argumentVector[i-1],">")==0){
			arguments.outputRedirect = arguments.argumentVector[i];
			arguments.argumentVector[i]= NULL;
			arguments.argumentVector[--i] = NULL;
			continue;
		}
	
		/*handle redirects w/o whitespace*/
		if(strncmp((arguments.argumentVector[i]),"<",1)==0 && strncmp((arguments.argumentVector[i]+1),"\0",1)!=0){
			arguments.inputRedirect = arguments.argumentVector[i]+1;
			arguments.argumentVector[i] = NULL;
			continue;
		}
		if(strncmp((arguments.argumentVector[i]),">",1)==0 && strncmp((arguments.argumentVector[i]+1),"\0",1)!=0){
			arguments.outputRedirect = arguments.argumentVector[i]+1;
			arguments.argumentVector[i] = NULL;
			continue;

		}	

		if(strcmp(arguments.argumentVector[i],"&")==0){
			arguments.background = 1;
			arguments.argumentVector[i] = NULL;
			continue;
	
		}
		i++;
	}
	arguments.argumentCount = i;
}

/*done*/
void debugMode(int argc, char **argv){
	debug = false;

	if(argc > 1){
		if(strcmp(argv[1],"-Debug")==0){
			printf("Entering Debug Mode\n\n");	
			debug = true;	
		}	
	}
}

/*done*/
void printParams(Param_t * param){
	int i;
	printf ("InputRedirect: [%s]\n",
 (param->inputRedirect != NULL) ? param->inputRedirect:"NULL");
	printf ("OutputRedirect: [%s]\n",
 (param->outputRedirect != NULL) ? param->outputRedirect:"NULL");
	printf ("Background: [%d]\n", param->background);
	printf ("ArgumentCount: [%d]\n", param->argumentCount);
	for (i = 0; i < param->argumentCount; i++)
		printf("ArgumentVector[%2d]: [%s]\n", i, param->argumentVector[i]);
return;
}

void runUserInput(){

	pid_t pid = fork();
	pid_t w;
	int wstatus = 0;

	if(pid == -1){/*fork failed*/
		perror("Fork Failed");
		exit(1);
	}

	else if(pid ==0){/*isChild*/

		if(arguments.inputRedirect){
			freopen(arguments.inputRedirect, "r", stdin);
		}
		if(arguments.outputRedirect){
			freopen(arguments.outputRedirect, "w", stdout);
		}

		/*printf("hi i am child");*/	
		execvp(arguments.argumentVector[0],(arguments.argumentVector));
		perror(arguments.argumentVector[0]);
		exit(1);

	}	
	else{/*isParent*/

		if(arguments.background){
		/*dont wait on child*/
		}
		else{
		/*wait on child, run in foreground
		wait(NULL);*/
		do {
                   w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
                   if (w == -1) {
                       perror("waitpid");
                       exit(EXIT_FAILURE);
                   }

                   if (WIFEXITED(wstatus)&&debug) {
                       printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                   } else if (WIFSIGNALED(wstatus)&&debug) {
                       printf("killed by signal %d\n", WTERMSIG(wstatus));
                   } else if (WIFSTOPPED(wstatus)&&debug) {
                       printf("stopped by signal %d\n", WSTOPSIG(wstatus));
                   } else if (WIFCONTINUED(wstatus)&&debug) {
                       printf("continued\n");
                   }
               } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
             /*exit(EXIT_SUCCESS);*/

		
		}

	}
	/*do some house cleaning, check for zombies (i.e., completed background processes)*/
	waitpid(-1, &wstatus, WNOHANG);

}

bool exitMyShell(){

	int wstatus = 0;
	if((strcmp(arguments.argumentVector[0], EXIT)==0)){

		printf("\nPerforming Graceful Shutdown...\n");	
		
		while(wait(&wstatus)>0){
		
		}/*wait for all child before shutting down*/

		printf("\nAuf Wiedersehen!\n");
	return true; /*exit true*/
	}

return false;/*exit false*/
}
