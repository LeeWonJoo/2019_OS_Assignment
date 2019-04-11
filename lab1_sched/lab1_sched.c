/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395, 32162925
*	    Student name : Kim EunJin, Lee WonJoo, Yun SeulGi
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
#define QUEUE_NUM 5
/* We defined the TIMESLICE size as 4 !!
 * We defined the # of queues in MLFQ as 5 !! */

extern int** result;
int process_num = 0;
int t_total = 0;
char* char_result;

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */




/* Temporary functions for checking purposes */
void print_workload(PROCESS* p_array[]);
//print the char_result
int printQ()
{
	const int total = now; //to prevent any changes to process_num
printf("result is : ");
	for(int i=0; i<t_total; i++) {
		printf("%c ", char_result[i]);
	}
printf("\nnow is %d\n", now);
	return 0;
}
//print the queue
int printQQ(PROCESS* Queue[])
{
	const int p_num = process_num; //to prevent any changes to process_num
	int index = (head+1)%p_num;
printf("Queue now: ");
	while(1){
		printf("%c ", Queue[index]->name);
		if(index == tail) break;
		index = (index+1)%p_num;
	}
printf("\n");
	return 0;
}

/* Need to be deleted before writting report */




//timer variable for all the functions
int now = 0;

//to manage index of Q's head and tail
int head = 0; //Nothing in Queue
int tail = 0; //Nothing in Queue

int sched_FIFO(PROCESS* p_array[])
{
	//First-In-First-Out
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	const int p_num = process_num; //to prevent any changes to process_num
	//int i; //for loops
//forErrorDetection	
print_workload(p_array);

	//PROCESS *Queue[p_num]; //Queue for managing processes
	
	//for(i=0; i<p_num; i++) {
	//}
	return 0;
}

int sched_RoundRobin(PROCESS* p_array[])
{
	/* Round_Robin
	 * int income is for the index of p_array
	 * if income == 0 again(finished the pushes) and the q is empty,
	 * the now value is turned to -1 and ends the function. */
	int income = 0;
	int detect; //for error detection
	int flag, k=0; //for some use...
	int will_add; //value will be added to the "new"
	const int p_num = process_num; //to prevent any changes to process_num
	
	PROCESS *Queue[p_num]; //Queue for managing processes
	PROCESS *tmpQ = NULL;
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	
	initialize_memory(Queue);

	while(1) { //this loop is done till the end of execution
		int i;
		will_add = 0; //value to be added to "now"
		flag = 0;
		detect = 0;
		if(head ^ tail) { //when Queue has any value -> head and tail different
			int	tmp = (head+1)%p_num;
			tmpQ = Queue[tmp]; //temporary save the head value
			detect = RRpop(Queue);
			k=0;
	
			switch(detect) { //Error detection
				case 0:
					//process is !!not!! done with pop
					will_add = TIMESLICE;
					break;
				case -1:
					//Empty Queue(head == tail)
					//change of will_add does not happens
					//so, the value of will_add will be 0
					printf("\nError :Empty Queue!!\n");
					break;
				default:
					//other (num > 0) : process is done with pop
					will_add = detect;
					tmpQ = NULL;
			}
		}
		//so, if Queue is not empty == any process is executed == will_add will not be 0
		//but will_add will be 0 at the first time..
		//also when func is finished & queue is empty

		for(i=0; ; i++) { //check for any new process during the execution
			while( (income!=p_num) && (p_array[income]->t_arrival == (now+i)) ) {
				//this is done when income!=0 and p_array[income] is arrived
				detect = RRpush(Queue, p_array, NULL, income);
				switch(detect) { //Error detection
					case -1:
						printf("\nError :Full Queue!!\n");
						break;
					default:
						flag = i;
				}
				income++;
			} //ends when income==0 or p_array[income]->t_arrival != (now+i)
			
			if( (will_add) && (i == (will_add-1)) ) {
				//case when we have done a pop (will_add != 0)
				//when will_add != 0 but i!= will_add-1, loop will continue.
				break;
			}
			else if( (!will_add) && flag ) {
				//case when will_add == 0 and flag != 0
				//when will_add==0 but flag==0, loop will continue.
				will_add = flag;
				break;
			}
			else if( (!will_add) && (!flag) ) {
				//case when will_add == 0 and flag == 0
				//when will_add==0 but flag==0, loop will continue.
				(k == 0) ? (k=1) : (will_add=1, k=0);
				break;
			}
		}

		if(tmpQ != NULL) {
			//if the job was not done, push the job agin into the Queue.
			RRpush(Queue, NULL, tmpQ, -1);
		}
		now += will_add;

		if( (income == p_num) && (head == tail) ) {
			//nothing for new pushs & Queue empty means the end of the scheduling.
			break; //now breaks the loop!
		}
	}
	return 0;
}

int sched_MLFQ(PROCESS* p_array[])
{
	//Muti-Level-Feedback-Queue
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	const int p_num = process_num; //to prevent any changes to process_num
	return 0;
}

