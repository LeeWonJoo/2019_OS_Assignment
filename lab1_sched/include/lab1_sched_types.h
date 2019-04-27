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
	int ticket; //this is used in Lottery sched!!
//	int rate; //this is used in RateMonitoring sched!!
}PROCESS;

int sched_FIFO(PROCESS* p_array[]);
int sched_RoundRobin(PROCESS* p_array[]);
int sched_MLFQ(PROCESS* p_array[], int q_slice);
int sched_Lottery(PROCESS* p_array[]);
int sched_SJF(PROCESS* p_array[]);
int sched_RM(PROCESS* p_array[]);

void print_sched(PROCESS* p_array[]); //how?

//for Dinamic allocation of memory in RR, MLFQ, ...
void allocate_memory(PROCESS* array[]);

void initialize_memory(PROCESS* array[]); //initialize memory to NULL
void initialize_results(void); //initialize both result arrays
int copy_data(PROCESS** p1, PROCESS** p2); //copying the data

//these can be used in other scheds too.
int pop(PROCESS* Queue[], int exe_time); 
int push(PROCESS* Queue[], PROCESS* p_input, int index);

//this is only used in FIFO
int FIFOpush(PROCESS* Queue[], PROCESS* p_input);

//this is only used in SJF
void search_and_sort(PROCESS* Queue[]);

char Lotpush(int pu,PROCESS* array[],int income);
int Lotpop(PROCESS* array[],int p_num);
int Lottery_push(PROCESS* Queue[], PROCESS* p_input, int index);

//Theses functions are only used in MLFQ
int periodic_boost(PROCESS** Queue[]); //this is only used in MLFQ
int MLFQpop(PROCESS* Queue[], int index, int exe_time); //this is only used in MLFQ
int MLFQpush(PROCESS* Queue[], PROCESS* p_input, int index, int sign); //this is only used in MLFQ
int get_timeslice(int q_slice, int num);

/*Do we need to >>>>sort the workloads<<<< in t_arrival order? */

#endif /* _LAB1_HEADER_H*/

