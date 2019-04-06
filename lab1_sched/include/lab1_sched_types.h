/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170896, 32163395
*	    Student name : Kim EunJin, Lee WonJoo
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - Must contain scheduler algorithm function's declations.
*       + ALSO contains structure infomation used in the functions
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
				
}PROCESS;

int sched_FIFO(PROCESS* p_array, int p_num);
int sched_RoundRobin(PROCESS* p_array, int p_num);
int sched_MLFQ(PROCESS* p_array, int p_num);
int sched_Lottery(PROCESS* p_array, int p_num);

#endif /* _LAB1_HEADER_H*/



