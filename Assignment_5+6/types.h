/* 	Author: Pu Huang
	Description: Assignment_5+6
	Course: CS3305A
	Date: Dec 1st, 2020
*/
#ifndef TYPES_H
#define TYPES_H
#include <pthread.h>

typedef struct input_queues
{
    int *acc_b;
    int **client_queue;
    int num_of_request;
    int w;
    int d;
    int t;
    int size_c;
    int size_a;
    int *size_of_requests;
} input_queues;

typedef struct transaction
{
    int *bank_acc;
    pthread_t thread;
    int account_from;
    int account_to;
    int amount;
} transaction;

#endif