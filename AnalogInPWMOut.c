// AnalogInPWMOut.c

// 10k ohm pot on PC4/ADC4 (pin 27)
// LED on PD6/OC0A (pin 12), PWMed in response to 10k ohm pot position

#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1000000UL			// define it now as 1 MHz unsigned long
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
	
	DDRD |= (1 << PD6);				// set PD6/OC0A (pin 12) for output
	
	/*
	ADMUX - ADC Multiplexer Selection Register
	
	bit          7           6          5         4        3         2          1          0
	name       REFS1       REFS0      ADLAR       -       MUX3      MUX2       MUX1       MUX0
	set to       0           1          1         0        0         1          0          0
	
	REFS1 = 0    use AVCC for reference voltage
	REFS0 = 1
	
	ADLAR = 1    left justify ADC result in ADCH/ADCL
	
	bit 4 = 0
	
	MUX3 = 0     use PC4/ADC4 (pin 27) for input
	MUX2 = 1
	MUX1 = 0
	MUX0 = 0
	*/
	ADMUX = 0b01100100;
	
	/*
	ADCSRA - ADC Control and Status Register A
	
	bit          7           6            5          4          3            2           1           0
	name        ADEN        ADSC        ADATE       ADIF       ADIE        ADPS2       ADPS1       ADPS0
	set to       1           0            1          0          1            0           1           1
	
	ADEN = 1     enable ADC
	ADSC = 0     don't start ADC yet
	ADATE = 1    enable ADC auto trigger (i.e. use free running mode)
	ADIF = 0     don't set ADC interrupt flag
	ADIE = 1     enable ADC interrupt
	
	ADPS2 = 0
	ADPS1 = 1    1 MHz clock / 8 = 125 kHz ADC clock
	ADPS0 = 1
	*/
	ADCSRA = 0b10101011;
	
	/*
	ADCSRB - ADC Control and Status Register B
	
	bit         7           6           5           4           3         2           1           0
	name        -          ACME         -           -           -       ADTS2       ADTS1       ADTS0
	set to      0           0           0           0           0         0           0           0
	
	bit 7 = 0
	ACME = 0     don't enable analog comparator multiplexer
	bit 5 = 0
	bit 4 = 0
	bit 3 = 0
	ADTS2 = 0
	ADTS1 = 0    free running mode
	ADTS0 = 0
	*/
	ADCSRB = 0b00000000;
	
	/*
	TCCR0A - Timer/Counter 0 Control Register A
	
	bit           7         6         5         4        3       2        1        0
	name        COM0A1    COM0A0    COM0B1    COM0B0     -       -      WGM01    WGM00
	set to        1         0         0         0        0       0        1        1
	
	COM0A1 = 1    when Timer/Counter 0 (TCNT0) rolls over, set pin OC0A to high
	COM0A0 = 0    when Timer/Counter 0 (TCNT0) equals OCR0A, set pin OC0A to low
	
	COM0B1 = 0    normal port operation, OC0B disconnected
	COM0B0 = 0
	
	bit 3 = 0
	bit 2 = 0
	
	WGM01 = 1     Fast PWM mode, also see TCCR0B
	WGM00 = 1
	*/
	TCCR0A = 0b10000011;
	
	/*
	TCCR0B - Timer/Counter 0 Control Register B
	
	bit           7          6        5       4         3         2         1        0
	name        FOC0A      FOC0B      -       -       WGM02      CS02      CS01     CS00
	set to        0          0        0       0         0         0         0        1
	
	FOC0A = 0     not used in PWM mode
	FOC0B = 0
	
	bit 5 = 0
	bit 4 = 0
	
	WGM02 = 0     Fast PWM mode, also see TCCR0A
	
	CS02 = 0
	CS01 = 0      no prescaling
	CS00 = 1
	*/
	TCCR0B = 0b00000001;
	
	sei();			// enable interrupts

	ADCSRA |= (1 << ADSC);		// start ADC
	
    while (1) { }
	
	return(0);					// should never get here, this is to prevent a compiler warning
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ISR(ADC_vect) {
	OCR0A = ADCH;				// assign contents of ADC high register to output compare register
}



