#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "fcntl.h"

volatile int vi;
volatile int* pi = &vi;

#define NUM_LOOPS 100
#define NUM_ITER 1000000
#define SYNC_F "/syncf"
void proc_cpu() {
	volatile int i,j ;
	for (i =0 ; i< NUM_LOOPS ; ++i);
	for (j =0 ; j<NUM_ITER ; ++j) {
		*pi = j;
//		if(j%1000 == 0) {
//			printf(1,"%d",j);
//		}
	}
	exit();
}
#define NUM_P 20

int get_prio_from_pid(int pid) {
	return pid % NUM_P;
}

void test() {

	int i = 0 ;
#ifdef N_SYNC
	int fd;
	fd = open(SYNC_F,O_CREATE);
	close(fd);
#endif
	for (;i<NUM_P ; ++i) {
		int fret = fork();
		if (fret == -1 ) {
			printf(2,"error in fork\n");
			exit();
		}
		if (fret == 0) {
//			printf(2,"child.pid %d\n",getpid());

#ifdef SCHEDFLAG
#if SCHEDFLAG==SML
			extern int set_prio(int);
			const int prio = get_prio_from_pid(getpid());
			set_prio(prio);
						printf(2,"child.pid %d prio:%d\n",getpid(),prio);



#endif
#endif

#ifdef N_SYNC
		while (-1 != open(SYNC_F,0)) {
//								printf(2,"child.pid %d prio:%d wait\n",getpid());

							}
#endif //N_SYNC
		proc_cpu();
		}
		if ( fret >0 ) {
			//			printf(2,"forked %d proc\n",fret);
		}
	}
#ifdef N_SYNC
	unlink(SYNC_F);
#endif
	for (i = 0 ; i<NUM_P ; ++i) {
		uint s, re, ru;
		const int pid = wait2(&re, &ru, &s);
		printf(1, "proc id:%d finished.priority:%d. totaltime:%d\n",
				pid,get_prio_from_pid(pid),(int)(s+re+ru));

	}
}

int
main(int argc, char *argv[])
{
	test();
	exit();
}
