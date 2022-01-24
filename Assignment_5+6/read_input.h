/* 	Author: Pu Huang
	Description: Assignment_5+6
	Course: CS3305A
	Date: Dec 1st, 2020
*/
#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <stdio.h>
#include "types.h"

int num_client_queues(char *line);
int read_input_file(char *filename, input_queues *inp);
void *withdraw_money(void *trans);
void *deposit_money(void *trans);
void *transfer_money(void *trans);
void print_result(int *bank_result);

#endif