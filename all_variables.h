#ifndef all_variables

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int * res_arr, * avail_res, ** req_arr, ** alloc_res, deadlock_interval, res_no, max_threads, heuristic_type, D_no;

pthread_mutex_t mutex;



#define all_variables
#endif