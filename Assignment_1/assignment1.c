#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	/************************************************** command line arguments ***********************************************
		For the purposes of this explanation, assume that "external_program.out" is located in /home/usr/A1/external_program.out 

		When you are testing your program, replace "/home/usr/A1/external_program.out" with the path to the executable file 
		generated when you compile "external_program.c"
		
		To compile assignment1.c: gcc assignment1.c -o assignment1
		To run your program: ./assignment1 /home/usr/A1/external_program.out
		argv[0] = "./assignment1"
		argv[1] = "/home/usr/A1/external_program.out"

		Number of arguments (argc) = 2

		In this assignment, the path to the external program is in argv[1]
	*************************************************************************************************************************/

	// If the path of external_program.out isn't provided by the user, the program will terminate
	if (argc != 2)
	{
		printf("Invalid arguments\n");
		exit(0);
	}

	// You must insert the following into your code (Replace zeros with the appropriate values/variables)
	// printf("parent process (PID %d) created child_1 (PID %d) \n", 0, 0);
	// printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", 0, 0);
	// printf("child_1 (PID %d) created child_1.1 (PID %d)\n", 0, 0);
	// printf("child_1 (PID %d) is now complete\n", 0);
	// printf("parent (PID %d) created child_2 (PID %d)\n", 0, 0);
	// printf("child_2 (PID %d) is calling an external program external_program.out and leaving child_2..\n", 0);

	// Hint: You might find snprintf() helpful in this assignment



	/***************************************************************************************************
	 * 										 YOUR CODE GOES HERE										
	 ***************************************************************************************************/
	pid_t pid1, pid1_1, pid2, i, j, status;
	char buffer[100];

	pid1 = fork(); //Create the first child process

	if (pid1 < 0) // Fork unsuccessful
	{
		printf("fork unsuccessful");
		exit(1);
	}

	if (pid1 > 0)// Parent
	{
		wait(NULL);// Waiting for child_1's process to finish
		pid2 = fork();// Create the second child process
		wait(NULL);//Waiting for child_2's process to finish
	}

	if (pid1 == 0)// Child 1
	{
		i = getppid(); // Get parent's pid
		j = getpid(); // Get child_1's pid
		printf("parent process (PID %d) created child_1 (PID %d) \n", i, j);
		printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", i, j);
	
		if(j > 0){
			pid1_1 = fork(); //Only create child_1.1 process when child_1 is created successfully
		}

		if(pid1_1 == 0){
			i = getppid(); // Get child_1's pid
			j = getpid(); // Get child_1.1's pid
			printf("child_1 (PID %d) created child_1.1 (PID %d)\n", i, j);
			printf("child_1 (PID %d) is now complete\n", i);
		}		
	}

	if (pid2 == 0) // Child 2
	{
		i = getppid();// Get parent's pid
		j = getpid(); // Get child_2's pid
		printf("parent process (PID %d) created child_2 (PID %d) \n", i, j);
		printf("child_2 (PID %d) is calling an external program external_program.out and leaving child_2..\n", j);
		sprintf(buffer,"%d\n", j); // Transforming child_2's pid to be of type char
		status = execl(argv[1], argv[1], buffer, NULL); // Calling external program

		if (status < 0)
			printf("\n child_2(PID %d): execl failed", j);
	}

	return 0;
}
