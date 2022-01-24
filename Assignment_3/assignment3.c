/* 	Author: Pu Huang
	Description: Assignment_3
	Course: CS3305A
	Date: Oct 18th, 2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

	int x, y, z; 

void *sum(){
	z=y+x;
	return NULL;
}

int main(){
	pid_t child;
	pthread_t thread;
	// Initializing the global variables
	x=10, y=20, z=0;

	/* MOVE THE FOLLOWING LINES INTO THEIR PROPER PLACES...
		printf("main function: errno number is %d\n", errno);
		printf("Using a fork(), the value of z in the child process is: %d\n", z); 
		printf("Using a fork(), the value of z in the parent process is: %d\n", z); //value of z after the fork process won't change
		printf("Using a thread, the value of z is: %d\n", z); //value of z after passing to the thread will change
		
    */

   /* Add code under every comment to demonstrate differences between child and thread */

    // create child process
	child = fork();
	// check for child creation error
	if (child < 0)
	{
		perror("fork error");
		exit(0);
	}

	// in parent:
	// wait for child, and print value of z
	// create thread, wait for it to complete, then print value of z
	if(child > 0){
		wait(NULL);
		printf("Using a fork(), the value of z in the parent process is: %d\n", z); //value of z after the fork process won't change
		if(pthread_create(&thread, NULL, sum, NULL)){//Create thread using the sum function and also check for failure of creating a thread
			printf("main function: errno number is %d\n", errno);//Out put message if the thread is created unsuccessfully 
			exit(1);
		}
		pthread_join(thread, NULL); // Wait until the thread executes
		printf("Using a thread, the value of z is: %d\n", z); //value of z after passing to the thread will change
	}

	// in child:
	if(child == 0){
		z = x + y; // sum x and y and store it in z
		printf("Using a fork(), the value of z in the child process is: %d\n", z); // print value of z
	}

	return 0;
}
