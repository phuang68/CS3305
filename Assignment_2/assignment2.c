/* 	Author: Pu Huang
	Description: Assignment_2
	Course: CS3305A
	Date: Sep 29th, 2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Reading and writing end of the pipe
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{

	/*************************************** command line arguments ***************************************
		To compile assignment2.c: gcc assignment2.c -o assignment2
		To run your program: ./assignment2 "CS" "3305" " is fun!"
		argv[0] = "./assignment2"
		argv[1] = "CS"
		argv[2] = "3305"
		argv[3] = " is fun!"

		Number of arguments (argc) = 4

		In this assignment, the parent process reads argv[1] and the child process reads argv[2] and argv[3]
		e.g. In the parent process: char *x = argv[1];
	*******************************************************************************************************/

	// If the user does not pass X, Y and Z, the program will terminate
	if (argc != 4)
	{
		printf("Invalid arguments\n");
		exit(0);
	}

	// You must insert the following into your code (Replace zeros with the appropriate values/variables)
	//printf("A pipe is created for communication between parent (PID %d) and child\n", 0);
	//printf("parent (PID %d) created a child (PID %d)\n", 0, 0);
	//printf("parent (PID %d) receives X = \"%s\" from the user\n", 0, 0);
	// printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", 0, 0, 0);
	// printf("child (PID %d) concatenates Y and Z to generate Y'= %s\n", 0, 0);
	// printf("child (PID %d) writes Y' into the pipe\n", 0);
	// printf("parent (PID %d) reads Y' from the pipe (Y' = \"%s\")\n", 0, 0);
	// printf("parent (PID %d) concatenates X and Y' to generate the string: %s\n", 0, 0);

	// Note: You do not need to implement a function to concatenate two strings. "string.h" header file has been included



	/***************************************************************************************************
	 * 										   YOUR CODE GOES HERE										
	 ***************************************************************************************************/
	int i,j;
	char x[100], y[100], z[100];// Create strings for receiving arguments from user
	char readbuffer[100]; // Used for reading from the pipe
	int port[2];
	pid_t pid; // pid for creating a child process

	if (pipe(port) < 0) // Create a pipe for communication between parent and child
	{
		perror("pipe error");
		exit(0);
	}
	else{
		i = getpid();
		printf("A pipe is created for communication between parent (PID %d) and child\n", i);
	}

	pid = fork(); // Create child process
	if (pid < 0)
	{
		perror("fork error");
		exit(0);
	}

	if(pid > 0){ //Parent
		i = getpid();
		printf("parent (PID %d) created a child (PID %d)\n", i, pid);

		strcpy(x, argv[1]);// Parent reads X from user
		printf("parent (PID %d) receives X = \"%s\" from the user\n", i, x);

		wait(NULL);
		read(port[0], readbuffer, sizeof(readbuffer));// Read Y' from the pipe
		printf("parent (PID %d) reads Y' from the pipe (Y' = \"%s\")\n", i, readbuffer);

		strcat(x, readbuffer);// Concatentate X and Y'
		printf("parent (PID %d) concatenates X and Y' to generate the string: \"%s\"\n", i, x);
	}
	
	if(pid == 0){ //child
		j = getpid();  // Get child's pid

		strcpy(y, argv[2]); //Child reads Y from user:
		strcpy(z, argv[3]); //Child reads Z from user:
		printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", j, y, z);

		strcat(y, z); // Concatentate Y and Z to generate Y'
		printf("child (PID %d) concatenates Y and Z to generate Y'= \"%s\"\n", j, y);

		write(port[1], y, (strlen(y) + 1)); // Write Y' into the pipe
		printf("child (PID %d) writes Y' into the pipe\n", j);
		
	}

	return 0;
}
