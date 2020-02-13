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


void PWM_Init(void);
void ClkSelect(void);
void Timer0_Init(void);



int main(void)
{
	ClkSelect();
	PWM_Init();   
	Timer0_Init();
	
	PORTF_DIR = PIN5_bm;
	
    while (1) 
    {
		
		PORTF.OUTTGL = PIN5_bm;
		_delay_ms(200);
		
    }
}

void PWM_Init(void){
	PORTD_DIR = (PWM1 | PWM2);  //Setting PWM pins as output
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU cloock frequency
	
}

void Timer0_Init(void){
	PORTMUX.TCAROUTEA = 0x3; //Enables PORTMUXD to achieve Waveform Output on the PWM Pins
	TCA0.SINGLE.CTRLA = (1<<0); //Enables Timer 0
	TCA0.SINGLE.CTRLB = (1<<4) | (1<<5) | (1<<1); //Sets PWM1 and PWM0 as the waveform output pin and enables frequency generation.
	
}
