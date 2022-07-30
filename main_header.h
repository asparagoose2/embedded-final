
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H


#include <xc.h> // include processor files - each processor file is guarded.  

typedef enum WORKING_MODE_ENUM {
    CLOCK_MODE,
    MENU_MODE
}WorkingMode;

void init_clock_display(unsigned long time_in_seconds, CLOCK_DISPLAY_MODE mode);

void set_working_mode(WorkingMode mode);

void set_alarm(unsigned long time_in_seconds);




#endif	/* XC_HEADER_TEMPLATE_H */

