/*
 * File:   BTNS.c
 * Author: duchovne
 *
 * Created on May 13, 2022, 10:58 AM
 */


#include "xc.h"
#include "BTNS.h"

/*
 * initialize the S1 and S2 buttons
 */
void BTNS_INIT()
{
    S1_TRIS = 1;
    S2_TRIS = 1;
}

/*
 * returns true if the button is pressed
 */
bool IS_PRESSED(BUTTON btn)
{
    switch(btn)
    {
        case S1:
            return S1_PORT == PRESSED ? true : false;
        case S2:
            return S2_PORT == PRESSED ? true : false;
        default:
            return false;
    }
    return false;
}