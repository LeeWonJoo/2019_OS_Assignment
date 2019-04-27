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

// We defined the TIMESLICE size as 4 !!
#define TIMESLICE 4

//We defined the # of queues in MLFQ as 5 !!
#define QUEUE_NUM 5
#define TIMEPERIOD 10
#define HIGHEST_Q 0


extern int** result;
int process_num = 0;
int t_total = 0;
char* char_result;

//timer variable for all the functions
int now = 0;

//to manage index of Q's head and tail
int head = 0; //Nothing in Queue
int tail = 0; //Nothing in Queue

//to manage the head and tail index of Q's of MLFQ
int Qhead[QUEUE_NUM] = {0, }; //Nothing in Queue
int Qtail[QUEUE_NUM] = {0, }; //Nothing in Queue





void initialize_results(void)
{
	const int p_num = process_num; //to prevent any changes to process_num
	const int total = t_total; //to prevent any changes to t_total
	
	for(int i=0; i<total; i++) {
		char_result[i] = 0;
	}
	for(int j=0; j<p_num; j++) {
		for(int i=0; i<total; i++) {
			result[j][i] = 0;
		}
	}
	return;
}

int sched_FIFO(PROCESS* p_array[])
{
	//First-In-First-Out
	int income = 0; //for push,new process
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	int detect;//for error detection
	int will_add = 0;
	const int p_num = process_num; //to prevent any changes to process_num
	//int i; //for loops
	
	PROCESS* Queue[p_num];	//Queue for managing processes
	
	initialize_memory(Queue);
	initialize_results();

	printf("\n*****************- FIFO Scheduling -*****************\n");

	while(1){	//this loop is done till the end of execution
		detect = 0;
		while((income != p_num)&&(p_array[income]->t_arrival <= now)){
			detect = FIFOpush(Queue, p_array[income]);
			switch(detect) { //Error detection
				case -1:
					printf("\nError :Full Queue!!\n");
					break;
				case -2:
					printf("\nError :Too few args!!\n");
					break;
			} //if push happens, detect==0
			income++;
		}

		detect = 0;
		if(head ^ tail){
			int tmp = (head+1)%p_num;
			int temp = Queue[tmp]->t_process;
			detect = pop(Queue, temp);
	
			switch(detect) { //Error detection
				case -1:
					//Empty Queue(head == tail)
					//change of will_add does not happens
					//so, the value of will_add will be 0
					printf("\nError :Empty Queue!!\n");
					break;
				default:
					//other (num > 0) : process is always done with pop
					will_add = detect;
			}
		}
		if(!will_add) will_add = 1;
		now += will_add;
		if((income == p_num)&&(head == tail)) break; //Queue empty means the end of the scheduling
	}
	return 0;
}


