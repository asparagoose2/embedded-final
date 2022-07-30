

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ACCEL_H
#define	ACCEL_H
#include <xc.h>


#define SCL         PORTBbits.RB6
#define SDA         PORTBbits.RB5
#define SDACTRL     TRISBbits.TRISB5

#define TAKE    0       //  SDACTRL = TAKE
#define RELEASE 1       //  SDACTRL = RELEASE

#define ACCEL_ADDR 0x1D


typedef enum {OK, NACK, ACK, BAD_ADDR, BAD_REG} I2Cerror;

/*
 * initialize the accelerometer
 */
void ACCEL_INIT();

/*
 * Read the accelerometer  and returns true if device is shaken
 */


#endif	/*ACCEL_H*/
