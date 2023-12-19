#include <stdio.h>  // Include the necessary header file for printf
#include "kernel.h"
#include <stdarg.h>
#include "main.h"
#include <stdbool.h>

#define stackSize 1024
int threadCount;

uint32_t* stackptr;
uint32_t* MSP_INIT_VAL;

extern void print_continuously(void);

bool osKernelInitialize(void)
{
	threadCount = 0;
	MSP_INIT_VAL = (uint32_t*)0;
	return true;
}

void SVC_Handler_Main(unsigned int *svc_args)
{
    unsigned int svc_number;
    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, r14, the return addy and xPSR
     * First argument (r0) is svc_args[0]
     */
    svc_number = ((char *)svc_args[6])[-2];
    switch (svc_number)
    {
        case 17: //17 is arbitrarily chosen
            printf("Success!\r\n");
            break;
        case 1:
            printf("One!\r\n");
            break;
        case 3:
            __set_PSP(stackptr);
            runFirstThread();
            break;
        default: /*unknown SVC*/
            break;
    }
}


uint32_t stackAllocater(void)
{
   if (threadCount <= 31)
	{
		threadCount++;
		uint32_t stackLocation;
		stackLocation = MSP_INIT_VAL - stackSize;
		if(stackLocation == MSP_INIT_VAL - stackSize)
		{
			return stackLocation;
		}
	}

	return 0x0; // Add this return statement
}


/**set up thread stack with the name of thread as input,
 * set MSP_INIT_VAL to the value of stackptr from previous location of stack
 * set stackptr to the value of the start of the new stack, i.e 0x200 - 0x200
 */

void setupThreadStack(void* threadName)
{
	MSP_INIT_VAL = (uint32_t*)stackptr;

	*(--stackptr) = 1 << 24; /**this is xPSR*/
	*(--stackptr) = (uint32_t)threadName;
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/
	*(--stackptr) = 0xA; /** arbitrary number*/

}

/**
 * check if the stack for the thread was allocated
 * if not, return false,
 * if allocated properly, set up the stack for the desired thread
 */

bool osCreateThread(void* fcnPtr)
{
    bool threadAllocated = false;

    threadAllocated = stackAllocater();

    if (threadAllocated != NULL)
    {
        setupThreadStack(fcnPtr);
        threadAllocated = true;
    }


    return threadAllocated;
}

bool osKernelStart()
{
	stackptr = stackAllocater();
	osCreateThread(print_continuously);
	return true;
}
