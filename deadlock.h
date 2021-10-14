#ifndef deadlock
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include"all_variables.h"


void * thread_work_func(void * arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int thread_index = * (int * ) arg;

    while (1) {
        for (int i = 0; i < res_no; i++) 
            req_arr[thread_index][i] = rand() % (res_arr[i]);

        while (1) {
            sleep(0.1);
            int random_index = rand() % res_no;

            pthread_mutex_lock( & mutex);
            if (avail_res[random_index] >= req_arr[thread_index][random_index]) {
                avail_res[random_index] -= req_arr[thread_index][random_index];
                alloc_res[thread_index][random_index] += req_arr[thread_index][random_index];
                req_arr[thread_index][random_index] = 0;
            } else if(req_arr[thread_index][random_index] !=0){

                req_arr[thread_index][random_index] -= avail_res[random_index];
                alloc_res[thread_index][random_index] += avail_res[random_index];
                avail_res[random_index] = 0;
            }
            pthread_mutex_unlock( & mutex);
            int flag = -1;
            for (int i = 0; i < res_no; i++) {
                if (req_arr[thread_index][i] != 0)
                {
                    flag = i;
                    break;
                }
            }
            if (flag == res_no) {
                sleep(((0.8 * ((float) rand() / RAND_MAX)) + 0.7) * deadlock_interval);
                pthread_mutex_lock( & mutex);
                for (int j = 0; j < res_no; j++) {
                    avail_res[j] += alloc_res[thread_index][j];
                    alloc_res[thread_index][j] = 0;
                }
                pthread_mutex_unlock( & mutex);
                break;
            }
        }
    }
    pthread_mutex_unlock( & mutex);
}



int deadlock_check_function(int work[], int finished[]) {
    int wip_res = 0;
    for (int i = 0; i < res_no; i++)
        work[i] = avail_res[i];
    
    for (int i = 0; i < max_threads; i++) {
        
        for (int j = 0; j <= res_no; j++) {
            if (req_arr[i][j] > work[j])
            {
                break;
            }
            if (j == res_no) {
                for (int k = 0; k < res_no; k++) {
                    work[k] += alloc_res[i][k];
                }
                finished[i] = 1;
                wip_res++;
            }   
        }
    }
    return wip_res;
}

void * deadlock_thread_func(void * arg) {
    pthread_t * thread = (pthread_t * ) arg;
    while (1) {
        sleep(deadlock_interval);
        printf("Checking Deadlock!! Please Wait\n");
        
        int work[res_no], finished[max_threads], finish[max_threads], wip_res = 0;
        memset(finished, 0, sizeof(finished));
        pthread_mutex_lock( & mutex);
        wip_res = deadlock_check_function(work, finished);
        if (wip_res != max_threads) {
            printf("Deadlock is detected and it is between the threads:  ");
            for (int i = 0; i < max_threads; i++) {
                if (!finished[i]) {
                    printf("<> (%d) <>", i + 1);
                }
            }
            printf("\n Resolving Issues with selected heuristic... Wait for a second\n");
            

            if (heuristic_type == 1) {
                printf("\nHeuristic Type 1 Working....\n");
                int freed = 0;
                for (int i = 0; i < max_threads; i++) {
                    if (!finished[i]) {
                        for (int j = 0; j < res_no; j++) {
                            avail_res[j] += alloc_res[i][j];
                            alloc_res[i][j] = 0;
                        }
                        freed++;
                        memset(finish, 0, sizeof(finish));
                        wip_res = deadlock_check_function(work, finish);
                        if (wip_res == max_threads){
                            printf("\nTotal Threads terminated are: %d\n", freed);
                            break;
                        }
                    }
                }
                for (int i = 0; i < max_threads && freed; i++) {
                    if (!finished[i]) {
                        pthread_cancel(thread[i]);
                        int * thread_index = (int * ) malloc(sizeof(int));
                        * thread_index = i;
                        pthread_create( & thread[i], NULL, thread_work_func , thread_index);
                        freed--;
                    }
                }
            } else if (heuristic_type == 2) {

                printf("\nHeuristic Type 2 Working....\n");

                int total_resources[max_threads], replace[max_threads];
                memset(total_resources, 0, sizeof(total_resources));
                memset(replace, 0, sizeof(replace));

                for (int i = 0; i < max_threads; i++) {
                    for (int j = 0; j < res_no; j++) {
                        total_resources[i] += alloc_res[i][j];

                    }
                }
                int total_freed = 0;
                while (wip_res != max_threads){
                    int max_blocks = 0;
                    for (int i = 1; i < max_threads; i++) {
                        if (total_resources[i] > total_resources[max_blocks])
                            max_blocks = i;
                    }

                    replace[max_blocks] = 1;
                    total_resources[max_blocks] = 0;

                    
                    for (int j = 0; j < res_no; j++) {
                        avail_res[j] += alloc_res[max_blocks][j];
                        alloc_res[max_blocks][j] = 0;
                    }
                    pthread_cancel(thread[max_blocks]);
                    int * thread_index = (int * ) malloc(sizeof(int));
                    * thread_index = max_blocks;
                    pthread_create( & thread[max_blocks], NULL, thread_work_func , thread_index);

                    memset(finish, 0, sizeof(finish));
                    wip_res = deadlock_check_function(work, finish);
                    total_freed++;
                } 
                printf("\nTotal Threads terminated are: %d\n", total_freed);
                
            }
        } else {
            printf("No DeadLock detected in the system!! All good!\n");
        }
        pthread_mutex_unlock( & mutex);
    }
}



#define deadlock
#endif