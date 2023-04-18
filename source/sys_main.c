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
#include "string.h"
#include "rti.h"
#include "calendar.h"



/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

/*
char month[12][11] = {"January ", "February ", "March ", "April ", "May ", "June ", "July ", "August ", "September ", "October ", "November ", "December "};
char days[23] = "Mo Tu We Th Fr Sa Su\n\r\0";
char *pdays = &days[0];

uint8_t days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t days_year_before = 6; // дней с прошлого года, которые надо вычесть (если 1 января - не понедельник), в 2023 - 6
uint8_t current_month = 2; // текущий месяц
uint16_t year = 2023; // текущий год
*/

bool send_next = false; // флаг для считывания команды кнопки
struct callendar my_callendar;

uint8_t current_month;

uint32_t time = 100000; // перменная для програмной задержки

void delay(uint32_t time) // задержка
{
	while(time>0) time--;
}

void send_set_month(sciBASE_t *sci, struct callendar* c)
{
	sciSend(sci, 1, '\t');
	sciSend(sci, sizeof(c->month), c->month);
	sciSend(sci, 1, ' ');
	sciSend(sci, 2, "\n\r");

	char buff[4];

	buff[0] = c->year / 1000 + '0';
	buff[1] = (c->year / 100) % 10 + '0';
	buff[2] = (c->year / 10) % 10 + '0';
	buff[3] = c->year % 10 + '0';

	sciSend(sci, 4, buff);

	sciSend(sci, 20, week_days);

	for (uint8_t i; )
}




/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */

	systemInit();
	gioInit();
	sciInit();
	rtiInit();
	_enable_IRQ(); // разрешение прерывания
	gioEnableNotification (gioPORTA, 7); // включение прерывания кнопки

	rtiEnableNotification(rtiNOTIFICATION_COMPARE0); // вклчюение прерывания таймера
	//rtiStartCounter(rtiCOUNTER_BLOCK0); // тест, что б засылал без кнопок
	rtiREG1->CMP[0U].UDCPx = 0; // что бы компаратор не добавлял себе значение каждый раз при срабатывании, т.к. rtiReset не сбрасывает этот параметр




	callendar_init(&my_callendar, 3, 2023);




	while(1)
	{



	}
/* USER CODE END */


}


/* USER CODE BEGIN (4) */

/*
 * При нажатии кнопки срабатывает прерывание, которое запускает таймера для отслеживания времени удержания,
 * режим срабатывания прерывания кнопки переключается на режим срабатывания по спаду (то есть, когда отожмем кнопку).
 * Если держать кнопку больше времени срабатывания таймера (1 с.), срабатывает прерывание таймера,
 * которое убавляет месяц, который надо отправить, сбрасывает таймер для последующего включения,
 * и переключает режим прерывания кнопки обратно в нарастающий, теперь, при отжатии кнопки, прерывание не сработает.
 * Если отжать кнопку до срабатывания таймера, срабатывает прерывание по спаду, выключается и сбрасывается таймер,
 * режим прерывания кнопки переключается обратно на нарастающий и идет команда заслать следующий месяц.
 */

void gioNotification(gioPORT_t *port, uint32  bit) // прерывание сработало
{

	//gioDisableNotification (gioPORTA, 7);
	if (gioREG->POL == 1<<7) // если сработало на наростании
	{
		rtiStartCounter(rtiCOUNTER_BLOCK0); // включаем таймер для остлеживания, сколько времени нажата кнопка
		gioREG->POL = 0<<7; // прерывание на спад фронта
	}
	else // если сработало на спаде
	{
		rtiStopCounter(rtiCOUNTER_BLOCK0); // стоп таймера
		rtiResetCounter(rtiCOUNTER_BLOCK0); // сброс текущего тика
		gioREG->POL = 1<<7; // прерывание по наростанию
		current_month++; // + месяц
		send_next = true; // флаг отправки
	}

}


void rtiNotification(uint32 Notification) // сработало прерывание компаратора
{
	rtiStopCounter(rtiCOUNTER_BLOCK0); // остановка таймера
	rtiResetCounter(rtiCOUNTER_BLOCK0); // сброс
	current_month--; // - месяц
	send_next = true; // флаг отправки
	gioREG->POL = 1<<7; // переключение на срабатывание прерывания кнопки по наростанию
}

/* USER CODE END */