int sched_Lottery(PROCESS* p_array[])
{
	//Lottery
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	const int p_num = process_num; //to prevent any changes to process_num
	return 0;
}

/* Temporary functions for checking purposes */
void print_workload(PROCESS* p_array[])
{
	int i;
	const int p_num = process_num; //to prevent any changes to process_num
	
	for(i=0; i<p_num; i++) {
		printf("\nProcess - %c :\n", p_array[i]->name);
		printf("Arival time : %d \tService time : %d \n", p_array[i]->t_arrival, p_array[i]->t_service);
		printf("Process time : %d \tPriority : %d \n", p_array[i]->t_process, p_array[i]->priority);
	}
	return;
}
/*
void insert_in_Queue(PROCESS* Queue[], PROCESS* p_array[])
{
	int i; //for loops
	
	//Initialization process
	for(i=0; i<p_num; i++) {
		Queue[i]->name = p_array[i]->name;
		Queue[i]->t_arrival = p_array[i]->t_arrival;
		Queue[i]->t_service = p_array[i]->t_service;
		Queue[i]->t_process = p_array[i]->t_process;
	}
	return;
}
*/

int RRpop(PROCESS* Queue[])
{
	/* pop returns the value that will be added to "now" value after the pop func.
	 * pop returns the integer code for the next works.
	 * 
	 * changes the head and tail value
	 * no need of extra index value
	 *
	 * types of return values:
	 *  other (num > 0) : process is done with pop
	 *  0 : process is !!not!! done with pop
	 * -1 : Empty Queue(head == tail)
	 */
	const int p_num = process_num; //to prevent any changes to process_num
	int tmp = (head+1)%p_num; //index of head value(NOT HEAD)
	int loop, r_value; //this will be the return value
	char is_name;
	
	if(head == tail) {
		printf("\nError -1 : Empty Queue\n");
		return -1;
	}

	is_name = Queue[tmp]->name;

	//process is executed
	if(Queue[tmp]->t_process > TIMESLICE) {
		Queue[tmp]->t_process -= TIMESLICE;
		r_value = 0;
		loop = TIMESLICE;
	} else {
		loop = r_value = Queue[tmp]->t_process;
		free(Queue[tmp]);
	}

	for(int i=0; i<loop; i++) {
		char_result[now+i] = is_name;
	}
	Queue[tmp] = NULL;
	head = (head+1)%p_num;
	return r_value;
}

int RRpush(PROCESS* Queue[], PROCESS* p_array[], PROCESS* temp, int index)
{
	/*
	 * returns 0 if succeded
	 *
	 * process that came now has the highest priority
	 * so, becomes the head of the queue
	 *
	 * types of errors:
	 * -1 : Full Queue
	 * -2 : Too few args
	 * -3 : Too many args
	 */
	const int p_num = process_num; //to prevent any changes to process_num
	int tmp;
	PROCESS* tmpQ = NULL;
	
	if( (p_array==NULL) && (temp == NULL) ){
		printf("\nError -2 : Too few args.\n");
		return -2;
	}
	else if( (p_array!=NULL) && (temp != NULL) ){
		printf("\nError -3 : Too many args.\n");
		return -3;
	}
	
	if(head == (tail+1)%p_num) {
		printf("\nError -1 : Full Queue\n");
		return -1;
	}

	if(p_array != NULL) { //insert New process into Queue
		if(head == tail) { //FIRST INPUT INTO QUEUE
			head = 0;
			tmp = tail = 1;
			tmpQ = p_array[index];
		} else {
			tmp = head;
			head = (head+p_num-1)%p_num;
			tmpQ = p_array[index];
		}
		Queue[tmp] = (PROCESS*)malloc(sizeof(PROCESS));
		copy_data(&Queue[tmp], &tmpQ);
	}
	else { //insert Old process back into the Queue
		tmp = tail = (tail+1)%p_num;
		tmpQ = temp;
		Queue[tmp] = tmpQ;
	}
	return 0;
}

void print_sched()
{
	const int p_num = process_num; //to prevent any changes to process_num
	//how?
	//with what?
}

void allocate_memory(PROCESS* array[])
{
	int i; //for loops
	const int p_num = process_num; //to prevent any changes to process_num
	
	//Dinamic allocation of memory
	for(i=0; i<p_num; i++) {
		array[i] = (PROCESS*)malloc(sizeof(PROCESS));
	}
	return;
}

void initialize_memory(PROCESS* array[])
{
	int i; //for loops
	const int p_num = process_num; //to prevent any changes to process_num
	
	//Dinamic allocation of memory
	for(i=0; i<p_num; i++) {
		array[i] = NULL;
	}
	return;

}

int copy_data(PROCESS** p1, PROCESS** p2)
{
	//copying the data from p2 to p1
	//need to call with index!!
	//ex. copy_data(Queue[1], p_array[1]);
	(*p1)->name = (*p2)->name;
	(*p1)->t_arrival = (*p2)->t_arrival;
	(*p1)->t_service = (*p2)->t_service;
	(*p1)->t_process = (*p2)->t_process;
	(*p1)->priority = (*p2)->priority;
}
