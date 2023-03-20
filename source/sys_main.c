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



/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

char month[12][11] = {"January ", "February ", "March ", "April ", "May ", "June ", "July ", "August ", "September ", "October ", "November ", "December "};
char days[23] = "Su Mo Tu We Th Fr Sa\n\r\0";
char *pdays = &days[0];

uint8_t days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t days_year_before = 6; // дней с прошлого года, которые надо вычесть (если 1 января - не понедельник), в 2023 - 6
uint8_t current_month = 2; // текущий месяц
uint16_t year = 2023; // текущий год

bool send_next = false; // флаг для считывания команды кнопки

uint32_t time = 100000; // перменная для програмной задержки

void delay(uint32_t time) // задержка
{
	while(time>0) time--;
}


void callendar(uint8_t month_number)
{
	gioSetBit(gioPORTA, 2, 1); // просто индикация работы функции

	uint8_t week_day = 0; // номер дня недели
	uint8_t i=0; // для for
	uint16_t days_before_current = 0; // дней перед первым числом месяца

	char *pmonth = &month[month_number][0]; // берем выбранный месяц


	char year_send[5]; // буфер для преобразования int года в char год
	char *pyear_send = &year_send[0];

	year_send[0] = year/1000 + '0'; // вычленяем по цифре
	year_send[1] = (year/100)%10 + '0';
	year_send[2] = (year/10)%100 + '0';
	year_send[3] = year%10 + '0';
	year_send[4] = '\0';

	char month_year[20]; // буфер для отправки месяц + год
	char *pmonth_year = &month_year[0];

	sciSend(scilinREG, 20, (uint8_t*) strcat(strcat(pmonth_year, pmonth),pyear_send)); // объединение месяц + год и отправка
	delay(time); // задержка что бы уарт отработал, потому что он в прерывающем режиме и если начать новую отправку он стопнет предыдущую

	memset(pmonth_year, '\0', 20); // очистка буфера

	sciSend(scilinREG, 3, "\n\r\0"); // сдвиг каретки терминала
	delay(time);

	sciSend(scilinREG, 23, (uint8_t*) pdays); // дни недели
	delay(time+time);

	if (year%4 == 0) // проверяем високосность
	{
		days_in_months[1] = 29; // февраль 28 или 29 дней
	}
	else
	{
		days_in_months[1] = 28;
	}


	for (i = 0; i < month_number; i++) // высчитывает, сколько будет дней до 1 числа текущего месяца
	{
		days_before_current += days_in_months[i];
	}

	days_before_current += days_year_before; //добавляем недостающие дни с прошлого года

	uint8_t callendar_days = days_before_current % 7; // из этого числа узнаем остаток относительно недели

	for (i = callendar_days; i != 0; i--) // засылаем пустые места вместо даты
	{
		char buf[4] = "   \0";
		char * pbuf = &buf[0];

		sciSend(scilinREG, 3, (uint8_t*) pbuf);
		delay(time);
		week_day++;
	}

// days in month

	for (i = 1; i<=days_in_months[month_number]; i++) // засылаем даты
	{
		char buf[4];
		char * pbuf = &buf[0]; // буфер для каждого числа
		buf[2] = ' '; // 3 символ всегда пробел
		buf[3] = '\0';

		if (i>=10) // если дата - двузначное число, первым будет пробел
		{
			buf[0] = i/10 + '0';
			buf[1] = i%10 + '0';
		}
		else
		{
			buf[0] = ' ';
			buf[1] = i + '0';
		}

		sciSend(scilinREG, 3, (uint8_t*) pbuf); // засылаем дату
		delay(time);

		week_day++; // считаем дни недели

		if (week_day > 6) // неделя закончилась - двигаем каретку, обнуляем число
		{
			sciSend(scilinREG, 3, "\n\r\0");
			delay(time);
			week_day = 0;

		}
	}

	gioSetBit(gioPORTA, 2, 0);
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

	while(1)
	{

		if (send_next == true) // кнопка была нажата
		{
			switch (current_month) // переход на следующий/предыдущий год
					{
						case 12: // 12 - следующиее после 11
							current_month = 0;
							year++;

							if (year%4 == 1)	// если идем в будущее - в самой високосный год лишний день в
												// феврале, значит добавить к счетчику на следующий год

							{
								days_year_before++; // если високосный - сдвигается на 2 дня, если нет, 1 день
							}
							days_year_before++;
							break;
						case 255: // предыдущее 0
							current_month = 11;
							year--;

							if (year%4 == 0)	// если в прошлое убавляем лишний день именно в сам високосный год
							{
								days_year_before--;
							}
							days_year_before--;
							break;

					}

			switch (days_year_before) // в неделе 7 дней, обнуление если больше 7ми и наоборот, если меньше
			{
				case 7:
					days_year_before = 0;
					break;
				case 8: 	// 8 т.к. в високосный год к 6ти может пребавить 2, следовательно будет сдвиг на 1, а не 0
					days_year_before = 1;
					break;
				case 255:
					days_year_before = 6;
					break;
				case 254:	// то же самое с високосным годом
					days_year_before = 5;
					break;
			}

			callendar(current_month);
			delay(time);
			sciSend(scilinREG, 3, "\n\r\0"); // сдвиг каретки после отправки месяца
			delay(time);
			send_next = false; // сброс флага
			delay(time);
			//gioEnableNotification (gioPORTA, 7);
		}
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
