#ifndef thread_work_func

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include"all_variables.h"
#include"deadlock.h"

void * thread_work_func(void * arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int th = * (int * ) arg;

    do{
        printf("Thread number initiated:  %d\n", th);

        for (int i = 0; i < res_no; i++) 
            req_arr[th][i] = rand() % (res_arr[i]);

        while (1) {
            sleep(0.1);
            int ind = rand() % res_no;

            pthread_mutex_lock( & mutex);
            if (req_arr[th][ind] == 0) {
                pthread_mutex_unlock( & mutex);
                continue;
            } else if (avail_res[ind] >= req_arr[th][ind]) {
                avail_res[ind] -= req_arr[th][ind];
                alloc_res[th][ind] += req_arr[th][ind];
                req_arr[th][ind] = 0;
            } else {

                req_arr[th][ind] -= avail_res[ind];
                alloc_res[th][ind] += avail_res[ind];
                avail_res[ind] = 0;
            }
            pthread_mutex_unlock( & mutex);

            int i;
            for (i = 0; i < res_no; i++) {
                if (req_arr[th][i] != 0)
                    break;
            }
            if (i == res_no) {
                sleep(((0.8 * ((float) rand() / RAND_MAX)) + 0.7) * deadlock_interval);
                pthread_mutex_lock( & mutex);
                for (int j = 0; j < res_no; j++) {
                    avail_res[j] += alloc_res[th][j];
                    alloc_res[th][j] = 0;
                }
                pthread_mutex_unlock( & mutex);
                break;
            }
        }
    }while(1);
    pthread_mutex_unlock( & mutex);
}



#define thread_work_func
#endif


