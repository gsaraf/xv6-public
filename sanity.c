#include "types.h"
#include "stat.h"
#include "user.h"

volatile int vi;
volatile int* pi = &vi;

#define NUM_LOOPS 100
#define NUM_ITER 100000
void proc_cpu() {
	volatile int i,j ;
	for (i =0 ; i < NUM_LOOPS ; ++i);
	for (j =0 ; j < NUM_ITER ; ++j) {
		*pi = j;
	}
}

void proc_scpu() {
	volatile int i,j ;
	for (i =0 ; i < NUM_LOOPS ; ++i);
	for (j =0 ; j < NUM_ITER ; ++j) {
		yield();
	}

}

void proc_io() {
	volatile int i;
	for (i =0 ; i < NUM_LOOPS ; ++i) {
		sleep(1);
	}

}


void run_fun_proc(int pid) {
	switch (pid % 3) {
	case 0: proc_cpu(); break;
	case 1: proc_scpu(); break;
	case 2: proc_io(); break;
	}
	exit();
}

const char* proc_type(int pid) {
	switch (pid % 3) {
	case 0: return "CPU";
	case 1: return "SCPU";
	case 2: return "IO";
	}
	return "ERROR"; // here only to prevent warning
}

void test(const int n) {

	uint sleep_time_arr[3] = {0};
	uint ready_time_arr[3] = {0};
	uint running_time_arr[3] = {0};
	uint num_proc_counter_arr[3] = {0};
	int i = 0;
	for (;i<3*n ; ++i) {
		int fret = fork();
		if (fret == -1 ) {
			printf(2,"error in fork\n");
			exit();
		}
		if (fret == 0) {
			run_fun_proc(getpid());
		}
	}
	for (i = 0 ; i<3*n ; ++i) {
		uint s, re, ru;
		const int pid = wait2(&re, &ru, &s);
		printf(1, "proc id:%d type:%s\nwait:%d, run:%d, IO:%d\n",
				pid,proc_type(pid), re, ru, s);
		const int idx = pid %3;
		sleep_time_arr[idx] += s;
		ready_time_arr[idx] += re;
		running_time_arr[idx] += ru;
		num_proc_counter_arr[idx] += 1;
	}
	for (i=0 ; i<3 ; ++i) {
		const double num_p = num_proc_counter_arr[i];
		const double avg_sleep = sleep_time_arr[i] / num_p;
		const double avg_wait = ready_time_arr[i] / num_p;
		const double total_avg = (sleep_time_arr[i] + ready_time_arr[i] + running_time_arr[i]) / num_p ;
		printf(1, "group type:%s\nsleep average:%d\n ready average:%d\n, Turnaround average:%d\n\n",
				proc_type(i),(int)avg_sleep ,(int)avg_wait,(int) total_avg );
	}
}

int
main(int argc, char *argv[])
{

	if (argc != 2) {
		printf(2 ,"need exactly one arg - num processes>=1\n");
		exit();
	}
	const int num = atoi(argv[1]);
	if (! (num >=1 ) ) {
		printf(2,"got %d . need to be >=1\n",num);
		exit();
	}
	test(num);
	exit();
}
