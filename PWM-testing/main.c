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
#define DIR 0

// Må lage en pinDefines.h som inneholder alle defines for å gjøre alt mere oversiktlig!


void PWM_Init(void);
void ClkSelect(void);
void Timer0_Init(void);

int lookUp1[] = {   //Need 5 clock cycles LESS than PER register. ISR takes 4 clock cycles
	16, 31, 16,
	0, 0, 0
};

int lookUp2[] = {	//Need 5 clock cycles LESS than PER register. ISR takes 4 clock cycles
	0, 0, 0,
	16, 31, 16
};




int main(void)
{
	
	ClkSelect();
	Timer0_Init();
	//TCA0.SPLIT.LCMP0 = 33; /* Compare value Channel 0: 0x20 */
	//TCA0.SPLIT.HCMP0 = 33; /* Compare value of channel 0: 0x10 */
	PWM_Init();   
	sei();  //Global interrupts enabled
	
    while (1) 
    {
		
    }
}

void PWM_Init(void){
	
	PORTD_DIR = 0XFF; //Set PORTD as output.
	
	TCA0.SPLIT.LPER = 37; /*  Low-byte Timer Period Register: 0x43 */
	TCA0.SPLIT.HPER = 37; /*  High-byte Timer Period Register: 0x43 */
	
	TCA0.SPLIT.INTCTRL = (1<<OVF);  //Enables overflow interrupt in Timer 0
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU clock frequency
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (1<<7)); //Sender klokke-signal ut på PA7-pin
	
	
	PORTA_DIR |= PIN7_bm; //PA7 as output for clock frequency check
}

void Timer0_Init(void){
	PORTMUX.TCAROUTEA |= (PORTMUX_TCA00_bm | PORTMUX_TCA01_bm); //Enables PORTMUXD to achieve Waveform Output on the PWM Pins
	
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc    /* System Clock */
	                    | 1 << TCA_SPLIT_ENABLE_bp; /* Module Enable: enabled */
					
	TCA0.SPLIT.CTRLB =  TCA_SPLIT_HCMP0EN_bm    /* High Compare 0 Enable: enabled */
						| 1 << TCA_SPLIT_LCMP0EN_bp;  /* Low Compare 0 Enable: enabled */
	
						
	TCA0.SPLIT.CTRLD = 1 << TCA_SPLIT_SPLITM_bp; /* Split Mode : enabled */
}


ISR(TCA0_OVF_vect){
	
	
	
	static int num;
	//TCA0.SINGLE.CMP0 = lookUp1[num];
	
	TCA0.SPLIT.LCMP0 = lookUp1[num]; /* Compare value Channel 0: 0x20 */
	TCA0.SPLIT.HCMP0 = lookUp2[num]; /* Compare value of channel 0: 0x10 */
	TCA0.SPLIT.LCNT = 37;
	TCA0.SPLIT.HCNT = 37;
	
	if(++num >= 6){ // Pre-increment num then check it's below 10.
		num = 0;       // Reset num.
	}
	
	TCA0.SPLIT.INTFLAGS = (1<<OVF) | (1<<4); //clearing the interrupt flag
}