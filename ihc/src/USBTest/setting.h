#ifndef _SETTING_H_
#define _SETTING_H_


#define NTHREADS     2   // number of user defined threads
#define NALARMS		 1
#define STACKSIZE 4096   // stack size of user defined threads

extern int priority[NTHREADS];    // thread priority (0-14) <- 0 is the highest
extern int interval[NTHREADS];    // interval between thread call

extern void (*thread_func[])();

#endif //_SETTING_H_


