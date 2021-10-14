#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include"all_variables.h"
#include"deadlock.h"

int main(int argc, char * argv[]) {
    if (argc < 6) {
        printf("\nToo few Arguements. Please refer to the respective readme for running the simulation properly\n");
        exit(-1);
    }
    printf("\nSimulator Started. To terminate the program after testing press CTRL+C\n\n");
    max_threads = atoi(argv[1]);
    heuristic_type = atoi(argv[2]);
    deadlock_interval = atoi(argv[3]);
    res_no = argc - 4;
    D_no = 0;


    avail_res = (int * ) malloc(sizeof(int) * res_no);
    req_arr = (int ** ) malloc(sizeof(int * ) * res_no);
    alloc_res = (int ** ) malloc(sizeof(int * ) * max_threads);
    res_arr = (int * ) malloc(sizeof(int) * res_no);

    for (int i = 0; i < max_threads; ++i) {
        req_arr[i] = (int * ) malloc(res_no * sizeof(int));
        alloc_res[i] = (int * ) malloc(res_no * sizeof(int));
    }

    for (int i = 4; i < argc; i++) {
        res_arr[i - 4] = atoi(argv[i]);
        avail_res[i - 4] = res_arr[i - 4];
    }
    pthread_t * threads = (pthread_t * ) malloc(max_threads * sizeof(pthread_t));
    int * t = (int * ) malloc(sizeof(int));


    pthread_t deadlock_thread;
    pthread_create( & deadlock_thread, NULL, deadlock_thread_func, threads);
    

    for (int i = 0; i < max_threads; i++) {
        * t = i;
        pthread_create( & threads[i], NULL, thread_work_func, t);
    }

    pthread_join(deadlock_thread, NULL);


    for (int i = 0; i < max_threads; i++)
        pthread_join( threads[i], NULL);
    

    return 0;
}