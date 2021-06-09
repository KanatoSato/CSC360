/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

static resource_t codeword;
char used_codeword[25];

pthread_mutex_t m;
pthread_cond_t bq;

int count;
int generation;
int cap;
int teller;

void initialize_meetup(int n, int mf) {
    char label[100];
    int i;
   
    // int n is barrier for n threads
    //pthread_mutex_t m; // mutex
    //pthread_cond_t bq; // condition variable

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }
    cap = n;
    teller = mf;

    init_resource(&codeword, "codeword");


    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
}


void join_meetup(char *value, int len) {
	//printf("%d\n",cap);
	//printf("Number of Bobas: %d, Word provider: %s",argv[2],argv[3]);
    	//printf("NOTHING IMPLEMENTED YET FOR join_meetup\n");
	pthread_mutex_lock(&m);
	count = count + 1;
	if (count < cap) {
		int my_generation = generation;
		if (teller == 1 && count == 1) {
			write_resource(&codeword, value, len);
		}
		while (my_generation == generation) {
			pthread_cond_wait(&bq, &m);
		}

	
	}
	else {
		if (teller == 0) {
			write_resource(&codeword, value, len);
		}
		count = 0;
		generation++;
		pthread_cond_broadcast(&bq);
	}
	pthread_mutex_unlock(&m);
	read_resource(&codeword, value, len);
	//memcpy(used_codeword, codeword->value, strlen(&codeword->value)+1);
	printf("Group %d with codeword %s is now gone.\n",generation-1, used_codeword);

}
