

#include "date_time.h" // include processor files - each processor file is guarded.  

// this array represents the number of days in one non-leap year at 
//    the beginning of each month
unsigned long DaysToMonth[13] = {
   0,31,59,90,120,151,181,212,243,273,304,334,365
};


unsigned long datetime_to_epoc(Datetime *datetime) {

   unsigned long iday;
   unsigned long val;

   iday = 365 * (datetime->tm_year - 70) + DaysToMonth[datetime->tm_mon-1] + (datetime->tm_mday - 1);
   iday = iday + (datetime->tm_year - 69) / 4;
   if ((datetime->tm_mon > 2) && ((datetime->tm_year % 4) == 0)) {
      iday++;
   }
   val = datetime->tm_sec + 60 * datetime->tm_min + 3600 * (datetime->tm_hour + 24 * iday);
   return val;
}


void epoc_to_datetime(unsigned long binary,Datetime *datetime) {

   unsigned long hour;
   unsigned long day;
   unsigned long minute;
   unsigned long second;
   unsigned long month;
   unsigned long year;
     
   unsigned long whole_minutes;
   unsigned long whole_hours;
   unsigned long whole_days;
   unsigned long whole_days_since_1968;
   unsigned long leap_year_periods;
   unsigned long days_since_current_lyear;
   unsigned long whole_years;
   unsigned long days_since_first_of_year;
   unsigned long days_to_month;
   unsigned long day_of_week;
       
   whole_minutes = binary / 60;
   second = binary - (60 * whole_minutes);                 // leftover seconds

   whole_hours  = whole_minutes / 60;
   minute = whole_minutes - (60 * whole_hours);            // leftover minutes

   whole_days   = whole_hours / 24;
   hour         = whole_hours - (24 * whole_days);         // leftover hours
        
   whole_days_since_1968 = whole_days + 365 + 366;
   leap_year_periods = whole_days_since_1968 / ((4 * 365) + 1);

   days_since_current_lyear = whole_days_since_1968 % ((4 * 365) + 1);
        
   // if days are after a current leap year then add a leap year period
   if ((days_since_current_lyear >= (31 + 29))) {
      leap_year_periods++;
   }
   whole_years = (whole_days_since_1968 - leap_year_periods) / 365;
   days_since_first_of_year = whole_days_since_1968 - (whole_years * 365) - leap_year_periods;

   if ((days_since_current_lyear <= 365) && (days_since_current_lyear >= 60)) {
      days_since_first_of_year++;
   }
   year = whole_years + 68;        

   // setup for a search for what month it is based on how many days have past
   //   within the current year
   month = 13;
   days_to_month = 366;
   while (days_since_first_of_year < days_to_month) {
       month--;
       days_to_month = DaysToMonth[month-1];
       if ((month > 2) && ((year % 4) == 0)) {
           days_to_month++;
        }
   }
   day = days_since_first_of_year - days_to_month + 1;

   day_of_week = (whole_days  + 4) % 7;

   datetime->tm_yday = 
       days_since_first_of_year;      /* days since January 1 - [0,365]       */
   datetime->tm_sec  = second;        /* seconds after the minute - [0,59]    */
   datetime->tm_min  = minute;        /* minutes after the hour - [0,59]      */
   datetime->tm_hour = hour;          /* hours since midnight - [0,23]        */
   datetime->tm_mday = day;           /* day of the month - [1,31]            */
   datetime->tm_wday = day_of_week;   /* days since Sunday - [0,6]            */
   datetime->tm_mon  = month;         /* months since January - [0,11]        */
   datetime->tm_year = year;          /* years since 1900                     */
}


