#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stdint.h>

extern void runFirstThread(void);
extern uint32_t* stackptr;
extern uint32_t* MSP_INIT_VAL;

void SVC_Handler_Main(unsigned int *svc_args);
bool osCreateThread(void* fcnPtr);

// Rest of your code in kernel.h

typedef struct k_thread{
	uint32_t* sp; //stack pointer
	void (*thread_function) (void*);
}thread;

#endif /* KERNEL_H */
