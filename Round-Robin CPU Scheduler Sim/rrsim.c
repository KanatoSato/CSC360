#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_BUFFER_LEN 80

taskval_t *event_list = NULL;

void print_task(taskval_t *t, void *arg) {
    printf("task %03d: %5d %3.2f %3.2f\n",
        t->id,
        t->arrival_time,
        t->cpu_request,
        t->cpu_used
    );  
}


void increment_count(taskval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}


void run_simulation(int qlen, int dlen, taskval_t *list) {
    int d_count;               
    int q_count;
    int count = 0;                // count cpu cycles
    int cpu_count;                // store cpu used so far for a task
    int complete_time = 0;        // store time when a task is done
    
    taskval_t *running;           // store task info that is running
    taskval_t *waiting;           // store the next task info that is awaiting to get called up by ready_q 
    taskval_t *ready_q = NULL;    // linked list to hold tasks ready to run
	
    waiting = peek_front(list);           // First task to be put on ready_q
    list = remove_front(list);            // remove the first task from event list
    ready_q = add_end(ready_q,waiting);   // move the waiting task to ready_q
    running = peek_front(ready_q);        // run the task in ready_q
    ready_q = remove_front(ready_q);      // remove the running task from ready_q
    waiting = peek_front(list);           // Now waiting is the second task in event list
    list = remove_front(list);            // so remove the task from event list

    int len = 0;

    for(;;){
	if (running == NULL) {     // if nothing is running, which means ready_q is empty. 
		break;             // So stop the function.
	}

	if (count < running->arrival_time){ // Until count reaches arrival time of 
		                            // the current running tasks, print IDLE
	    printf("[%05d] IDLE\n",count);
	    count++;
	    continue;
	}

	cpu_count = running->cpu_used;        // Grab cpu_used of the current cpu_used
	running->finish_time = complete_time; // Keep placing 0 as finish_time until task finishes
	
	if (dlen == 0){                       // If dlen = 0, print Dispatch without increment
	    printf("[%05d] Dispatching\n",count);
	} 

	for (d_count=0; d_count < dlen; d_count++){ // If not increment by one.
	    printf("[%05d] Dispatching\n",count);

	    count++;
	}

	for (q_count=0; q_count < qlen; q_count++){
            printf("[%05d] id=%05d req=%.2f used%.2f\n",count,running->id,running->cpu_request,running->cpu_used);             // Print current status of running task
	    cpu_count = cpu_count + 1;
	    running->cpu_used = cpu_count;
	    count++;
	    if (cpu_count >= running->cpu_request) {  // If current cpu_used >= cpu_request, task is done.
		 complete_time = count;               // So store the finish time
		 float ta = count - running->arrival_time; // Calculate turnasround time
		 float w = ta - running->cpu_request;      // Calculate waiting time
	         printf("[%05d] id=%05d EXIT w=%.2f ta=%.2f\n",count,running->id,w,ta); // Print status of finished task 
		 running->finish_time = complete_time;
		 
		 len++; // len is just for counting number of tasks

		 break; // If task finishes while enough room for quantum, close quantum and move on  
	    }
	}

	if (waiting != NULL){    // waiting is no NULL means there's still some task to be put on ready_q
            if (waiting->arrival_time <= count) {  // If count is bigger than arrival time of waiting task, then push the task to ready_q
            	ready_q = add_end(ready_q,waiting);
                waiting = peek_front(list);

                list = remove_front(list);
            }

	}
	if (running->finish_time == 0) {  // If running task hasn't finished after quantum counts, push the task to ready_q
	     ready_q = add_end(ready_q,running);
	}
	d_count = 0;
    	q_count = 0;	
 	complete_time = 0;

	running = peek_front(ready_q); // After each quantum counts, pull task from ready_q
	ready_q = remove_front(ready_q); // Then remove the task from ready_q
    
    }

    
}


