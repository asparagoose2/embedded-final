/*
 * File:   POT_ATD.c
 * Author: duchovne
 *
 * Created on May 13, 2022, 10:47 AM
 */


#include "xc.h"
#include "pot.h"


/*
 * initialize the ADC to work with the potentiometer 
 */
void POT_ADC_INIT() 
{
    AD1CON2bits.PVCFG = 0x00 ;
    AD1CON2bits.NVCFG0 = 0x0 ;
    AD1CON3bits.ADCS = 0xFF ;
    AD1CON1bits.SSRC = 0x0 ;
    AD1CON3bits.SAMC = 0b10000 ;
    AD1CON1bits.FORM = 0b00 ;
    AD1CON2bits.SMPI = 0x0 ;
    AD1CON1bits.MODE12 = 0 ;
    AD1CON1bits.ADON = 1 ;
    ANSCbits.ANSC0 = 1 ;
}

/*
 * Read the pot and returns a value 0 - 1023
 */
int READ_POT() 
{
    int i = 0;
    AD1CHS = 8 ;

    // Get an ADC sample
    AD1CON1bits.SAMP = 1 ;           //Start sampling
    for (i = 0 ; i < 1000 ; i++) ; //Sample delay, conversion start automatically

    AD1CON1bits.SAMP = 0 ;           //Start sampling
    for (i = 0 ; i < 1000 ; i++) ; //Sample delay, conversion start automatically

    while (!AD1CON1bits.DONE) ;       //Wait for conversion to complete

    return ADC1BUF0 ;
}