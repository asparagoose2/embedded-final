/*
 * File:   DELAY.c
 * Author: duchovne
 *
 * Created on May 13, 2022, 11:13 AM
 */


#include "xc.h"
#include "delay.h"

DELAY_MS(unsigned int delay)
{
    unsigned int  i;
    unsigned char j;

    for (i =0; i< delay; i++)
    {
       for (j =0 ; j < 200; j++)
       {
           asm("NOP");
           asm("NOP");
       }
    }
}
