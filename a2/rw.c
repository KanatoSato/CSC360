/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */
sem_t R, W;
int readers = 0;
int writers = 0;
static resource_t data;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    if (sem_init(&R, 0, 0) != 0){
        perror("Something went wtong initializing semaphore for reader.\n");
        exit(1);
    }
    if (sem_init(&W, 0, 0) != 0){
        perror("Something went wtong initializing semaphore for writer.\n");
        exit(1);
    }
    init_resource(&data, "Read Write Problem");
    printf("Succeeded declaring semaphores\n");
}


void rw_read(char *value, int len) {
	while (!(writers == 0)) {
		sem_wait(&R);
	}
	readers++;
	sem_post(&R);
 	read_resource(&data, value, len);
	//printf("Read %s\n",value);
	//sleep(1);
	print_stats(&data);
	//sleep(1);
	if (--readers == 0) {
		sem_post(&W);	
	}
}


void rw_write(char *value, int len) {
    	while ((writers == 0) && !(readers == 0)) {
		sem_wait(&W);
	}
	writers++;
	write_resource(&data, value, len);
	//printf("Written %s\n",value);
	//sleep(1);
	print_stats(&data);
	//sleep(1);
	if (--writers == 0) {
		sem_post(&R);
	}
	

}
