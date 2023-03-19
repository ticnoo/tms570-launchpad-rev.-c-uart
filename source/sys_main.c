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

uint8_t days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t days_year_before = 6;
//uint8_t days_year_before = 4;


char days[23] = "Su Mo Tu We Th Fr Sa\n\r\0";
char *pdays = &days[0];

uint32_t time = 100000;


uint8_t current_month = 2;
uint16_t year = 2023;
//uint16_t year = 2021;

bool next_month = true;
bool send_next = false;
bool bottom_flag = false;


void delay(uint32_t time)
{
	while(time>0) time--;
}


void callendar(uint8_t month_number)
{
	gioSetBit(gioPORTA, 2, 1);

	uint8_t week_day = 0;
	uint8_t i=0;
	uint16_t days_before_current = 0;
	char year_send[5];

	year_send[0] = year/1000 + '0';
	year_send[1] = (year/100)%10 + '0';
	year_send[2] = (year/10)%100 + '0';
	year_send[3] = year%10 + '0';
	year_send[4] = '\0';

	char *pyear_send = &year_send[0];

	char *pmonth = &month[month_number][0];

	char month_year[20];
	char *pmonth_year = &month_year[0];



	sciSend(scilinREG, 20, (uint8_t*) strcat(strcat(pmonth_year, pmonth),pyear_send));
	delay(time);

	memset(pmonth_year, '\0', 20);

	sciSend(scilinREG, 3, "\n\r\0");
	delay(time);

	sciSend(scilinREG, 23, (uint8_t*) pdays);
	delay(time+time);

	if (year%4 == 0)
	{
		days_in_months[1] = 29;
		//days_before_current--;
	}
	else
	{
		days_in_months[1] = 28;
	}

	for (i = 0; i < month_number; i++)
	{
		days_before_current += days_in_months[i];
	}

	days_before_current += days_year_before;


	uint8_t callendar_days = days_before_current % 7;

	for (i = callendar_days; i != 0; i--)
	{
		char buffer[3] = "   ";
		char * pbuffer = &buffer[0];

		/* buffer[2] = ' ';
		buffer[0] = (days_in_months[month_number]-i)/10 + '0';
		buffer[1] = (days_in_months[month_number]-i)%10 + '0'; */

		sciSend(scilinREG, 3, (uint8_t*) pbuffer);
		delay(time);
		week_day++;
	}


// check days in month

	for (i = 1; i<=days_in_months[month_number]; i++)
	{

		char buffer[3];
		char * pbuffer = &buffer[0];
		buffer[2] = ' ';

		if (i>=10)
		{
			buffer[0] = i/10 + '0';
			buffer[1] = i%10 + '0';
		}
		else
		{
			buffer[0] = ' ';
			buffer[1] = i + '0';
		}

		sciSend(scilinREG, 3, (uint8_t*) pbuffer);
		delay(time);

		week_day++;
		if (week_day > 6)
		{
			sciSend(scilinREG, 3, "\n\r\0");
			delay(time);
			week_day = 0;
		}


	}


	gioSetBit(gioPORTA, 2, 0);

}

/*
uint8_t napr = 0;

void napravlenie(void)
{
	if (year == 2024)
	{
		if (current_month == 11)
		{
			napr = 1;
		}
	}

	if (napr == 1)
	{
		current_month--;
	}
	else
	{
		current_month++;
	}
}

*/

/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */

	systemInit();
	gioInit();
	sciInit();
	rtiInit();
	_enable_IRQ(); // разрешение прерывания
	gioEnableNotification (gioPORTA, 7);

	rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
	//rtiStartCounter(rtiCOUNTER_BLOCK0);
	rtiREG1->CMP[0U].UDCPx = 0;

	while(1)
	{

		if (bottom_flag == true)
		{


		}





		if (send_next == true)
		{
			switch (current_month)
					{
						case 12:
							current_month = 0;
							year++;

							if (year%4 == 1)
							{
								days_year_before++;
							}
							days_year_before++;
							break;
						case 255:
							current_month = 11;
							year--;

							if (year%4 == 0)
							{
								days_year_before--;
							}
							days_year_before--;
							break;

					}

			switch (days_year_before)
			{
				case 7:
					days_year_before = 0;
					break;
				case 8:
					days_year_before = 1;
					break;
				case 255:
					days_year_before = 6;
					break;
				case 254:
					days_year_before = 5;
					break;
			}



			callendar(current_month);
			delay(time);
			sciSend(scilinREG, 3, "\n\r\0");
			delay(time);
			send_next = false;
			delay(time);
			//gioEnableNotification (gioPORTA, 7);
		}



/*
		if (gioGetBit(gioPORTA, 7) == 1)
		{
			callendar(current_month);
			delay(time);

			sciSend(scilinREG, 3, "\n\r\0");
			delay(4000000);
			current_month++;
		}
*/


	}
/* USER CODE END */


}


/* USER CODE BEGIN (4) */

void gioNotification(gioPORT_t *port, uint32  bit)
{

	//gioDisableNotification (gioPORTA, 7);
	if (gioREG->POL == 1<<7)
	{
		rtiStartCounter(rtiCOUNTER_BLOCK0);
		gioREG->POL = 0<<7;
	}
	else
	{
		rtiStopCounter(rtiCOUNTER_BLOCK0);
		rtiResetCounter(rtiCOUNTER_BLOCK0);
		gioREG->POL = 1<<7;
		//if (rtiGetCurrentTick(rtiCOMPARE0) < 1000000)
		//{
			current_month++;
			send_next = true;
		//}
	}
	delay(200000);

	bottom_flag = true;



}


void rtiNotification(uint32 Notification)
{
	gioToggleBit(gioPORTA, 2);
	rtiStopCounter(rtiCOUNTER_BLOCK0);
	rtiResetCounter(rtiCOUNTER_BLOCK0);
	//napravlenie();
	current_month--;
	send_next = true;
	gioREG->POL = 1<<7;
}

/* USER CODE END */
