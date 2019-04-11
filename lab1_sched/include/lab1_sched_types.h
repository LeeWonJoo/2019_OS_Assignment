/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395, 32162925
*	    Student name : Kim EunJin, Lee WonJoo, Yun SeulGi
*
*   lab1_sched_types.h :
*       - Lab1 header file.
*       - Must contain scheduler algorithm function's declations.
*       + ALSO contains structure infomation used in the functions.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */

typedef struct process_ {
	//name of the structure is set as "process_" for just in case
	char name;
	int t_arrival;
	int t_service;
	int t_process; //for checking remaining servicetime
	int priority; //this is used in Lottery sched!!
}PROCESS;

int sched_FIFO(PROCESS* p_array[]);
int sched_RoundRobin(PROCESS* p_array[]);
int sched_MLFQ(PROCESS* p_array[]);
int sched_Lottery(PROCESS* p_array[]);

//for Dinamic allocation of memory in RR, MLFQ, ...
void allocate_memory(PROCESS* array[]);

void initialize_memory(PROCESS* array[]); //initialize memory to NULL
int copy_data(PROCESS** p1, PROCESS** p2); //copying the data

void print_sched(); //how?

//these funcs will be turned into void funcs at the end of the project(not fixed)
int RRpop(PROCESS* Queue[]); //this is used in rr
int RRpush(PROCESS* Queue[], PROCESS* p_array[], PROCESS* temp, int index); //this is used in rr



/*Do we need to >>>>sort the workloads<<<< in t_arrival order? */

#endif /* _LAB1_HEADER_H*/

