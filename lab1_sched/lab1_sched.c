/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395
*	    Student name : Kim EunJin, Lee WonJoo
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contain scheduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <fcntl.h> - Why included for 2 times?
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

#define TIMESLICE 4

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

/* Temporary functions for checking purposes */
void print_workload(PROCESS* p_array, int p_num);
/* Need to be deleted before writting report */

//timer variable for all the functions
int time = 0;

int sched_FIFO(PROCESS* p_array, int p_num)
{
	//First-In-First-Out
	time = 0; //time is always 0 at first
}

int sched_RoundRobin(PROCESS* p_array, int p_num)
{
	//Round_Robin
	time = 0; //time is always 0 at first
}

int sched_MLFQ(PROCESS* p_array, int p_num)
{
	//Muti-Level-Feedback-Queue
	time = 0; //time is always 0 at first
}

int sched_Lottery(PROCESS* p_array, int p_num)
{
	//Lottery
	time = 0; //time is always 0 at first
}

/* Temporary functions for checking purposes */
void print_workload(PROCESS* p_array, int p_num)
{
	int i;
	for(i=0; i<p_num; i++) {
		printf("Process - %c :\n", p_array[i]->name);
		printf("Arival time : %d \t \\ Servicetime : %d \n", p_array[i]->t_arrival, p_array[i]->t_service);
	}
}

