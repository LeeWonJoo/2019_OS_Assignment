/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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
	int *process; //for temporary
	char *p_name; //for temporary
	/*need to make a structure for process!!!*/

	int i; //for loops

	printf("**** Scheduler simulator - FIFO, RR, MLFQ, Lottery ****\n");
	printf("Number of processes? : ");
	scanf("%d", &p_num);
	//Dinamic allocation of memory
	process = (int*)malloc(sizeof(int) * p_num);
	p_name = (char*)malloc(sizeof(char) * p_num);

	printf("Input all the names of processes together : ");
	for(i=0; i<p_num; i++) {
		scanf("%c", &p_name[i]);
	}

}

