/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395
*	    Student name : Kim EunJin, Lee WonJoo
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contain scheduler algorithm test code.
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

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
	int p_num; //for number of processes
	PROCESS *p_array; //struct process_

	int i; //for loops

	printf("**** Scheduler simulator - FIFO, RR, MLFQ, Lottery ****\n");
	printf("Number of processes? : ");
	scanf("%d", &p_num);
	//Dinamic allocation of memory
	p_array = (int*)malloc(sizeof(PROCESS) * p_num);

	printf("Input all the names of processes together : ");

	for(i=0; i<p_num; i++) {
		printf("Input the name(single char) of %dth process : ", i);
		scanf("%c", &p_array[i]->name);
		printf("Input arrival time of %c : ", p_array[i]->name);
		scanf("%d", &p_array[i]->t_arrival);
		printf("Input service time of %c : ", p_array[i]->name);
		scanf("%d", &p_array[i]->t_service);			
	}

	/* Now calls all the scheduling simulator */
	/* You can modify the name and args of the functions */
	
	//sched_FIFO(p_array, p_num);
	//sched_RoundRobin(p_array, p_num);
	//sched_MLFQ(p_array, p_num);
	//sched_Lottery(p_array, p_num);

	return 0;
}
