#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <iostream>
using namespace std;

// These numbers depend entirely on your application
#define NUMBER_OF_WORKERS    6
#define PRODUCER_PRIORITY   0
#define WORKER_PRIORITY     1
#define PRODUCER_STACKSIZE  CYGNUM_HAL_STACK_SIZE_TYPICAL
#define WORKER_STACKSIZE    (CYGNUM_HAL_STACK_SIZE_MINIMUM + 1024)

static unsigned char producer_stack[PRODUCER_STACKSIZE];
static unsigned char worker_stacks[NUMBER_OF_WORKERS][WORKER_STACKSIZE];
static cyg_handle_t producer_handle, worker_handles[NUMBER_OF_WORKERS];
static cyg_thread producer_thread, worker_threads[NUMBER_OF_WORKERS];


static int last;
static int counter;
static int counter2;
static void producer(cyg_addrword_t data){
	while(1){
		cout << "P: last=" << last << " counter=" << counter << " counter2=" << counter2 << std::endl;
		cout.flush();
		cyg_thread_delay(10000);
	}
}

static void worker(cyg_addrword_t data){
	while(1){
		if (last != data){
			counter ++;
			if (data == NUMBER_OF_WORKERS-1){
				counter2++;
			}
		}
		last = data;
		cyg_thread_delay(1);
	}
}

int main(){
	printf("ThreadTest start.\n");
	cout << "cyg_thread_create" << endl;
	cout.flush();
    int i;
	cyg_thread_create(PRODUCER_PRIORITY, &producer, 0, "producer",
                      producer_stack, PRODUCER_STACKSIZE,
                      &producer_handle, &producer_thread);
	cout << "cyg_thread_resume" << endl;
	cout.flush();
    cyg_thread_resume(producer_handle);
    for (i = 0; i < NUMBER_OF_WORKERS; i++) {
        cyg_thread_create(WORKER_PRIORITY+i, &worker, i, "worker",
                          worker_stacks[i], WORKER_STACKSIZE,
                          &(worker_handles[i]), &(worker_threads[i]));
        cyg_thread_resume(worker_handles[i]);
    }
	return 0;
}

