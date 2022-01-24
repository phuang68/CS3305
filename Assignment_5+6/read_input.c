/* 	Author: Pu Huang
	Description: Assignment_5+6
	Course: CS3305A
	Date: Dec 1st, 2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "read_input.h"

pthread_mutex_t lock;
#define SIZEOF(a) sizeof(a) / sizeof(*a)

int num_client_queues(char *line) // Check how many processes for a client
{
    const char delim[2] = " ";
    char *tok;
    int r = -1;

    tok = strtok(line, delim);
    while (tok != NULL)
    {
        r++;
        tok = strtok(NULL, delim);
    }

    return r;
}

int read_input_file(char *filename, input_queues *inp) // Read the input file and create a input queue with the banking account info and the client requests info
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror(filename);
        exit(1);
    }

    char *buffer = NULL;
    size_t len = 0;
    char *tok;
    const char delim[2] = " ";
    ssize_t nread;

    int num_of_acc = 0;
    int num_of_client = 0;

    //Get the number of accounts and of clients by recognizing the first character of each line
    while (nread = getline(&buffer, &len, fp) != -1)
    {
        if(buffer[0] == 'a')
            num_of_acc++;
        else if(buffer[0] == 'c')
            num_of_client++;
            
    }
    //Allocate the parameters with suitable size
    int *b_queues = malloc(sizeof(int) * num_of_acc);
    int **client_queues = malloc(sizeof(int *) * num_of_client);
    int *size_of_requests = malloc(sizeof(int) * num_of_client);

    fseek(fp, 0, SEEK_SET);

    int ncq;
    int i = -1;
    int client = 0;
    while (nread = getline(&buffer, &len, fp) != -1)
    {
        if(client > 3){
            ncq = num_client_queues(buffer);
            //printf("The size for the client queue is: %d\n", ncq);
            client_queues[++i] = malloc(sizeof(int) * ncq);
        }
        else{
            client++;
        }
    }

    fseek(fp, 0, SEEK_SET);
    
    int j, l, k, w, d, t, temp;
    i = -1; // Keep track of the number of bank accounts
    l = -1; // Keep track of the number of the client
    k = 0; // Keep track of the number of requests for each client
    w = num_of_acc + 1; //Set the a withdraw code to the number of account + 1
    d = num_of_acc + 2; //Set the a deposit code to the number of account + 2
    t = num_of_acc + 3; //Set the a transfer code to the number of account + 3
    while (getline(&buffer, &len, fp) != -1){
        j = 0;
        tok = strtok(buffer, delim);
        // printf("The current string is %s\n",tok);
        if(tok[0] == 'c'){ //Only increment the count when the first character is 'c'
            ++l;
            printf("Reading requests of client %d\n", l);
        }
        while (tok != NULL)
        {
            switch (tok[0])
            {
            case 'b': 
                tok = strtok(NULL, delim);
                b_queues[++i] = atoi(tok); //Store the amount of money to the bank account
                temp = atoi(tok);
                printf("Putting money: %d to account %d\n", b_queues[i], i);
                break;
            case 'w':
                k++;
                client_queues[l][j] = w; //Store the withdraw code as the request of the client in the request queue
                printf("Client %d wants to withdraw(%d) money ", l, client_queues[l][j]); 
                j++;
                tok = strtok(NULL, delim);
                temp = tok[1] - '0' - 1; //We extract the number of the desired account
                client_queues[l][j] = temp; // and store it in the request queue
                printf("from account %d ", client_queues[l][j]);
                j++;
                tok = strtok(NULL, delim);
                char *buf_w = strdup(tok);
                temp = atoi(buf_w); //Transform the amount of money to int type
                client_queues[l][j] = temp;
                printf("with %d\n", client_queues[l][j]);
                j++;
                free(buf_w);
                break;
            case 'd':
                k++;
                client_queues[l][j] = d; //Store the deposit code as the request of the client in the request queue
                printf("Client %d wants to deposit(%d) money ", l, client_queues[l][j]);
                j++; 
                tok = strtok(NULL, delim);
                temp = tok[1] - '0' - 1; //We extract the number of the desired account
                client_queues[l][j] = temp; // and store it in the request queue
                printf("to account %d ", client_queues[l][j]);
                j++;
                tok = strtok(NULL, delim);
                char *buf_d = strdup(tok);
                client_queues[l][j] = atoi(buf_d); //Transform the amount of money to int type
                printf("with %d\n", client_queues[l][j]);
                j++;
                free(buf_d);
                break;
            case 't':
                k++;
                client_queues[l][j] = t; //Store the transfer code as the request of the client in the request queue
                printf("Client %d wants to transfer(%d) money ", l, client_queues[l][j]);
                j++;
                tok = strtok(NULL, delim);
                temp = tok[1] - '0' - 1;    //We extract the number of the desired account we transfer money from
                client_queues[l][j] = temp; // and store it in the request queue
                printf("from account %d ", client_queues[l][j]);
                j++;
                tok = strtok(NULL, delim);
                temp = tok[1] - '0' - 1; //We extract the number of the desired account we transfer money to
                client_queues[l][j] = temp; // and store it in the request queue
                printf("to account %d ", client_queues[l][j]);
                j++;
                tok = strtok(NULL, delim);
                char *buf_t = strdup(tok);
                client_queues[l][j] = atoi(buf_t); //Transform the amount of money to int type
                printf("with %d\n", client_queues[l][j]);
                j++;
                free(buf_t);
                break;
            default:
                tok = strtok(NULL, delim);
                break;
            }
        }
        if(l >= 0){
        size_of_requests[l] = k;
        k = 0;
        printf("The num of requests for client %d is %d\n", l, size_of_requests[l]);
        }
    }
    //Get the number of requests made by all the clients
    for(int i = 0; i < num_of_client; i++){
        k += size_of_requests[i];
    }
    printf("The num of total requests is %d\n", k);
    inp->acc_b = b_queues;
    inp->client_queue = client_queues;
    inp->d = d;
    inp->w = w;
    inp->t = t;
    inp->num_of_request = k;
    inp->size_a = num_of_acc;
    inp->size_c = num_of_client;
    inp->size_of_requests = size_of_requests;
    fclose(fp);
    free(buffer);

    return 0;
}

void *withdraw_money(void *tran){
    transaction *trans = (transaction*)tran;
    long int *temp = (long int *)trans->thread;
    long int thread_name = *temp;
    int *bank_acc = trans->bank_acc;
    int account_from = trans->account_to;
    int amount = trans->amount;
    printf("\n\n ****From withdraw_money thread ID %ld****", thread_name);

    pthread_mutex_lock(&lock); // ENTRY REGION
    printf("\n current balance is %d", bank_acc[account_from]); //CRITICAL STARTS
    printf("\n Processing Thread ID %ld request for $%d", thread_name, amount);

    if (bank_acc[account_from] >= amount)
    {
        printf("\n Withdraw amount of %d is Authorized", amount);
        bank_acc[account_from] = bank_acc[account_from]- amount;
        printf("\n Updated balance %d\n", bank_acc[account_from]);
    }
    else
         printf("\n Request denied"); //CRITICAL ENDS

    pthread_mutex_unlock(&lock); // EXIT REGION
}

void *deposit_money(void *tran)
{
    transaction *trans = (transaction *)tran;
    long int *temp = (long int *)trans->thread;
    long int thread_name = *temp;
    int *bank_acc = trans->bank_acc;
    int account_to = trans->account_to;
    int amount = trans->amount;
    printf("\n\n ****From deposit_money thread ID %ld****", thread_name);

    pthread_mutex_lock(&lock);                             // ENTRY REGION
    printf("\n current balance is %d", bank_acc[account_to]); //CRITICAL STARTS
    printf("\n Processing Thread ID %ld request for $ %d", thread_name, amount);

    printf("\n Deposit amount of %d is Authorized", amount);
    bank_acc[account_to] = bank_acc[account_to] + amount;
    printf("\n Updated balance %d\n", bank_acc[account_to]); //CRITICAL ENDS

    pthread_mutex_unlock(&lock); // EXIT REGION
}

void *transfer_money(void *tran)
{
    transaction *trans = (transaction *)tran;
    long int *temp = (long int *)trans->thread;
    long int thread_name = *temp;
    int *bank_acc = trans->bank_acc;
    int account_from = trans->account_from;
    int account_to = trans -> account_to;
    int amount = trans -> amount;
    printf("\n\n ****From transfer_money thread ID %ld****", thread_name);

    pthread_mutex_lock(&lock);                             // ENTRY REGION
    printf("\n Transfer from account: current balance is %d", bank_acc[account_from]); //CRITICAL STARTS
    printf("\n Transfer to account: current balance is %d", bank_acc[account_to]);
    printf("\n Processing Thread ID %ld request for $ %d", thread_name, amount);

    if (bank_acc[account_from] >= amount)
    {
        printf("\n Withdraw amount of %d is Authorized", amount);
        bank_acc[account_from] = bank_acc[account_from] - amount;
        bank_acc[account_to] = bank_acc[account_to] + amount;
        printf("\n Transfer from account: Updated balance %d\n", bank_acc[account_from]);
        printf("\n Transfer to account: Updated balance %d\n", bank_acc[account_to]);
    }
    else
        printf("\n Request denied"); //CRITICAL ENDS

    pthread_mutex_unlock(&lock); // EXIT REGION
}

void print_result(int *bank_result, int bank_size){
    for(int i = 0; i < bank_size; i++){
        printf("a%d b %d\n", i + 1, bank_result[i]);
    }
};