/** @file calendar.h
 * 	@brief Calendar Definition File
 * 	@date 19-Apr-2023
 */

#ifndef callendar_h
#define callendar_h

#include "sys_common.h"
#include "sci.h"

extern const char week_days[20];

//extern uint8_t days_in_months[12];
//extern uint8_t current_month; // текущий месяц
//extern uint16_t year; // текущий год

/** @struct calendar
 *  @brief Struct for calendar
 * 
 * 	This struct hold all the month date needed to sent via UART  
*/

struct calendar
{
	uint16_t year; // year
	uint8_t days_before_set_year; // days before Jan 1st in week in this year
	const char* month; // month
	uint8_t days_in_month; // days in month
	uint8_t days_before_set_month_1st; // days before 1st day in month on week
};

void calendar_init (struct calendar* c, uint8_t current_month, uint16_t current_year);
void set_month (struct calendar* c, uint8_t month_set, uint16_t year_set);
void send_set_month(sciBASE_t *sci, struct calendar* c);

//uint8_t look_for_7_days (uint8_t week_day);
//_Bool look_for_leap_year (uint16_t year_check);
//uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set);
//uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set);
//void look_for_7_days (uint8_t week_day);

#endif