int sched_RoundRobin(PROCESS* p_array[])
{
	//Round_Robin
	//How does it works:
	//int income is for the index of p_array
	//if income == p_num(finished the pushes) and the q is empty,
	//the while loop breaks and function is finished.
	
	int income = 0;
	int detect; //for error detection
	int will_add; //value will be added to the "now"
	const int p_num = process_num; //to prevent any changes to process_num
	
	PROCESS *Queue[p_num]; //Queue for managing processes
	PROCESS *tmpQ = NULL;
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	
	initialize_memory(Queue);
	initialize_results();

	printf("\n*****************- RR Scheduling -*****************\n");

	while(1) { //this loop is done till the end of execution
		will_add = 0; //value to be added to "now"

		detect = 0;
		while( (income!=p_num) && (p_array[income]->t_arrival <= now) ) {
			//this is done when income!=p_num and p_array[income] is arrived
			detect = push(Queue, p_array[income], 1);
			switch(detect) { //Error detection
				case -1:
					printf("\nError :Full Queue!!\n");
					break;
				case -2:
					printf("\nError :Too few args!!\n");
					break;
			} //if push happens, detect==0
			income++;
		} //ends when income==p_num or p_array[income]->t_arrival != (now)

		detect = 0;
		if(head ^ tail) { //when Queue has any value -> head and tail different
			int tmp = (head+1)%p_num;
			tmpQ = Queue[tmp]; //temporary save the head value
			detect = pop(Queue, TIMESLICE);
	
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

		if(tmpQ != NULL) {
			//if the job was not done, push the job agin into the Queue.
			push(Queue, tmpQ, 0);
		}
		if(!will_add) will_add = 1;
		now += will_add;

		if( (income == p_num) && (head == tail) ) {
			//nothing for new pushs & Queue empty means the end of the scheduling.
			break; //now breaks the loop!
		}
	}
	return 0;
}

int sched_MLFQ(PROCESS* p_array[], int q_slice)
{
	// Muti-Level-Feedback-Queue
	// int income is for the index of p_array
	
	// How does it works:
	// there are QUEUE_NUMS of queue,
	// and Queue[0] gets the highest priority.
	// when Queue[0] is finished RR, processes in next Queue are executed.
	// so, the basic logic is similar to RR.
	// Queue boost is happened every TIMEPERIOD time.
	// if income == p_num (finished the pushes) and all the qs are empty,
	// the while loop breaks and function is finished. */
	const int p_num = process_num; //to prevent any changes to process_num
	int income = 0;
	int detect = 0; //for error detection
	int will_add = 1; //value will be added to the "now"
	int time_period = TIMEPERIOD; //this is to check for periodic_boost()
	int exe_time; //this is for execution time(work as timeslice)

	//MLQF needs special Queue and heads&tails
	PROCESS ***Queue = NULL; //Muliple Queues for managing processes
	int Qnum = 0; //this is for indexing of Queue
	
	PROCESS *tmpQ = NULL;
	now = 0; //time is always set 0 at first

	//Dinamic allocation & initializing of ***Queue...
	Queue = (PROCESS***)malloc(sizeof(PROCESS**)*QUEUE_NUM);
	for(int i=0; i<QUEUE_NUM; i++) {
		Queue[i] = (PROCESS **)malloc(sizeof(PROCESS*)*p_num);
		initialize_memory(Queue[i]);
	}
	initialize_results();

	printf("\n*****************- MLFQ Scheduling -*****************\n");
	printf("***********- timeslice per queue #i = %d^i -**********\n", q_slice);

	while(1) { //this loop is done till the end of execution
		int  j; //for loops
		will_add = 0; //value to be added to "now"
		Qnum = 0;

		//periodic_boost is done every TIMEPERIOD time.
		if( now >= time_period ) {
			periodic_boost(Queue);
			time_period += TIMEPERIOD;
		}

		detect = 0;
		//check for any new process during the execution	
		while( (income!=p_num) && (p_array[income]->t_arrival <= now) ) {
			//this is done when income!=p_num and p_array[income] is arrived
			detect = MLFQpush(Queue[HIGHEST_Q], p_array[income], HIGHEST_Q, 1);
			switch(detect) { //Error detection
				case -1:
					printf("\nError :Full Queue!!\n");
					break;
				case -2:
					printf("\nError :Too few args!!\n");
					break;
			}//if push happens, detect==0
			income++;
		} //ends when income==p_num or p_array[income]->t_arrival != (now)

		detect = 0;
		for(j=0; j<QUEUE_NUM; j++) {
			if(Qhead[j] ^ Qtail[j]) {
				//when Queue[j] has any value -> head and tail different
				int tmp = (Qhead[j]+1)%p_num;
				tmpQ = Queue[j][tmp]; //temporary save the head value

				exe_time = get_timeslice(q_slice, j);
				detect = MLFQpop(Queue[j], j, exe_time);
				switch(detect) { //Error detection
					case 0:
						//process is !!not!! done with pop
						will_add = exe_time;
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
				Qnum = j;
				break;
			}
		}
		//so, if any one of Queue is not empty
		// == any process is executed == will_add will not be 0
		//but will_add will be 0 at the first time..
		//also when func is finished & queue is empty

		if(tmpQ != NULL) {
			//if the job was not done, push the job into the !!NEXT!! Queue.
			Qnum = (Qnum==QUEUE_NUM) ? Qnum : (Qnum+1);
			MLFQpush(Queue[Qnum], tmpQ, Qnum, 0);
		}
		if(!will_add) will_add = 1;
		now += will_add;

		//check if all the queues are empty
		detect = 0;
		for(j=0; j<QUEUE_NUM; j++) {
			detect = (Qhead[j] != Qtail[j]) ? 1 : 0;
			if(detect) break;
		}
		if( (income == p_num) && (!detect) )  {
			//income == p_num & detect == 0 -> ALL Queue empty == the end of the scheduling.
			break; //now ends the loop!
		}
	}
	return 0;
}

int sched_Lottery(PROCESS* p_array[])
{
	//Lottery
	int income = 0;
	int detect; //for error detection
	const int p_num = process_num; //to prevent any changes to process_num

	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first

	int i,j;
	int winner;//selected process
	int totalticket=0;
	int temp=0,last=0;
	int st=p_array[0]->t_arrival;//start time

	for(i=1;i<p_num;i++){
		if(st>p_array[i]->t_arrival) st=p_array[i]->t_arrival;
	}
	//t_total+=st;

	PROCESS *Queue[p_num]; //Queue for managing processes

	initialize_memory(Queue);
	initialize_results();
printf("\n*****************- Lottery Scheduling -*****************\n");

	for(i=0;i<p_num;i++)    totalticket+=p_array[i]->ticket;
	char job[totalticket]; //sort by the size of each ticket

	//To put the process name in an array by the number of tickets  
	for(i=0;i<p_num;i++){
		for(j=0;j<p_array[i]->ticket;j++){
			job[j+temp]=p_array[i]->name;
			last=j+temp;
		}
		temp=last+1;
	}

	char rank[t_total];//selected process
	char queuelist[p_num];//processname in queue

	while(1){
		while(income!=p_num){
			//this is done when income!=p_num and p_array[income] is arrived
			detect = Lottery_push(Queue, p_array[income], 1);
			//processlist taht are push
			queuelist[income]=Lotpush(detect,p_array,income);
			income++;
		}
		if(income==p_num) break;
	}
	srand(time(NULL));
	//get winner
	for(i=0;i<t_total;i++){
		//rank is empty before the process begin
		if(i<st) {rank[i]='^';continue;}
		//check t_process==0(=end of performance)
		temp=Lotpop(p_array,p_num);

		if(temp!=-1){
			for(j=0;j<totalticket;j++)
				if(queuelist[temp]==job[j]) job[j]='^';
			queuelist[temp]='^';
		}
		winner=rand()%totalticket;
		int num=-1;//p_array index;
		//check the winner's process arrival time
		//if not, pull winner again
		for(j=0;j<p_num;j++)
			if(job[winner]==p_array[j]->name) num=j;
		if((p_array[num]->t_arrival) > i){i--; continue;}
		//if the process is not finished, put it in rank
		if(job[winner]!='^'){
			rank[i]=job[winner];
			for(j=0;j<p_num;j++)
				if(p_array[j]->name==job[winner]) {p_array[j]->t_process--;}
		}
		else i--;
	}
	//print result
	for(i=0;i<p_num;i++){
		printf("%4c : |",p_array[i]->name);
		for(j=0;j<t_total;j++){
			if(rank[j]==p_array[i]->name)
				printf("O|");
			else
				printf("-|");
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}



char Lotpush(int pu,PROCESS* array[], int income)
{
	if(pu==0)
		return array[income]->name;
	else
		return '^';
}

int Lotpop(PROCESS* array[],int p_num)
{
	int i;
	for(i=0;i<p_num;i++){
		if(array[i]->t_process==0){
			array[i]->t_process=-1;
			return i;
		}
	}
	return -1;
}

int Lottery_push(PROCESS* Queue[], PROCESS* p_input, int index)
{
        //returns 0 if succeded
        //process that came now has the highest priority
        //so, becomes the head of the queue
        //
        //index is 0 when p_input is new input
        //index is 1 when p_input is old process
        //
        //types of errors:
        //-1 : Full Queue
        //-2 : Too few args
         
        const int p_num = process_num; //to prevent any changes to process_num
        int tmp; 
        PROCESS* tmpQ = p_input;

        if( (p_input==NULL) ){
                printf("\nError -2 : Too few args.\n");
                return -2;
        }       
        
        if(index) { //insert New process into Queue -> index is not 0
                if(head == tail) { //FIRST INPUT INTO QUEUE
                        head = 0;
                        tmp = tail = 1;
                } else {
                        tmp = head;
                        head = (head+p_num-1)%p_num;
                }
                Queue[tmp] = (PROCESS*)malloc(sizeof(PROCESS));
                copy_data(&Queue[tmp], &tmpQ);
        }
        else { //insert Old process back into the Queue -> index is 0
                tmp = tail = (tail+1)%p_num;
                Queue[tmp] = tmpQ;
        }
        return 0;
}


/*
int sched_RM(PROCESS* p_array[])
{
	//Rate-Monitoring
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	const int p_num = process_num; //to prevent any changes to process_num
	
	initialize_results();
	printf("\n*****************- RM Scheduling -*****************\n");


	return 0;
}
*/

int sched_SJF(PROCESS* p_array[])
{
	//Shortest-Job-First
	int income = 0; //for push,new process
	head = 0; //Nothing in Queue
	tail = 0; //Nothing in Queue
	now = 0; //time is always set 0 at first
	int detect;//for error detection
	int will_add = 0;
	const int p_num = process_num; //to prevent any changes to process_num
	//int i; //for loops
	
	PROCESS* Queue[p_num];	//Queue for managing processes
	
	initialize_memory(Queue);
	initialize_results();

	printf("\n*****************- SJF Scheduling -*****************\n");

	while(1){	//this loop is done till the end of execution
		detect = 0;
		while((income != p_num)&&(p_array[income]->t_arrival <= now)){
			detect = push(Queue, p_array[income], 1);
			switch(detect) { //Error detection
				case -1:
					printf("\nError :Full Queue!!\n");
					break;
				case -2:
					printf("\nError :Too few args!!\n");
					break;
			} //if push happens, detect==0
			income++;
		}

		detect = 0;
		if(head ^ tail){
			search_and_sort(Queue);

			int tmp = (head+1)%p_num;
			int temp = Queue[tmp]->t_process;
			detect = pop(Queue, temp);
	
			switch(detect) { //Error detection
				case -1:
					//Empty Queue(head == tail)
					//change of will_add does not happens
					//so, the value of will_add will be 0
					printf("\nError :Empty Queue!!\n");
					break;
				default:
					//other (num > 0) : process is always done with pop
					will_add = detect;
			}
		}
		if(!will_add) will_add = 1;
		now += will_add;
		if((income == p_num)&&(head == tail)) break; //Queue empty means the end of the scheduling
	}
	return 0;
}


void search_and_sort(PROCESS* Queue[])
{
	const int p_num = process_num; //to prevent any changes to process_num
	int i = (head+1)%p_num;
	int k = i;
	
	if(i == tail)
		return;

	while(1) {
		while(1) {
			if(Queue[i]->t_process > Queue[k]->t_process) {
				PROCESS* tmpQ = Queue[i];
				Queue[i] = Queue[k];
				Queue[k] = tmpQ;
			}
			if(k == tail)
				break;
			k = (k+1)%p_num;
		}
		if( i == (tail-1+p_num)%p_num )
			break;
		i = (i+1)%p_num;
	}
	return;
}

int pop(PROCESS* Queue[], int exe_time)
{
	// pop returns the value that will be added to "now" value after the pop func.
	// pop returns the integer code for the next works.
	 
	// changes the head and tail value
	// no need of index value
	
	// types of return values:
	//  other (num > 0) : process is done with pop
	//  0 : process is !!not!! done with pop
	// -1 : Empty Queue(head == tail)
	
	const int p_num = process_num; //to prevent any changes to process_num
	int tmp = (head+1)%p_num; //index of head value(NOT HEAD)
	int loop, r_value; //this will be the return value

	int Tslice = exe_time;
	char is_name;
	
	if(head == tail) {
		printf("\nError -1 : Empty Queue\n");
		return -1;
	}

	is_name = Queue[tmp]->name;

	//process is executed
	if(Queue[tmp]->t_process > Tslice) {
		Queue[tmp]->t_process -= Tslice;
		r_value = 0;
		loop = Tslice;
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

int push(PROCESS* Queue[], PROCESS* p_input, int index)
{
	//returns 0 if succeded
	//process that came now has the highest priority
	//so, becomes the head of the queue
	//
	//types of errors:
	//-1 : Full Queue
	//-2 : Too few args

	const int p_num = process_num; //to prevent any changes to process_num
	int tmp;
	PROCESS* tmpQ = p_input;
	
	if( p_input == NULL ){
		printf("\nError -2 : Too few args.\n");
		return -2;
	}
	
	if(head == (tail+1)%p_num) {
		printf("\nError -1 : Full Queue\n");
		return -1;
	}

	if(index) { //insert New process into Queue  -> index is not 0
		if(head == tail) { //FIRST INPUT INTO QUEUE
			head = 0;
			tmp = tail = 1;
		} else {
			tmp = head;
			head = (head+p_num-1)%p_num;
		}
		Queue[tmp] = (PROCESS*)malloc(sizeof(PROCESS));
		copy_data(&Queue[tmp], &tmpQ);
	}
	else { //insert Old process back into the Queue
		tmp = tail = (tail+1)%p_num;
		Queue[tmp] = tmpQ;
	}
	return 0;
}

int FIFOpush(PROCESS* Queue[], PROCESS* p_input)
{
	//returns 0 if succeded
	//process that came now has the highest priority
	//so, becomes the head of the queue
	//
	//types of errors:
	//-1 : Full Queue
	//-2 : Too few args

	const int p_num = process_num; //to prevent any changes to process_num
	int tmp;
	PROCESS* tmpQ = p_input;
	
	if( p_input == NULL ){
		printf("\nError -2 : Too few args.\n");
		return -2;
	}
	
	if(head == (tail+1)%p_num) {
		printf("\nError -1 : Full Queue\n");
		return -1;
	}

	if(head == tail) { //FIRST INPUT INTO QUEUE
		head = 0;
		tmp = tail = 1;
	} else {
		tmp = tail = (tail+1)%p_num;
	}
	Queue[tmp] = (PROCESS*)malloc(sizeof(PROCESS));
	copy_data(&Queue[tmp], &tmpQ);

	return 0;
}

/*PUSH AND POP SPECIALY FOR MLFQ*/
int MLFQpop(PROCESS* Queue[], int index, int exe_time)
{
	// pop returns the value that will be added to "now" value after the pop func.
	// pop returns the integer code for the next works.
	
	// changes the head and tail value
	// no need of extra index value
	
	// types of return values:
	//  other (num > 0) : process is done with pop
	//  0 : process is !!not!! done with pop
	// -1 : Empty Queue(head == tail)
	
	const int p_num = process_num; //to prevent any changes to process_num
	int tmp = (Qhead[index]+1)%p_num; //index of head value(NOT HEAD)
	int loop, r_value; //this will be the return value
	int Tslice = exe_time;
	char is_name;
	
	if(Qhead[index] == Qtail[index]) {
		printf("\nError -1 : Empty Queue\n");
		return -1;
	}

	is_name = Queue[tmp]->name;

	//process is executed
	if(Queue[tmp]->t_process > Tslice) {
		Queue[tmp]->t_process -= Tslice;
		r_value = 0;
		loop = Tslice;
	} else {
		loop = r_value = Queue[tmp]->t_process;
		free(Queue[tmp]);
	}

	for(int i=0; i<loop; i++) {
		char_result[now+i] = is_name;
	}
	Queue[tmp] = NULL;
	Qhead[index] = (Qhead[index]+1)%p_num;
	return r_value;
}

int get_timeslice(int q_slice, int num)
{
	int tslice = 1;
	int multiply = q_slice;

	if(multiply == 1) {
		//when no need for caculation..
		return 1;
	}

	for(int i=0; i<num; i++) {
		tslice *= multiply; 
	}
	return tslice;
}

int MLFQpush(PROCESS* Queue[], PROCESS* p_input, int index, int sign)
{
	// returns 0 if succeded
	//
	// process that came now has the highest priority
	// so, becomes the head of the queue
	//
	// types of errors:
	// -1 : Full Queue
	// -2 : Too few args
	
	const int p_num = process_num; //to prevent any changes to process_num
	int tmp;
	PROCESS* tmpQ = p_input;
	
	if(p_input == NULL){
		printf("\nError -2 : Too few args.\n");
		return -2;
	}
	
	if(Qhead[index] == (Qtail[index]+1)%p_num) {
		printf("\nError -1 : Full Queue\n");
		return -1;
	}

	switch(sign) {
		case 0: //insert Old process back into the Queue
			tmp = Qtail[index] = (Qtail[index]+1)%p_num;
			Queue[tmp] = tmpQ;
			break;

		case 1: //insert New process into the Queue
			if(Qhead[index] == Qtail[index]) { //FIRST INPUT INTO QUEUE
				Qhead[index] = 0;
				tmp = Qtail[index] = 1;
			} else {
				tmp = Qhead[index];
				Qhead[index] = (Qhead[index]+p_num-1)%p_num;
			}
			Queue[tmp] = (PROCESS*)malloc(sizeof(PROCESS));
			copy_data(&Queue[tmp], &tmpQ);
			break;
	}
	return 0;
}

int periodic_boost(PROCESS** Queue[])
{
	const int p_num = process_num; //to prevent any changes to process_num
	int temp = 0;
	int i=0;

	while(i<QUEUE_NUM) {
		if(Qhead[i] != Qtail[i]) {
			temp = (Qhead[i]+1)%p_num;
			MLFQpush(Queue[HIGHEST_Q], Queue[i][temp], HIGHEST_Q, 1);
			Qhead[i] = temp;
			Queue[i][temp] = NULL;
		}
		else i++;
	}
	return 0;
}

void print_sched(PROCESS* p_array[])
{
	const int p_num = process_num; //to prevent any changes to process_num
	const int total = t_total; //to prevent any changes to t_total
	int _time_, index;
	//input the values to result array using char_result
	for(_time_=0; _time_<total; _time_++) {
		for(index=0; index<p_num; index++) {
			if(char_result[_time_] == p_array[index]->name)
				result[index][_time_] = 1;
		}
	}
	//print out the graph using result array
	for(index=0; index<p_num; index++) {
		printf("%4c : |", p_array[index]->name);
		for(_time_=0; _time_<total; _time_++) {
			if(result[index][_time_])
				printf("O|");
			else
				printf("-|");
		}
		printf("\n");
	}
	printf("\n");
	return;
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
	(*p1)->ticket = (*p2)->ticket;
//	(*p1)->rate = (*p2)->rate;
	return 0;
}
