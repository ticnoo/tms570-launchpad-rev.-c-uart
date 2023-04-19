/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */

#include "system.h"
#include "gio.h"
#include "sci.h"
#include "rti.h"

#include "calendar.h"

#define UART scilinREG
#define MONTH 3
#define YEAR 2023


/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

bool send_next = true; 			// buttom send flag
struct calendar my_calendar;	// init calendar struct
uint8_t current_month = MONTH;	// set current month 
uint16_t current_year = YEAR;	// set current year

uint32_t time = 10000000; 		// time for programm delay

/** @fn void delay(uint32_t time)
 *  @brief program block delay 
*/

void delay(uint32_t time) 
{
	while(time>0) time--;
}

void send_month_via(uint8_t lengh, const void* data)
{
	sciSend(UART, lengh, (uint8_t*) data);
}

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

	/** init */
	systemInit();
	gioInit();
	sciInit();
	rtiInit();
	_enable_IRQ();
	gioEnableNotification (gioPORTA, 7); 				// buttom itr
	rtiEnableNotification(rtiNOTIFICATION_COMPARE0); 	// timer itr
	rtiREG1->CMP[0U].UDCPx = 0; 						// rtiReset is not reset the up register, ITR will increas all the time, if it is not 0
	calendar_init(&my_calendar, MONTH, YEAR);

	//rtiStartCounter(rtiCOUNTER_BLOCK0); 				// test, sending months with no button press

	while(1)
	{
		/** send flag check */
		if (send_next)
		{
			send_next = false;
			if (current_month == 12)	// next year
			{
				current_year++;
				current_month = 0;
			}
			if (current_month == 255)	// prev year
			{
				current_year--;
				current_month = 11;
			}

			gioSetBit(gioPORTA, 2, 1);
			set_month(&my_calendar, current_month, current_year); 	// change the month
			send_set_month(&my_calendar);							// send it
			gioSetBit(gioPORTA, 2, 0);
		}

	}
/* USER CODE END */

}


/* USER CODE BEGIN (4) */

/** @fn void gioNotification(gioPORT_t *port, uint32  bit) 
 * 	@brief Buttom interrupt notification
 *  
 * При нажатии кнопки срабатывает прерывание, которое запускает таймера для отслеживания времени удержания,
 * режим срабатывания прерывания кнопки переключается на режим срабатывания по спаду (то есть, когда отожмем кнопку).
 * Если держать кнопку больше времени срабатывания таймера (1 с.), срабатывает прерывание таймера,
 * которое убавляет месяц, который надо отправить, сбрасывает таймер для последующего включения,
 * и переключает режим прерывания кнопки обратно в нарастающий, теперь, при отжатии кнопки, прерывание не сработает.
 * Если отжать кнопку до срабатывания таймера, срабатывает прерывание по спаду, выключается и сбрасывается таймер,
 * режим прерывания кнопки переключается обратно на нарастающий и идет команда заслать следующий месяц.
 */

void gioNotification(gioPORT_t *port, uint32  bit) 
{
	/** checking is buttom ITR starts from falling or rising edge (release or press) */
	if ((gioREG->POL >> 7) && 1U)			// rising
	{
	/** starts timer to count time after press and change ITR to falling edge (release) */
		rtiStartCounter(rtiCOUNTER_BLOCK0);
		gioREG->POL &= ~(1U<<7);
	}
	else									// falling
	{
	/** timer reset, ITR to rising edge, increment of the month, updating the send flag */
		rtiStopCounter(rtiCOUNTER_BLOCK0); 
		rtiResetCounter(rtiCOUNTER_BLOCK0); 
		gioREG->POL |= 1<<7;
		current_month++; 
		send_next = true; 
	}

}

/** @fn void rtiNotification(uint32 Notification)
 * 	@brief Timer interrupt notification
 * 
 * interrupt after *set time* ms, stop and reset the timer, decrement of the month, updating the send flag
 * 
*/

void rtiNotification(uint32 Notification) 
{
	/** Timer reset */
	rtiStopCounter(rtiCOUNTER_BLOCK0); 
	rtiResetCounter(rtiCOUNTER_BLOCK0); 
	current_month--; 
	send_next = true; 
	gioREG->POL |= 1<<7;
}

/* USER CODE END */
