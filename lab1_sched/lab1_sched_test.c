/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395, 32162925
*	    Student name : Kim EunJin, Lee WonJoo, Yun SeulGi
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

int** result = NULL;
extern int process_num; //for number of processes
extern int t_total; //for number of processes
extern int head;
extern int tail;
extern char* char_result;

/*
 * you need to implement scheduler simlator test code.
 *
 */

//function for inserting processe info's into "p_array"
void insert_process(PROCESS* p_array[]);

int main(int argc, char *argv[]){
	//PROCESS *p_array; //struct process_

	//int i; //for loops

	printf("**** Scheduler simulator - FIFO, RR, MLFQ, Lottery ****\n");
	printf("Number of processes? : ");
	scanf("%d", &process_num);

	const int p_num = process_num; //to prevent any changes to process_num
	PROCESS *p_array[p_num]; //array for PROCESS*(struct process_*)

printf("main ");

	//Dinamic allocation and initialization is done in insertProcess()
	insert_process(p_array);

	char_result = (char*)calloc(t_total, sizeof(char));
	
	/* Now calls all the scheduling simulator */
	/* You can modify the args of the functions */

	//sched_FIFO(p_array);
	sched_RoundRobin(p_array);
	//sched_MLFQ(p_array);
	//sched_Lottery(p_array);

	return 0;
}

void insert_process(PROCESS* p_array[])
{
	//changes the head and tail value
	int i; //for loops
	const int p_num = process_num; //to prevent any changes to process_num
	
printf("allocate ");	
	allocate_memory(p_array);

	//Initialization process
	for(i=0; i<p_num; i++) {
		getchar(); //emptys the buffer
		printf("\nInput the name(single char) of %dth process : ", i);
		scanf("%c", &p_array[i]->name);
		printf("Input arrival time of %c : ", p_array[i]->name);
		scanf("%d", &p_array[i]->t_arrival);
		printf("Input service time of %c : ", p_array[i]->name);
		scanf("%d", &p_array[i]->t_service);
		printf("Input priority of %c (for Lotterty sched) : ", p_array[i]->name);
		scanf("%d", &p_array[i]->priority);
		//t_process is always initialize with the value of t_service
		p_array[i]->t_process = p_array[i]->t_service;
		t_total += p_array[i]->t_service;
	}

printf("initialize done ");	
	return;
}
