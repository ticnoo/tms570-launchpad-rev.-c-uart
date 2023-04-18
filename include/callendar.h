#ifndef callendar_h
#define callendar_h

#define uint8_t int
#define uint16_t int

#include <stdio.h>
//#include "callendar.c"

// extern const uint8_t days_before_2023;
extern const char months[12][9];
extern const char days[23];
extern uint8_t days_in_months[12];
extern uint8_t current_month; // текущий месяц
extern uint16_t year; // текущий год

struct callendar
{
	uint16_t year; // year
	uint8_t days_before_set_year; // days before Jan 1st in week in this year
	char month[9]; // month
	uint8_t days_in_month; // days in month
	uint8_t days_before_set_month_1st; // days before 1st day in month on week
};

void callendar_init (struct callendar* c, uint8_t current_month, uint16_t current_year, uint8_t days_before_current_year);
void set_month (struct callendar* c, uint8_t month_set, uint16_t year_set);
void look_for_leap_year (struct callendar* c);
uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set);
uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set);
void look_for_7_days (uint8_t week_day);




#endif
