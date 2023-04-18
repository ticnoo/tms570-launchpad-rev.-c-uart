#include "callendar.h"
#include <stdio.h>
#include <string.h>

#define uint8_t int
#define uint16_t int

const char months[12][9] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char week_days[17] = "Mo Tu We Th Fr Sa";
uint8_t days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
const uint8_t days_before_2023 = 6;
uint8_t current_month = 2; // текущий месяц
uint16_t current_year = 2023; // текущий год
*/


// look for 7 days and change it if its not

void look_for_7_days (uint8_t week_day)
{
	if (week_day == 7) week_day = 0;
	if (week_day == 8) week_day = 1;
	if (week_day == -1) week_day = 6;
	if (week_day == -2) week_day = 5;
}

// find week days before 1st day in month, input year and month

uint8_t get_days_before_current_month_1st (uint8_t days_before_year, uint8_t month_set)
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

// find week days before 1st Jan, input year

uint8_t get_days_before_current_year (uint8_t days_before_current_year, uint16_t prev_year, uint16_t year_set)
{
	// + 1 day every year shift	
	
	uint8_t days_before = days_before_current_year;

	for (uint16_t i = year_set; prev_year != i;)
	{
		if (prev_year > year_set)
		{
			days_before--;
			if (i / 4 == 0) days_before--;
			i++;
		}
		if (prev_year < year_set)
		{
			days_before++;
			if (i / 4 == 0) days_before++;
			i--;
		}
		
		look_for_7_days(days_before);
	}

	return days_before;
}

// check for leap year, input year

void look_for_leap_year (struct callendar* c)
{
	if (c->year % 4 == 0)
	{
		days_in_months[1] = 29;
	}
	else 
	{
		days_in_months[1] = 28;
	}	
}

/* 
callendar init
input current momth, year, number of days before jan 1st, struct
*/

void callendar_init (struct callendar* c, uint8_t current_month, uint16_t current_year, uint8_t days_before_current_year)
{
	c->year = current_year;
	look_for_leap_year(c);
	
	c->days_before_set_year = days_before_current_year;
	
	strcpy(c->month, months[current_month]);
	
	c->days_in_month = days_in_months[current_month];
	
	c->days_before_set_month_1st = get_days_before_current_month_1st(c->days_before_set_year, current_month);
}

// change month

void set_month (struct callendar* c, uint8_t month_set, uint16_t year_set)
{
	
	c->days_before_set_year = get_days_before_current_year(c->days_before_set_year, c->year, year_set);
	c->year = year_set;
	strcpy(c->month, months[month_set]);
	c->days_in_month = days_in_months[month_set];
	c->days_before_set_month_1st = get_days_before_current_month_1st(c->days_before_set_year, month_set);
	
}



















