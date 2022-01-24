# CS3305 - Operating Systems

### Course Description

This course presents fundamental concepts related to the design and implementation of operating systems. Topics includes intro to the computer operating system, basic operating system structure, process fundamental, inter-process communication, signal, system calls, process scheduling, process and thread synchronization and concurrency, memory basics, memory management, file systems and storage servers, and if time permits a few “hot” topics.

### Assignments

#### Assignment 1

Write a program in C that will perform the following tasks:

1. Your program will create a parent process which will create two child processes (e.g., child_1, and child_2)
2. parent will wait for child_1 to complete before creating child_2
3. child_1 will create its own child child_1.1
4. Inside child_2, a system call to an external program will be made. child_2 must pass its PID to the external program “external_program.out.” As a result of this external program call, child_2 will be replaced by external_program.out (hint: execl()). The path to the external program “external_program.out” will be passed into the program as a command line argument.

#### Assignment 2

Write a C program that will accept three strings from the user as command line arguments (for example, X, Y and Z). Your program will create a parent and child where the parent process will read X and the child process will read Y and Z. The child process will concatenate Y and Z to generate Y’. child process will send Y’ to parent process by communicating through a pipe (i.e., shared memory). Then, the parent process will concatenate X and Y’ and output the resulting string. The expected output from your program should look like the following for the arguments “CS” “3305” and “ is fun!”:
1. A pipe is created for communication between parent (PID 2255) and child
2. parent (PID 2255) created a child (PID 2256)
3. parent (PID 2255) receives X = “CS” from the user
4. child (PID 2256) receives Y = “3305” and Z = “ is fun!” from the user
5. child (PID 2256) concatenates Y and Z to generate Y’= “3305 is fun!”
6. child (PID 2256) writes Y’ into the pipe
7. parent (PID 2255) reads Y’ from the pipe (Y’ = “3305 is fun!”)
8. parent (PID 2255) concatenates X and Y’ to generate the string: “CS3305 is fun!”

#### Assignment 3

Write a C program to test the data sharing ability of a thread and process. Your C program will do the following:
1. Your parent program will have three variables: int x,y,z; which to be initialized as 10, 20, and 0, respectively.
2. parent creating child: parent will create a child by fork() and the child will perform
z = x+y (i.e., add x and y and store the results in z). parent will wait for child to complete before parent proceeds. Upon completion of child, parent will print out the value of z. (50 points)
3. parent creating thread: After (2) above is completed, parent process will now create a thread by pthread_create() which will do the exact same task done by child above (i.e., z = x+y). parent will wait for its thread to complete before parent proceeds. Upon completion of the thread, parent will print out the value of z. (50 points)

#### Assignment 4

You will be applying CPU Scheduling Algorithms in the C programming language. A sample input file is provided with skeleton code, which must be used to develop the CPU Scheduling Algorithm, Round Robin (RR).



Skeleton Code:
The provided skeleton code performs the following:

1. Creates the number of ready queues as stated in the input file and assigns a time quantum for each queue
2. Creates all the processes for each of the ready queues based on the input file specifications (such as CPU burst time, arrival order, etc.).
3. Once the simulation of all the ready queues is complete, the program will output results to a text file “cpu_scheduling_output_file.txt”.



What you need to do:
The CPU scheduling algorithm RR must be applied on each ready queue. Ready queues should be simulated in the order of their queue number (q 1, q 2, etc.). You are required to implement the “rr” function in the file scheduling.c. If necessary, you may implement helper functions in the scheduling.c file to complete the assignment.

#### Assignment 5 + 6

Using C programming language, you will be developing a mutual exclusion algorithm for a process synchronization problem. You need to make sure that your mutual exclusion algorithm ensures that only one process can access the critical section portion of your code at a given point in time. You are allowed to use any mutual exclusion / semaphore related C systems calls.

Assume that there are a set of n bank accounts (n ≥ 1) shared by a set of x clients (x ≥ 1). Clients can perform three different types of transactions with each bank account: deposit, withdraw or transfer funds. If a particular transaction results in a negative account balance, the transaction should be ignored (i.e. an account balance should never be less than 0).
