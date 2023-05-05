#ifndef INCLUDE_SCHEDULER_H_
#define INCLUDE_SCHEDULER_H_

/********************************************************************************************************/
/************************************************INCULDES************************************************/
/********************************************************************************************************/

#include <pthread.h>
#include <ucontext.h>


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define NUM_FUNCTIONS 3
#define STACK_SIZE 128


/********************************************************************************************************/
/*****************************************FunctionPrototype**********************************************/
/********************************************************************************************************/

void Scheduler_Start();

#endif