

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef POT_ATD_H
#define	POT_ATD_H


/*
 * initialize the ADC to work with the potentiometer 
 */
void POT_ADC_INIT();

/*
 * Read the pot and returns a value 0 - 1023
 */
int READ_POT();


#endif	/*POT_ATD_H*/
