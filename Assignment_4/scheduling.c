/* 	Author: Pu Huang
	Description: Assignment_4
	Course: CS3305A
	Date: Nov 7th, 2020
*/
#include <stdlib.h>
#include "scheduling.h"

	/*
##  Round-Robin Scheduling Algorithm  ##

	Parameters:-

	queue: 1D int array containing the CPU burst time for every process in a ready queue
	np: number of processes (length of `queue` array)
	tq: time quantum for the RR algorithm


	Returns:-

	rr_result struct pointer, where
	rr_result -> order: 1D int array, containing the execution order of processes (i.e. `(rr_result -> order)[0]` is the process number of the first process)
		** hint: p1 is always the first item in order of selection in round-robin and FCFS
    rr_result -> order_n: int, representing the size of the `order` array, described above ^
	rr_result -> turnarounds: 1D int array, containing the turnaround time for each process (i.e. `(rr_result -> turnarounds)[0]` is the turnaround time for p1)
*/
	rr_result *rr(int *queue, int np, int tq)
{
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);

	// code here to assign values to result->turnarounds, result->order, and result->order_n
	int *ptr = malloc(sizeof(int) * np); // Pointer to an Integer array that will be dynamically sized
	int total_time = 0;  //Integer used for tracking the total time used in processing the queue, default at 0.
	int arrival_time[np]; //Integer array that will store each process's arrival time, with the same size as the ready queue
	int end_time[np];	 //Integer array that will store each process's end time, with the same size as the ready queue
	int remaining = np;	 //Integer used to track how many processes left unfinished, default at the number of the processes
	int order_num = 0;	 //Integer used to track order of how cpu traverse the queue, default at 0
	int counter = 0;	 //Integer used to track that a process is done so that the remaining count can be decremented, default at 0
	int new_size = np;	 //Integer used to dynamically allocate a new size of int array, default at the number of the processes
	int temp[np]; 		 //A temporal integer array that stores the burst time for each process
	for(int i = 0; i < np; i++)
	{ //copy all the burt time values to the temporal array
		temp[i] = queue[i];
		arrival_time[i] = 0;//Set all the arrival time at 0 for each process to recognize if the process has been assigned with an arrival time later
	}

	//Using Round Robin algorithm to process the ready queue
	while(remaining > 0){	//If there's still processes unfinished yet, we continue to traversing the queue and process the processes with remaining time
		if(order_num == new_size - 1) //If we reach the current size of the process order array, need to resize the process order array
		{
			new_size = new_size * 2;  // Double the current size
			int *new_ptr = realloc(ptr, sizeof(int) * new_size);  // Expand the size of the array with the new size to new array
			ptr = new_ptr;  //	Let the original order array pointer have the new array's address
		}
		for(int i = 0; i < np; i++) //  Traversing the process queue
		{
			if (temp[i] != 0)
			{  //Make sure the current process still has remaining time
				(ptr)[order_num] = i; // Store the current process number to the order array
				order_num++;  // Increment the order number
			}
			if (temp[i] <= tq && temp[i] > 0){  //If the process has shorter time than the time quantum

				if(arrival_time[i] == 0 && i !=0 )
				{  //If the process is processed the first time
					arrival_time[i] = total_time;  //Assign the process with their arrival time
					total_time = total_time + temp[i];  //Increment the total time spent on the process queue with the process's burst time
				}
				else
				{ // if it just a process that was processed before
					total_time = total_time + temp[i]; //Increment the total time spent on the process queue with the process's burst time
				}
				end_time[i] = total_time; // The end time for the process is determined and it's the total time spent on the queue so far
				temp[i] = 0;  // There's no time needed to be spent on the process
				counter = 1;  // We have one process is finished
			}
			else if (temp[i] > tq)
			{  //If the process has longer time than tq
				temp[i] = temp[i] - tq; // The remaining time for the process after the time quantum spented on the process
				if (arrival_time[i] == 0 && i != 0)
				{ //If the process is processed the first time
					arrival_time[i] = total_time;  //Assign the process with their arrival time
					total_time = total_time + tq;  //Increment the total time spent on the process queue with the time quantum
				}
				else
				{ // if it just a process that was processed before
					total_time = total_time + tq; //Increment the total time spent on the process queue with the time quantum
				}
			}
			if (temp[i] == 0 && counter == 1)
			{  //If the process is finished
				remaining--;  // There's one process less remainging in the queue to be process
				counter = 0;  // Reset the counter
			}
		}
		}
	result->order_n = order_num; // Assign the size of the `order` array
	result->order = ptr;  // Assign the order array with the processed order array
	for (int i = 0; i < np; i++)
	{
		(result->turnarounds)[i] = end_time[i] - arrival_time[i]; // Calculate the turnaround time for each process as we know the turnaround time is the difference between when the process finishes and when the process starts
	}
	return result;
}