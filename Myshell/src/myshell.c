#include<stdio.h>
#include<string.h>
#include <sys/wait.h>
#include <stdlib.h> 
#include <sys/types.h>  
#include <errno.h> 
#include <unistd.h>
#define MAX_LINE_SIZE 255

//CS 410 Brandon Ahrens Assignment 2.
//To run a file and its commands, use the 'run' command. IE: 'run ls -l'
//To exit the shell, use the 'exit' command by itself.

//these variables are global so they can be used in all of the functions in this program with ease.
char *words[20]; //will be used to store tokenized user input line.
char *args[20]; //will be used to store the file and commands the user wants to execute that they
		//inputted. Takes char *words and gets rid of the first index, which, in this case, would
		//be 'run' and takes the rest of the file and commands and puts them in this char ptr array
		//so it can be in the right format for execvp to work properly.
int i = 0; //index for char *words

//See actual functions below for more details.
void get_args(); //formats *words to *args for execvp to work properly.
int check_operation(); //checks the operation the user inputted.
void check_fork_result(); //checks fork result and performs the process action.
void perform_parent_process(); //performs parent process actions.
void perform_child_process(); //performs child process actions.


int main()
{

	//shell loop.
	while(1){

		//custom prompt.
		printf("myshell> ");
		
		//get user input.
		char line[MAX_LINE_SIZE];
		fgets(line, MAX_LINE_SIZE, stdin);

		//check if user only entered 'run' by itself, or entered nothing.
		//If user did one of these two actions, then do nothing and reloop to get next input.
		if (strcmp(line, "run\n")!=0 && strcmp(line, "\n")!=0){
			
			//tokenize line into an array of C strings.
			words[i] = strtok(line," \n");

			//check if run or exit operation is entered.
			int operation = check_operation();

			//if user enters exit, exit shell.
			if (operation == 0){
				exit(0);
			}
			else if (operation == 1){ //if user entered run.
				
				int status; //status of fork.
	
				//fork a new process.
				pid_t result = fork();

				//see if process is parent or child or failed.
				//if a parent process, will call perform_parent_process()
				//if a child process, will call perform_child_process()
				check_fork_result(&status, result);	
			}
		}
	}

	return 0;
}




//takes *words and formats it by putting it into *args without the 'run' operation as the first index
//so the user specified file and commands can be executed with execvp properly.
void get_args(){
	
	int j; //used for args index
	while (words[i]!=NULL){						
		i++;
		words[i] = strtok(NULL," \n");
		j = i-1; //will start at index 0.
		//builds arg array with file to execute and commands.
		args[j] = words[i];	
	}
}


//Will check to see what operation the user entered. The only two valid operations are: 'run' and 'exit'.
//If user enters an operation other than those two, then they will be told that the operation was invalid.
//Returns 0 if operation was exit.
//Returns 1 if operation was run.
//Returns -1 if operation was invalid. After returning with -1, will throw user back into while loop in main(),
//waiting for next valid input.
int check_operation(){
	//check what operation user entered.
	if (strcmp(words[0], "run")==0 || strcmp(words[0], "exit")==0){
		if (strcmp(words[0], "exit")==0){ //if exit was entered.
			return 0;
		}
		else{ //if run was entered.
			return 1;
		}
	}
	else { //if user entered invalid operation.
		printf("myshell: %s is not a valid operation \n", words[0]);
	}
	return -1; //will return -1 if a valid operation was NOT entered.
}


//Will check the result of fork() to see if the process is a parent process or child process.
//If the process is a parent, then perform_parent_process() will be called.
//If the process is a child, then perform_child_process() will be called.
//If fork() failed, a message will be displayed for the user with the error.
void check_fork_result(int status, pid_t result){
	//if parent, wait for child to finish.
	if (result>0){
		perform_parent_process();
	}
	else if (result == 0){ //if child process.
		perform_child_process();
	}
	else if(result == -1){ //if error when calling fork().
		printf("fork() failed: %s\n", strerror(errno));
	}
}


//Will perform the parent process' actions.
//Here, the parent process will just wait for the child process to finish it's actions, then continue on
//until the user inputs another run operation or exit operation.
//If wait() failes, a message will be displayed for the user with the error.
void perform_parent_process(int status){
	//wait for child process, but also check for error.
	if (wait(&status)==-1){
		printf("wait() failed: %s\n", strerror(errno));
	}
}


//Will perform the child process' actions.
//Here, the child process will display it's pid, call get_args, and execute the given file and commands, the
//user wishes to execute. Once executed, the process will automatically exit. So no need to call exit() if
//execvp() was a success.
//If execvp() fails, a message will be displayed to the user with the error and will call exit() to exit process.
void perform_child_process(){
	//prints child pid.
	printf("myshell: starting child pid %d\n", getpid());

	//get args for file and commands to run from *words.
	get_args();

	//execute given file and commands and check for error.
	if (execvp(*args, args) == -1){
		printf("myshell: %s %s\n", strerror(errno), args[0]);
		//exits child process with error.
		exit(1);
	}
}
