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

//pinDefines.h to be created later


void PWM_Init(void);
void ClkSelect(void);
void Timer0_Init(void);

int lookUp1[] = {   //Need 5 clock cycles LESS than PER (MAX value) register, since reti (return from interrupt) takes 4 clock cycles.
	225, 132, 225,
	0, 0, 0,
	225, 132, 225
};

int lookUpTest[] = {   //Need 5 clock cycles LESS than PER (MAX value) register, since reti (return from interrupt) takes 4 clock cycles.
	33, 67, 33,
	0, 0, 0,
	33, 67, 33
};

int lookUp2[] = {	//Need 5 clock cycles LESS than PER (MAX value) register, since reti (return from interrupt) takes 4 clock cycles.
	66, 90, 66,
	0, 0, 0,
	66, 90, 66
};


int main(void)
{
	
	ClkSelect();
	Timer0_Init();
	//TCA0.SPLIT.LCMP0 = 33; /* Compare value Channel 0: 0x20 */    //Initialization of first duty-cycle value (PD0). Not sure if this is needed when lookUp table is used
	//TCA0.SPLIT.HCMP0 = 10; /* Compare value Channel 0: 0x10 */    //Initialization of first duty-cycle value (PD1). Not sure if this is needed when lookUp table is used
	PWM_Init();   
	sei();  //Global interrupts enabled
	
    while (1) 
    {
		
    }
}

void PWM_Init(void){
	
	PORTD_DIR = 0XFF; //Set PORTD as output.
	
	TCA0.SPLIT.LPER = 127; /*  Low-byte Timer Period Register: 67 */  //This sets the TOP counter value (PD0).
	TCA0.SPLIT.HPER = 127; /*  High-byte Timer Period Register: 67 */ //This sets the TOP counter value (PD1).
	
	TCA0.SPLIT.INTCTRL = (0<<4) | (0<<5) | (0<<6) | (1<<1) | (0<<0);  //Enables overflow interrupt in Timer 0
	//TCA0.SPLIT.INTFLAGS = (1<<3) | (1<<4) | (1<<5) | (1<<1) | (1<<0);  //Enables overflow interrupt in Timer 0
	
}

void ClkSelect(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); //Forces 20MHz CPU clock frequency
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, (1<<7)); //Sender klokke-signal ut på PA7-pin
	
	PORTA_DIR |= PIN7_bm; //PA7 as output for CPU clock frequency check
}

void Timer0_Init(void){
	PORTMUX.TCAROUTEA |= (PORTMUX_TCA00_bm | PORTMUX_TCA01_bm); //Enables PORTMUXD to achieve Waveform Output on PD0 and PD1
	
	
	TCA0.SPLIT.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc    /* System Clock */
	                    | 1 << TCA_SPLIT_ENABLE_bp; /* Module Enable: enabled */
					
	TCA0.SPLIT.CTRLB =  1 << TCA_SPLIT_HCMP0EN_bp    /* High Compare 0 Enable: enabled */
						| 1 << TCA_SPLIT_LCMP0EN_bp;  /* Low Compare 0 Enable: enabled */
	
						
	TCA0.SPLIT.CTRLD = 1 << TCA_SPLIT_SPLITM_bp; /* Split Mode : enabled */
}


ISR(TCA0_HUNF_vect){
	TCA0_SPLIT_INTFLAGS = (1<<OVF) | (1<<1); //clearing the interrupt flag.  For some reason this messes up the PWM output more. 
	
	static int num;

	TCA0.SPLIT.LCMP0 = lookUp2[num]; /* Checks table for next duty cycle value */
	TCA0.SPLIT.HCMP0 = lookUp2[num+3]; /* Checks table for next duty cycle value */
	TCA0.SPLIT.LCNT = 127;  //Since split mode uses downcount, the counter value is reset to it's max value.
	TCA0.SPLIT.HCNT = 127;  //Since split mode uses downcount, the counter value is reset to it's max value.
	
	if(++num >= 6){ // Pre-increment num then check it's below 10.
		num = 0;       // Reset num.
	}
	
	
	//TCA0.SPLIT.INTFLAGS = (1<<1) | (1<<0); 
	
}
