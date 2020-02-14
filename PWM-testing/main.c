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
	
	PORTD_DIR = 0XFF; //Set PORTD as output.
	
	
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU clock frequency
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (1<<7)); //Sender klokke-signal ut på PA7-pin
	PORTA_DIR |= (1<<7); //PA7 as output for clock frequency check
	
}

void Timer0_Init(void){
	PORTMUX.TCAROUTEA |= (PORTMUX_TCA00_bm | PORTMUX_TCA01_bm); //Enables PORTMUXD to achieve Waveform Output on the PWM Pins
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc    /* System Clock */
	                    | 1 << TCA_SINGLE_ENABLE_bp; /* Module Enable: enabled */
	TCA0.SINGLE.CTRLB = 0 << TCA_SINGLE_ALUPD_bp            /* Auto Lock Update: disabled */
	                    | 0 << TCA_SINGLE_CMP0EN_bp         /* Setting: disabled */
	                    | 0 << TCA_SINGLE_CMP1EN_bp         /* Setting: disabled */
	                    | 0 << TCA_SINGLE_CMP2EN_bp         /* Setting: disabled */
	                    | TCA_SINGLE_WGMODE_SINGLESLOPE_gc; /*  */
	TCA0.SINGLE.CTRLC = 1 << TCA_SINGLE_CMP0OV_bp    /* Setting: enabled */
		| 1 << TCA_SINGLE_CMP1OV_bp  /* Setting: enabled */
		| 0 << TCA_SINGLE_CMP2OV_bp; /* Setting: disabled */
	TCA0.SINGLE.PER = 50;
	TCA0.SINGLE.CMP0 = 25;  //Dette definerer duty cycle ut på PD0
	
}
