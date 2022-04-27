/*
 * main.c
 *
 *  Created on: Apr 27, 2022
 *      Author: Mahmoud Mohamed Abd-Elqader
 */
/* includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void){
	/* generating square wave using Timer0 in CTC mode */
	/*
	 * To calculate frequency or OCR0 value:-
	 * OCR = ( clk(timer)/frequency(wave) ) / 2 - 1;
	 * for example: clk(timer) = 8_MHZ/8 = 1_MHZ
	 * 				frequency(wave) = 5_KHZ
	 * 	So, OCR0 = (1_MHZ/5_KHZ)/2 - 1 = 200/2 - 1 = 100 - 1 = 99
	 * */
	// OC0, PB3 >> output.
	DDRB |= (1<<PB3);

	// CTC mode.
	TCCR0 &= ~(1<<WGM00);
	TCCR0 |= (1<<WGM01);

	// Toggle OC0 on compare match
	TCCR0 |=  (1<<COM00);
	TCCR0 &= ~(1<<COM01);

	// clk/8
	TCCR0 &= ~(1<<CS00);
	TCCR0 |=  (1<<CS01);
	TCCR0 &= ~(1<<CS02);

	// Compare value.
	OCR0 = 99;

	/*===============================================================*/

	/* generating PWM using Timer2 in fast PWM mode */
	// OC2, PD7 >> output.
	DDRD |= (1<<PD7);

	// Fast PWM mode.
	TCCR2 |= (1<<WGM00);
	TCCR2 |= (1<<WGM01);

	// Non-inverted, Clear OC0 on compare match, set OC0 at TOP.
	TCCR2 &= ~(1<<COM00);
	TCCR2 |=  (1<<COM01);

	// clk (no pre-scaling)
	TCCR2 |=  (1<<CS20);
	TCCR2 &= ~(1<<CS21);
	TCCR2 &= ~(1<<CS22);

	// duty cycle value.
 	unsigned char duty;

	/*===============================================================*/

 	/* Hardware delay using timer1 */
 	void Timer1_delay_ms(unsigned long _time){
 		/* to get 1_ms delay consider frequency prescaler = 64
 		 * and F_cpu = 8_MHZ. So, F_timer = 8_MHZ/64 = 125_KHZ
 		 * time_count = 1/125_KHZ = 0.000008 sec.
 		 * to get number of counts in 1_ms = 0.001/0.000008 = 125
 		 * so number of counts = 125-1 = 124 count = 0x7C.
 		 * */
 		while(_time>0){
 	 		// load TCNT1L.
 	 		TCNT1L = 0xFF - 0x7C;

 	 		// CLK/64
 	 		TCCR1B = (1<<CS10);
 	 		TCCR1B |= (1<<CS11);
 	 		TCCR1B &= ~(1<<CS12);

 	 		// wait until TOV1 set.
 	 		while(!(TIFR&(1<<TOV1)));

 	 		// clear TOV1.
 	 		TIFR |= (1<<TOV1);

 	 		_time--;
 		}
 	}
 	void Timer1_delay_s(unsigned long time){
 		/* to get 1_s delay consider frequency prescaler = 256
 		 * and F_cpu = 8_MHZ. So, F_timer = 8_MHZ/256 = 31250_HZ
 		 * time_count = 1/31250_HZ = 0.000032 sec.
 		 * to get number of counts in 1_s = 1/0.000032 = 31250
 		 * so number of counts = 31250-1 = 31249 count = 0x7A11.
 		 * */
 		while(time>0){
 	 		// load TCNT1L.
 	 		TCNT1 = 0xFFFF - 0x7A11;

 	 		// CLK/64
 	 		TCCR1B &= ~(1<<CS10);
 	 		TCCR1B &= ~(1<<CS11);
 	 		TCCR1B |=  (1<<CS12);

 	 		// wait until TOV1 set.
 	 		while(!(TIFR&(1<<TOV1)));

 	 		// clear TOV1.
 	 		TIFR |= (1<<TOV1);

 	 		time--;
 		}
 	}
 	DDRD |= (1<<PD5);
	/*===============================================================*/

	for(;;){
		for(duty=0; duty<=255; duty++)
		{
			OCR2=duty;  /*increase the LED light intensity*/
		}
		PORTD ^= (1<<PD5);
		Timer1_delay_ms(1000);

	}
	return 0;
}