// This function does exactly the same thing as function above, but does not print anything exept the final result. This result is used for Task2 to analyze.
void run_simulation_result(int qlen, int dlen, taskval_t *list, float *avg_wait, float *avg_ta) {
    int d_count;
    int q_count;
    int count = 0;
    int cpu_count;
    int complete_time = 0;
    float total_wait = 0;
    float total_ta = 0;
    taskval_t *running;
    taskval_t *waiting;
    taskval_t *ready_q = NULL;

    waiting = peek_front(list);
    list = remove_front(list);
    ready_q = add_end(ready_q,waiting);
    running = peek_front(ready_q);
    ready_q = remove_front(ready_q);
    waiting = peek_front(list);
    list = remove_front(list);

    int len = 0;

    for(;;){
        if (running == NULL) {
                break;
        }
        assert(running != NULL);
        if (count < running->arrival_time){
            count++;
            continue;
        }

        cpu_count = running->cpu_used;
        running->finish_time = complete_time;

        if (dlen == 0){
            //printf("[%05d] Dispatching\n",count);
        }

        for (d_count=0; d_count < dlen; d_count++){
            //printf("[%05d] Dispatching\n",count);

            count++;
        }

	for (q_count=0; q_count < qlen; q_count++){
            //printf("[%05d] id=%05d req=%.2f used%.2f\n",count,running->id,running->cpu_request,running->cpu_used);
            cpu_count = cpu_count + 1;
            running->cpu_used = cpu_count;
            count++;
            if (cpu_count >= running->cpu_request) {
                 complete_time = count;
                 float ta = count - running->arrival_time;
                 float w = ta - running->cpu_request;
                 total_wait = total_wait + w;
                 total_ta = total_ta + ta;
                 //printf("[%05d] id=%05d EXIT w=%.2f ta=%.2f\n",count,running->id,w,ta);
                 running->finish_time = complete_time;
		 
		 len++;

                 break;
            }
        }
        if (waiting != NULL){
            if (waiting->arrival_time <= count) {
                ready_q = add_end(ready_q,waiting);
                waiting = peek_front(list);

                list = remove_front(list);
            }

        }
        if (running->finish_time == 0) {
             ready_q = add_end(ready_q,running);
        }
        d_count = 0;
        q_count = 0;
        complete_time = 0;

        running = peek_front(ready_q);
        ready_q = remove_front(ready_q);

    }

    *avg_wait = total_wait / len;
    *avg_ta = total_ta / len;

}


int main(int argc, char *argv[]) {
    char   input_line[MAX_BUFFER_LEN];
    int    i;
    int    task_num;
    int    task_arrival;
    float  task_cpu;
    int    quantum_length = -1;
    int    dispatch_length = -1;

    taskval_t *temp_task;
    float avg_wait = 0;
    float avg_ta = 0;


    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0 && i+1 < argc) {
            quantum_length = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--dispatch") == 0 && i+1 < argc) {
            dispatch_length = atoi(argv[i+1]);
        }
    }

    if (quantum_length == -1 || dispatch_length == -1) {
        fprintf(stderr, 
            "usage: %s --quantum <num> --dispatch <num>\n",
            argv[0]);
        exit(1);
    }


    while(fgets(input_line, MAX_BUFFER_LEN, stdin)) {
        sscanf(input_line, "%d %d %f", &task_num, &task_arrival,
            &task_cpu);
        temp_task = new_task();
        temp_task->id = task_num;
        temp_task->arrival_time = task_arrival;
        temp_task->cpu_request = task_cpu;
        temp_task->cpu_used = 0.0;
        event_list = add_end(event_list, temp_task);
    }
/*
#ifdef DEBUG
    int num_events;
    apply(event_list, increment_count, &num_events);
    //printf("DEBUG: # of events read into list -- %d\n", num_events);
    printf("DEBUG: value of quantum length -- %d\n", quantum_length);
    printf("DEBUG: value of dispatch length -- %d\n", dispatch_length);
    //peek_front(event_list);
    apply(event_list, print_task, NULL);
#endif
*/
    run_simulation(quantum_length, dispatch_length, event_list);
    //run_simulation_result(quantum_length, dispatch_length, event_list, &avg_wait, &avg_ta);
    //printf("quantum: %d, dispatch: %d, avg_wait: %.2f, avg_ta: %.2f\n",quantum_length,dispatch_length,avg_wait,avg_ta);
    return (0);
}
