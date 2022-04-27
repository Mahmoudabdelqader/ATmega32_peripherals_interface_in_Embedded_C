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
	// External Interrupt Request 0, 1 and 2 Enable.
	GICR |= (1<<INT2) | (1<<INT1) | (1<<INT0);

	// The falling edge on INT2 activates the interrupt.
	MCUCSR |=  (1<<ISC2);

	// Any logical change on INT1 generates an interrupt request.
	MCUCR |=  (1<<ISC10);
	MCUCR &= ~(1<<ISC11);

	// The rising edge of INT0 generates an interrupt request.
	MCUCR |= (1<<ISC00);
	MCUCR |= (1<<ISC01);

	/********************Toggle PD4 with INT0**************************/
	// configure PD4 >> output.
	DDRD |= (1<<PD4);

	/********************Toggle PB1 with INT1**************************/
	// configure PD4 >> output.
	DDRB |= (1<<PB1);

	/********************Toggle PD0 with INT2**************************/
	// configure PD4 >> output.
	DDRD |= (1<<PD0);


	// Enable global interrupts.
	sei();


	while(1);

	return 0;
}

ISR(INT2_vect){
	PORTD |= (1<<PD0);
}

ISR(INT1_vect){
	PORTB ^= (1<<PB1);
}

ISR(INT0_vect){
	PORTD ^= (1<<PD4);
}



