/** @file calendar.h
 * 	@brief Calendar Definition File
 * 	@date 19-Apr-2023
 * 	@version 0.9
 */

#ifndef callendar_h
#define callendar_h

#include "sys_common.h"

extern const char week_days[20];

//extern uint8_t days_in_months[12];
//extern uint8_t current_month; // текущий месяц
//extern uint16_t year; // текущий год

struct callendar
{
	uint16_t year; // year
	uint8_t days_before_set_year; // days before Jan 1st in week in this year
	const char* month; // month
	uint8_t days_in_month; // days in month
	uint8_t days_before_set_month_1st; // days before 1st day in month on week
};

void callendar_init (struct callendar* c, uint8_t current_month, uint16_t current_year);
void set_month (struct callendar* c, uint8_t month_set, uint16_t year_set);
//_Bool look_for_leap_year (uint16_t year_check);
//uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set);
//uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set);
//void look_for_7_days (uint8_t week_day);




#endif
