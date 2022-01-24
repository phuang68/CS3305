/* 	Author: Pu Huang
	Description: Assignment_5+6
	Course: CS3305A
	Date: Dec 1st, 2020
*/
#define FILENAME_INPUT "assignment_6_input_file.txt"
#define FILENAME_OUTPUT "assignment_6_output_file.txt"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "read_input.h"
#include "types.h"
pthread_mutex_t lock;
#define SIZEOF(a) ((int *)(&a + 1) - (int *)(&a))

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s input.txt\n", argv[0]);
        exit(1);
    }
    input_queues inp;
    if (access(FILENAME_OUTPUT, F_OK) != -1)
    {
        remove(FILENAME_OUTPUT);
    }
    int fp_outfile = open(FILENAME_OUTPUT, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    //Read input file and parse the file into 
    read_input_file(argv[1], &inp);
    dup2(fp_outfile, STDOUT_FILENO);

    int *acc_b = inp.acc_b;
    int **client_queue = inp.client_queue;
    int w = inp.w;
    int d = inp.d;
    int t = inp.t;
    int num_threads = inp.num_of_request;
    int size_of_client = inp.size_c;
    int *size_of_requests = inp.size_of_requests;
    pthread_t *threads = malloc(sizeof(int) * num_threads);

    printf("The code for withdraw is %d\n", w);
    printf("The code for deposit is %d\n", d);
    printf("The code for transfer is %d\n", t);

    int i, err_thread;
    int t_num = 0;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    transaction *trans = malloc(sizeof(int) * num_threads);

    printf("The size is %d and the number of requests is %d\n", size_of_client, num_threads);

    for(int i = 0; i < size_of_client; i++){
        int s_of_r = size_of_requests[i];
        int j = 0;
        int account_f, account_t;
        int amount;  
        while (j < s_of_r)
        {
            if(client_queue[i][j] == w)
            { //Read the client request queue and Perform a withdraw action
                trans[t_num].bank_acc = acc_b;
                trans[t_num].thread = threads[t_num];
                j++;
                account_f = client_queue[i][j];
                trans[t_num].account_from = account_f;
                j++;
                amount = client_queue[i][j];
                trans[t_num].amount = amount;
                //Create a thread to perform the widraw action
                err_thread = pthread_create(&threads[t_num], NULL, withdraw_money, &trans[t_num]);
                t_num++;
                if (err_thread != 0)
                {
                    printf("\n Error creating thread %d", i);
                }
                j++;
                break;
            }
            else if(client_queue[i][j] == d){
                //Read the client request queue and Perform a deposit action
                trans[t_num].thread = threads[t_num];
                j++;
                account_t = client_queue[i][j];
                trans[t_num].account_from = account_t;
                j++;
                amount = client_queue[i][j];
                trans[t_num].amount = amount;
                //Create a thread to perform the deposit action
                err_thread = pthread_create(&threads[t_num], NULL, deposit_money, &trans);
                if (err_thread != 0)
                {
                    printf("\n Error creating thread %d", i);
                }  
                else
                {
                    t_num++;
                }
                j++;
            }
            else if (client_queue[i][j] == t){
                //Read the client request queue and Perform a transfer action
                trans[t_num].thread = threads[t_num];
                j++;
                account_f = client_queue[i][j];
                trans[t_num].account_from = account_f;
                j++;
                account_t = client_queue[i][j];
                trans[t_num].account_to = account_t;
                j++;
                amount = client_queue[i][j];
                trans[t_num].amount = amount;
                //Create a thread to perform the transfer action
                err_thread = pthread_create(&threads[t_num], NULL, deposit_money, &trans);
                t_num++;
                if (err_thread != 0)
                {
                    printf("\n Error creating thread %d", i);
                }else
                {
                    t_num++;
                }
                j++;
                break;
            }
         }
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock); // Destroy the lock once all the threads are done

    print_result(acc_b); //We print the result with the account

    return 0;
}