// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DATE_TIME_H
#define	DATE_TIME_H

// this structure is just the local version of Visual C tm structure in time.h
typedef struct tm_struct {
   int tm_sec;     /* seconds after the minute - [0,59]    */
   int tm_min;     /* minutes after the hour - [0,59]      */
   int tm_hour;    /* hours since midnight - [0,23]        */
   int tm_mday;    /* day of the month - [1,31]            */
   int tm_mon;     /* months since January - [0,11]        */
   int tm_year;    /* years since 1900                     */
   int tm_wday;    /* days since Sunday - [0,6]            */
   int tm_yday;    /* days since January 1 - [0,365]       */
   int tm_isdst;   /* daylight savings time flag           */  // NOT USED
} Datetime;





unsigned long datetime_to_epoc(Datetime *datetime);

void epoc_to_datetime(unsigned long binary,Datetime *datetime);


#endif	/* DATE_TIME_H */


