/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
/*New APIs Prototype*/
#if ( configUSE_EDF_SCHEDULER == 1 )
	BaseType_t xTaskPeriodicCreate( TaskFunction_t pxTaskCode,
													const char * const pcName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
													const configSTACK_DEPTH_TYPE usStackDepth,
													void * const pvParameters,
													UBaseType_t uxPriority,
													TaskHandle_t * const pxCreatedTask,
													TickType_t period	);
#endif
/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
/* parameters for RTOS time anaylisis*/
char dataBuffer;
int task_1_in_time = 0, task_1_out_time = 0, task_1_total_time=0, cpu_load_1;
int task_2_in_time = 0, task_2_out_time = 0, task_2_total_time=0, cpu_load_2;
int task_3_in_time = 0, task_3_out_time = 0, task_3_total_time=0, cpu_load_3;
int task_4_in_time = 0, task_4_out_time = 0, task_4_total_time=0, cpu_load_4;
int task_5_in_time = 0, task_5_out_time = 0, task_5_total_time=0, cpu_load_5;
int task_6_in_time = 0, task_6_out_time = 0, task_6_total_time=0, cpu_load_6;
int system_time =0;
int cpu_load = 0;


void vApplicationTickHook(void){
		GPIO_write(PORT_0, PIN0, PIN_IS_HIGH);
		GPIO_write(PORT_0, PIN0, PIN_IS_LOW);
}
void vApplicationIdleHook(void){
}
/*
void vApplicationGetIdleTaskMemory(){

}
*/

/*This task will monitor rising and falling edge on button 1 and send this event to the consumer task. 
*(Note: The rising and failling edges are treated as separate events, hence they have separate strings */
void vTask_1(void *pvParameters ){
	TickType_t pxPreviousWakeTime;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	for(;;){
		pxPreviousWakeTime = xTaskGetTickCount();
		if(GPIO_read(PORT_0,PIN8))
			while(GPIO_read(PORT_0,PIN8));
		vTaskDelayUntil(&pxPreviousWakeTime, 50);
	}
}
/*This task will monitor rising and falling edge on button 2 and send this event to the consumer task. 
*(Note: The rising and failling edges are treated as separate events, hence they have separate strings */					
void vTask_2(void *pvParameters ){
	TickType_t pxPreviousWakeTime;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	for(;;){
		pxPreviousWakeTime = xTaskGetTickCount();
				if(GPIO_read(PORT_0,PIN9))
				while(GPIO_read(PORT_0,PIN9));// for debouncing       
		vTaskDelayUntil(&pxPreviousWakeTime, 50);
	}
}

/* This task will send preiodic string every 100ms to the consumer task*/
void vTask_3(void *pvParameters ){
	TickType_t pxPreviousWakeTime=0;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	for(;;){
		task_3_in_time = portGET_RUN_TIME_COUNTER_VALUE();
		pxPreviousWakeTime = xTaskGetTickCount();
		dataBuffer = 'A';
		vTaskDelayUntil(&pxPreviousWakeTime, 100);
	}
}
	
/* This is the consumer task which will write on UART any received string from other tasks*/			
void vTask_4(void *pvParameters ){
	TickType_t pxPreviousWakeTime;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );
	for(;;){
		pxPreviousWakeTime = xTaskGetTickCount();
		xSerialPutChar(dataBuffer);
		vTaskDelayUntil(&pxPreviousWakeTime, 20);
	}
}
/* Load Simulation Tasks */
void vTask_5(void *pvParameters ){
	TickType_t pxPreviousWakeTime;
	uint32_t loadSimulation;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
	for(;;){
		loadSimulation=50000;
		pxPreviousWakeTime = xTaskGetTickCount();
		while(loadSimulation-->0);
		vTaskDelayUntil(&pxPreviousWakeTime, 10);
	}
}

void vTask_6(void *pvParameters ){
	TickType_t pxPreviousWakeTime=0;
	uint32_t loadSimulation;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 6 );
	for(;;){
		loadSimulation=120000;
		pxPreviousWakeTime = xTaskGetTickCount();
		while(loadSimulation-- > 0);
		vTaskDelayUntil(&pxPreviousWakeTime, 100);
	}
}


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	TaskHandle_t xHandleT1, xHandleT2, xHandleT3, xHandleT4, xHandleT5, xHandleT6;

	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	xSerialPortInitMinimal(9600);
	
    /* Create Tasks here */
		/*This task will monitor rising and falling edge on button 1 and send this event to the consumer task. 
		 *(Note: The rising and failling edges are treated as separate events, hence they have separate strings */
		xTaskPeriodicCreate( 
										vTask_1, 									/* Pointer to the function that implements the task. */
										"B1",    							/* Text name given to the task. */
										configMINIMAL_STACK_SIZE,  /* The size of the stack that should be created for the task.This is defined in words, not bytes*/
										NULL,                      /* A reference to xParameters is used as the task parameter. This is cast to a void * to prevent compiler warnings. */
										1,                        /* The priority to assign to the newly created task. */
										&xHandleT1,											/* The handle to the task being created will be placed in xHandle. */
										50												/* Task's Period used for EDF Scheduling*/
								);

		/*This task will monitor rising and falling edge on button 2 and send this event to the consumer task. 
		 *(Note: The rising and failling edges are treated as separate events, hence they have separate strings */								
		xTaskPeriodicCreate( 
										vTask_2,
										"B2",
										configMINIMAL_STACK_SIZE,
										NULL,
										1,
										&xHandleT2,            
										50
							);
		/* This task will send preiodic string every 100ms to the consumer task*/

						xTaskPeriodicCreate( 
										vTask_3, 									/* Pointer to the function that implements the task. */
										"Sender",    /* Text name given to the task. */
										configMINIMAL_STACK_SIZE,  /* The size of the stack that should be created for the task.This is defined in words, not bytes*/
										NULL,                      /* A reference to xParameters is used as the task parameter. This is cast to a void * to prevent compiler warnings. */
										1,                        /* The priority to assign to the newly created task. */
										&xHandleT3,            					/* The handle to the task being created will be placed in xHandle. */
										100												/* Task's Period used for EDF Scheduling*/
								);

		/* This is the consumer task which will write on UART any received string from other tasks*/							
		xTaskPeriodicCreate( 
										vTask_4,
										"Consumer",
										configMINIMAL_STACK_SIZE,
										NULL,
										1,
										&xHandleT4,            
										20
							);
		/* Execution time: 5ms*/
		xTaskPeriodicCreate( 
										vTask_5, 									/* Pointer to the function that implements the task. */
										"L1",    /* Text name given to the task. */
										configMINIMAL_STACK_SIZE,  /* The size of the stack that should be created for the task.This is defined in words, not bytes*/
										NULL,                      /* A reference to xParameters is used as the task parameter. This is cast to a void * to prevent compiler warnings. */
										1,                        /* The priority to assign to the newly created task. */
										&xHandleT5,            					/* The handle to the task being created will be placed in xHandle. */
										10												/* Task's Period used for EDF Scheduling*/
								);
		/* Execution time: 12ms*/
		xTaskPeriodicCreate( 
										vTask_6,
										"L2",
										configMINIMAL_STACK_SIZE,
										NULL,
										1,
										&xHandleT6,            
										100
							);
							
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}




/*-----------------------------------------------------------*/
	

