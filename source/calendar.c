#include "calendar.h"
//#include <string.h>

static const char *months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static uint8_t days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char week_days[20] = "Mo Tu We Th Fr Sa Su";

/** @fn void look_for_7_days(uint8_t week_day)
 * 	@brief Hold week days between 1-7
 *	@param[in] Number of day which need to be checked
 *	@param[out] Change that variable to day in between 1-7
 *
 * 	This function look if the day count is not between 1-7 days in week and change it
 */

static void look_for_7_days (uint8_t week_day)
{
	if (week_day == 7) week_day = 0;
	if (week_day == 8) week_day = 1;
	if (week_day == 255) week_day = 6;
	if (week_day == 254) week_day = 5;
}

/** @fn _Bool look_for_leap_year (uint16_t year_check)
 * 	@brief Calculate leap year
 *
 * 	This function calculates leap year, rules:
 * 	if the year is a multiple of 400, the year is a leap year
 * 	if the year is a multiple of 100, it is not
 * 	if the year is a multiple of 4, it is
 *
 * 	Input year to check, output true/false
 */

_Bool look_for_leap_year (uint16_t year_check)
{
	/** look for leap year and change Fb days if it is */
	if ((year_check % 400 == 0) || (year_check % 4 == 0 && year_check % 100 != 0)) return true;
	else return false;
}

/** @fn uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set)
 * 	@brief Count days before first day of the year
 * 	@param[in] Number of the day of the week on January 1st of the current year
 * 	@param[in] Current year
 * 	@param[in] Set year
 * 	@return Number of days
 *
 * 	This function count days before first day in set year to find what day does the year start
 */

uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set)
{
	/** +1 day every year shift, +2 every leap year */
	uint8_t days_before = days_before_current_year;
	for (uint16_t i = year_set; prev_year != i;)
	{
		if (prev_year > year_set)
		{
			days_before--;
			if (look_for_leap_year(i)) days_before--;
			i++;
		}
		if (prev_year < year_set)
		{
			days_before++;
			if (look_for_leap_year(i)) days_before++;
			i--;
		}
		look_for_7_days(days_before);
	}
	return days_before;
}

/** @fn uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set)
 * 	@brief Count days before first day of the month
 * 	@param[in] Days before 1st Jan of this year
 * 	@param[in] Month
 * 	@return Number of days before first day in set month
 *
 * 	This function count days before first day in set month to find what day does the month start
 */

static uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set)
{
	uint16_t days_before_current = 0;

	for (uint8_t i = 0; i <= month_set; i++)
	{
		days_before_current += days_in_months[i];
	}
	days_before_current += days_before_year;
	days_before_current %= 7;

	return days_before_current;
}

/** @fn void callendar_init (struct callendar* c, uint8_t current_month, uint16_t current_year, uint8_t days_before_current_year)
 * 	@brief Calendar init
 * 	@param[in] Pointer to struct of you calendar
 * 	@param[in] Set Year
 * 	@param[in] Set Month
 * 	@param[out] Fill the struct with data of set month
 *
 * 	Initialize calendar for set structure for the first time, input month and year to get a full struct with the data
 */

void callendar_init (struct callendar* c, uint8_t current_month, uint16_t current_year)
{
	/** Input year and feb days in struct */
	c->year = current_year;
	if(look_for_leap_year(c->year)) days_in_months[1] = 29;
	else days_in_months[1] = 28;
	/** Set days before jan 1st in set year, calculates from 18 Apr 2023 */
	c->days_before_set_year = get_days_before_current_year(6, 2023, current_year);
	/** get days in month and calculatesdays before first day in month */
	c->days_in_month = days_in_months[current_month];
	c->days_before_set_month_1st = get_days_before_current_month_1st(c->days_before_set_year, current_month);
}

// change month

/** @fn void set_month (struct callendar* c, uint8_t month_set, uint16_t year_set)
 * 	@brief change month
 * 	@param[in] Pointer to struct of you calendar
 * 	@param[in] Number of month to set
 * 	@param[in] Number of year to set
 *
 * 	This function change month in youre structure
 */

void set_month (struct callendar* c, uint8_t month_set, uint16_t year_set)
{
	c->days_before_set_year = get_days_before_current_year(c->days_before_set_year, c->year, year_set);
	c->year = year_set;
	c->month = months[month_set];
	c->days_in_month = days_in_months[month_set];
	c->days_before_set_month_1st = get_days_before_current_month_1st(c->days_before_set_year, month_set);
}






















