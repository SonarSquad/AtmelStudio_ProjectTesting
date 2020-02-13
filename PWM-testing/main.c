/*
 * PWM-testing.c
 *
 * Created: 2/10/2020 3:06:01 PM
 * Author : Martin Ericsson
 */ 


#define F_CPU 20e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PWM1 PIN3_bp  //Defining PWM outputs on PORTD
#define PWM2 PIN4_bp  //Defining PWM outputs on PORTD
#define CMP0EN 4
#define CMP1EN 5
#define WGMODE 0
#define CNTEI 0

// Må lage en pinDefines.h som inneholder alle defines for å gjøre alt mere oversiktlig!


void PortDir(void);
void ClkSelect(void);
void Timer_Init(void);



int main(void)
{
	ClkSelect();
	PortDir();   
	
	
	
    while (1) 
    {
    }
}

void PortDir(void){
	PORTD_DIR = (PWM1 | PWM2);  //Setting PWM pins as output
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU cloock frequency
	
}

void Timer_Init(void){
	
}
