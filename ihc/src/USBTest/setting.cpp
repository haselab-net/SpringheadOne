#include "setting.h"

int priority[NTHREADS] = { 0, 1};    // thread priority (0-14) <- 0 is the highest
int interval[NTHREADS] = { 1, 10};    // interval between thread call (0.1ms)

extern void SpidarThread();
extern void UsbThread();

void (*thread_func[])() = {
	SpidarThread,
	UsbThread,
};
