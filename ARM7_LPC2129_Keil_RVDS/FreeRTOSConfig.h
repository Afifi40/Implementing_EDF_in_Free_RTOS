/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			1
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

#define configQUEUE_REGISTRY_SIZE 	0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_xTaskGetIdleTaskHandle	1		
#define configUSE_EDF_SCHEDULER 1		//put 1 for EDF Scheduler


/**RTOS TIME ANALYISIS**/
extern int task_1_in_time, task_1_out_time, task_1_total_time, cpu_load_1;
extern int task_2_in_time, task_2_out_time, task_2_total_time, cpu_load_2;
extern int task_3_in_time, task_3_out_time, task_3_total_time, cpu_load_3;
extern int task_4_in_time, task_4_out_time, task_4_total_time, cpu_load_4;
extern int task_5_in_time, task_5_out_time, task_5_total_time, cpu_load_5;
extern int task_6_in_time, task_6_out_time, task_6_total_time, cpu_load_6;
extern int system_time;
extern int cpu_load;
#define configUSE_APPLICATION_TASK_TAG 1
								
#define configGENERATE_RUN_TIME_STATS 0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE() (T1TC)
#define configUSE_STATS_FORMATTING_FUNCTIONS 0
#if configGENERATE_RUN_TIME_STATS == 0
	#define traceTASK_SWITCHED_OUT() do\
									{ switch ((int)pxCurrentTCB->pxTaskTag)\
										{\
											case 0: /*IDLE*/\
												GPIO_write(PORT_0, PIN7, PIN_IS_LOW);\
												system_time = portGET_RUN_TIME_COUNTER_VALUE(); \
												cpu_load = ((task_1_total_time + task_2_total_time + task_3_total_time + task_4_total_time + task_5_total_time + task_6_total_time)/ (float)system_time) * 100; \
												break;\
											case 1: /*TASK1*/\
												GPIO_write(PORT_0, PIN1, PIN_IS_LOW);\
												task_1_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_1_total_time += task_1_out_time - task_1_in_time;\
												cpu_load_1 =task_1_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 2:	/*TASK2*/\
												GPIO_write(PORT_0, PIN2, PIN_IS_LOW);\
												task_2_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_2_total_time += task_2_out_time - task_2_in_time;\
												cpu_load_2 =task_2_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 3:	/*TASK3*/\
												GPIO_write(PORT_0, PIN3, PIN_IS_LOW);\
												task_3_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_3_total_time += task_3_out_time - task_3_in_time;\
												cpu_load_3 =task_3_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 4:	/*TASK4*/\
												GPIO_write(PORT_0, PIN4, PIN_IS_LOW);\
												task_4_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_4_total_time += task_4_out_time - task_4_in_time;\
												cpu_load_4 =task_4_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 5:	/*TASK5*/\
												GPIO_write(PORT_0, PIN5, PIN_IS_LOW);\
												task_5_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_5_total_time += task_5_out_time - task_5_in_time;\
												cpu_load_5 =task_5_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 6:	/*TASK6*/\
												GPIO_write(PORT_0, PIN6, PIN_IS_LOW);\
												task_6_out_time = portGET_RUN_TIME_COUNTER_VALUE();\
												task_6_total_time += task_6_out_time - task_6_in_time;\
												cpu_load_6 =task_6_total_time * 100 / portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											default:\
										}\
									}while(0)

	#define traceTASK_SWITCHED_IN() do\
									{ switch ((int)pxCurrentTCB->pxTaskTag)\
										{\
												case 0: /*IDLE*/\
												GPIO_write(PORT_0, PIN7, PIN_IS_HIGH);\
												break;\
											case 1: /*TASK1*/\
												GPIO_write(PORT_0, PIN1, PIN_IS_HIGH);\
												task_1_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 2:	/*TASK2*/\
												GPIO_write(PORT_0, PIN2, PIN_IS_HIGH);\
												task_2_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 3:	/*TASK3*/\
												GPIO_write(PORT_0, PIN3, PIN_IS_HIGH);\
												task_3_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 4:	/*TASK4*/\
												GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);\
												task_4_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 5:	/*TASK5*/\
												GPIO_write(PORT_0, PIN5, PIN_IS_HIGH);\
												task_5_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											case 6:	/*TASK6*/\
												GPIO_write(PORT_0, PIN6, PIN_IS_HIGH);\
												task_6_in_time = portGET_RUN_TIME_COUNTER_VALUE();\
												break;\
											default:\
												\
										}\
									}while(0)
	#else
	#define traceTASK_SWITCHED_OUT() do\
									{ switch ((int)pxCurrentTCB->pxTaskTag)\
										{\
											/*case 0: IDLE\
												GPIO_write(PORT_0, PIN0, PIN_IS_LOW);\
												break;*/\
											case 1: /*TASK1*/\
												GPIO_write(PORT_0, PIN1, PIN_IS_LOW);\
												break;\
											case 2:	/*TASK2*/\
												GPIO_write(PORT_0, PIN2, PIN_IS_LOW);\
												break;\
											case 3:	/*TASK3*/\
												GPIO_write(PORT_0, PIN3, PIN_IS_LOW);\
												break;\
											case 4:	/*TASK4*/\
												GPIO_write(PORT_0, PIN4, PIN_IS_LOW);\
												break;\
											case 5:	/*TASK5*/\
												GPIO_write(PORT_0, PIN5, PIN_IS_LOW);\
												break;\
											case 6:	/*TASK6*/\
												GPIO_write(PORT_0, PIN6, PIN_IS_LOW);\
												break;\
											default:\
										}\
										system_time = portGET_RUN_TIME_COUNTER_VALUE(); \
										cpu_load = ((task_1_total_time + task_2_total_time + task_3_total_time + task_4_total_time + task_5_total_time + task_6_total_time)/ (float)system_time) * 100; \
									}while(0)

	#define traceTASK_SWITCHED_IN() do\
									{ switch ((int)pxCurrentTCB->pxTaskTag)\
										{\
											/*case 0: IDLE\
												GPIO_write(PORT_0, PIN0, PIN_IS_HIGH);\
												break;*/\
											case 1: /*TASK1*/\
												GPIO_write(PORT_0, PIN1, PIN_IS_HIGH);\
												break;\
											case 2:	/*TASK2*/\
												GPIO_write(PORT_0, PIN2, PIN_IS_HIGH);\
												break;\
											case 3:	/*TASK3*/\
												GPIO_write(PORT_0, PIN3, PIN_IS_HIGH);\
												break;\
											case 4:	/*TASK4*/\
												GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);\
												break;\
											case 5:	/*TASK5*/\
												GPIO_write(PORT_0, PIN5, PIN_IS_HIGH);\
												break;\
											case 6:	/*TASK6*/\
												GPIO_write(PORT_0, PIN6, PIN_IS_HIGH);\
												break;\
											default:\
												break;\
										}\
									}while(0)

#endif

#endif /* FREERTOS_CONFIG_H */
