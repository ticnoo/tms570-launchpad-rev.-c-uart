/** @file calendar.h
 * 	@brief Calendar Definition File
 * 	@date 19-Apr-2023
 * 	@version 1.0
 */

#ifndef callendar_h
#define callendar_h

#include "sys_common.h"
//#include "stdio.h"
#include "string.h"

/** @struct calendar
 *  @brief Struct for calendar
 * 
 * 	This structure hold all the month date needed to sent via UART
*/

struct calendar
{
	uint16_t year; // year
	uint8_t days_before_set_year; // days before Jan 1st in week in this year
	const char* month; // month
	uint8_t days_in_month; // days in month
	int8_t days_before_set_month_1st; // days before 1st day in month on week
};

extern const char week_days[21];

/** @fn extern void send_month_via(uint8_t lengh, const void* data)
 *  @brief How to send data in terminal
 *  @param[in] lengh of data
 *  @param[in] data pointer
 *
 *  Defining a function that will send data to the terminal.
 *  tms570 UART - sciSend(sciBASE_t* sci, uint32 length, uint8* data)
 *  stm32 UART - HAL_UART_Transmit (UART_HandleTypeDef * huart, uint8_t * pData, uint16_t Size, uint32_t Timeout)
 *  etc.
 *  Define in your main.c
 */

extern void send_month_via(uint8_t lengh, const void* data);

void calendar_init (struct calendar* c, uint8_t current_month, uint16_t current_year);
void set_month (struct calendar* c, uint8_t month_set, uint16_t year_set);
void send_set_month(struct calendar* c);

#endif
