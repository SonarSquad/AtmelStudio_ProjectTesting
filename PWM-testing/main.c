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
#define OVF 0

int lookUp1[] = {
5 ,10 ,25, 10, 5,
0 , 0, 0, 0, 0,
};

volatile int IntCount = 0;

// Må lage en pinDefines.h som inneholder alle defines for å gjøre alt mere oversiktlig!


void PWM_Init(void);
void ClkSelect(void);
void Timer0_Init(void);



int main(void)
{
	sei();  //Global interrupts enabled
	ClkSelect();
	Timer0_Init();
	TCA0.SINGLE.CMP0 = 10;  //Dette definerer duty cycle ut på PD0
	PWM_Init();   
	
	
	PORTF_DIR = PIN5_bm;  //Just for LED testing
	
	
    while (1) 
    {
		
		
		
		// Code for duty cycle modulation will be under this line. 
		
		//if (IntCount > 3) {
			//IntCount = 0;
			////cli();
			//PORTD_OUTCLR = PIN0_bm;
			//PORTD_DIRCLR = PIN0_bm;
			//
			////PORTF.OUTTGL = PIN5_bm;
			//
			//PORTD_DIR = PIN0_bm;
			////PORTF.OUT = PIN5_bm;
			//
			////sei();
		//}
		
		
		// Pseudocode:
		
		// set PWM1 as output
		
		// generate PWM out on PWM 1
		// 
    }
}

void PWM_Init(void){
	
	PORTD_DIR = 0XFF; //Set PORTD as output.

	
	
	TCA0.SINGLE.PER = 500;  //Dette definerer PWM-periodetiden.
	//TCA0.SINGLE.CMP1 = 5;  //Dette definerer duty cycle ut på PD1
	
	TCA0.SINGLE.INTCTRL = (1<<OVF);  //Enables overflow interrupt in Timer 0
	
	
	
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU clock frequency
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (1<<7)); //Sender klokke-signal ut på PA7-pin
	
	
	PORTA_DIR |= PIN7_bm; //PA7 as output for clock frequency check
	
}

void Timer0_Init(void){
	PORTMUX.TCAROUTEA |= (PORTMUX_TCA00_bm | PORTMUX_TCA01_bm); //Enables PORTMUXD to achieve Waveform Output on the PWM Pins
	
	
	TCA0.SINGLE.CTRLB = 0 << TCA_SINGLE_ALUPD_bp            /* Auto Lock Update: disabled */
	                    | 1 << TCA_SINGLE_CMP0EN_bp         /* Setting: enabled */
	                    | 1 << TCA_SINGLE_CMP1EN_bp         /* Setting: enabled */
	                    | 1 << TCA_SINGLE_CMP2EN_bp         /* Setting: enabled */
	                    | TCA_SINGLE_WGMODE_SINGLESLOPE_gc; /*  */
						
	TCA0.SINGLE.CTRLC = 1 << TCA_SINGLE_CMP0OV_bp    /* Setting: enabled */
						| 1 << TCA_SINGLE_CMP1OV_bp  /* Setting: enabled */
						| 0 << TCA_SINGLE_CMP2OV_bp; /* Setting: disabled */
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc    /* System Clock */
	                    | 1 << TCA_SINGLE_ENABLE_bp; /* Module Enable: enabled */

}


ISR(TCA0_OVF_vect){
	TCA0_SINGLE_INTFLAGS = (1<<OVF) | (1<<4); //clearing the interrupt flag
	static int num;
	TCA0.SINGLE.CMP0 = looup1[num];
	if(++num >= 10){ // Pre-increment num then check it's below 10.
		num = 0;       // Reset num.
	}
	//switch (TCA0.SINGLE.CMP0)
	//{
		//case 10:
			//TCA0.SINGLE.CMP0 = 20;
			//TCA0.SINGLE.CNT = 0;
			//break;
		//
		//case 20:
			//TCA0.SINGLE.CMP0 = 30;
			//TCA0.SINGLE.CNT = 0;
			//break;
		//
		//case 30:
			//TCA0.SINGLE.CMP0 = 5;
			//TCA0.SINGLE.CNT = 0;
			//break;
		//
		//case 5:
			//TCA0.SINGLE.CMP0 = 10;
			//TCA0.SINGLE.CNT = 0;
			//break;
	//}
	
	
	
	//if (TCA0.SINGLE.CMP0 == 10) {  //Dette definerer duty cycle ut på PD0
		//
		//TCA0.SINGLE.CMP0 = 20;
		//TCA0.SINGLE.CNT = 0;
		//
	//
	//}
	//
	//else if (TCA0.SINGLE.CMP0 == 20){
	//
		//TCA0.SINGLE.CMP0 = 30;
		//TCA0.SINGLE.CNT = 0;
		//
	//}
	//
	//else if (TCA0.SINGLE.CMP0 == 30){
	//
		//TCA0.SINGLE.CMP0 = 5;
		//TCA0.SINGLE.CNT = 0;
		//
	//}
	//
	//else  {
		//TCA0.SINGLE.CMP0 = 10;
		//TCA0.SINGLE.CNT = 0;
	//}
	
		
	//IntCount++;
	
	//asm (nop);
	
}