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

#define PWM1 3
#define PWM2 4
#define CMP0EN 4
#define CMP1EN 5
#define WGMODE 0
#define CNTEI 0

// Må lage en pinDefines.h som inneholder alle defines for å gjøre alt mere oversiktlig!





int main(void)
{
    PORTD.DIR = (1<<PWM1) || (1<<PWM2);  //Setter PWM1 og PWM2 som utgang
	TCA0.SINGLE.CTRLB = (1<<CMP0EN) || (3<<WGMODE);   //Setter TCA0 til waveform generation mode
	TCA0.SINGLE.EVCTRL = (1<<CNTEI);  //Deactivates count on event input
	
    while (1) 
    {
    }
}

