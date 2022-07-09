
/* 
 * File:   time_keeper.h
 * Author: Ofir Duchovne
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIME_KEEPER_H
#define	TIME_KEEPER_H
#include "point.h"
#define SECONDS_IN_DAY 60*60*24

typedef enum CLOCK_DISPLAY_MODE_ENUM {
    ANALOG_CLOCK_DISPLAY,
    DIGITAL_CLOCK_DISPLAY
} CLOCK_DISPLAY_MODE ;

typedef enum CLOCK_FORMAT_ENUM {
    HRS_12,
    HRS_24
} CLOCK_FORMAT ;

typedef enum AM_PM_ENUM {
    AM,
    PM
} AM_PM ;



#define CALC_POINT(a,l,c) ({Point p; p.x = (uint8_t)(c.x + (sin(a) * l));p.y = (uint8_t)(c.y + (-cos(a) * l)); p;})


#include <xc.h> // include processor files - each processor file is guarded.  
/*
 * Increment time by one second
 */
void second_passed();

/*
 * Set time
 */
void set_time(unsigned long time_in_seconds);

void draw_clock(unsigned long time_in_seconds, CLOCK_DISPLAY_MODE mode);
void draw_corner_clock(unsigned long time_in_seconds);
void draw_full_analog_clock(unsigned long time_in_seconds);
void set_clock_display(CLOCK_DISPLAY_MODE mode);
void draw_watch_face();
void set_clock_format(CLOCK_FORMAT format);

void set_font_color(uint16_t color);
void set_background_color(uint16_t color);

void set_am_pm(AM_PM am_pm);
void toggle_am_pm();

void set_alarm(unsigned long time_in_seconds);
void disable_alarm();
void toggle_alarm_on_off();
void set_off_alarm();
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star



#endif	/* TIME_KEEPER_H */

