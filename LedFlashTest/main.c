/*
 * LedFlashTest.c
 *
 * Created: 2/11/2020 9:51:07 PM
 * Author : Martin Ericsson
 */ 



#define F_CPU 20e6
#include <avr/io.h>
#include <util/delay.h>

#define PEN PIN0_bp
//hei
//test2

void clk_init(void);

int main(void)
{
	PWM_init();
	
	
	
	//CLKCTRL.MCLKCTRLA = (0<<2) | (0<<1);
    //FUSE.OSCCFG = (1<<1);
	
	PORTF_DIR = PIN5_bm;

			
    while (1) 
    {
		PORTF.OUTTGL = PIN5_bm;
		_delay_ms(500);
    }
}

void clk_init (void)
{
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, )
}
