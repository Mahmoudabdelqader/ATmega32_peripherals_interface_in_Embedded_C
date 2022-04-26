/*
 * main.c
 *
 *  Created on: Apr 26, 2022
 *      Author: Mahmoud Mohamed Abd-Elqader.
 */
/* includes */
#include <avr/io.h>
#include <util/delay.h>
#include "GPIO/GPIO.h"

int main(void){
	// LED configuration.
	Light_Emitting_Diode led1;
	led1.DDR = &DDRB;
	led1.PRT = &PORTB;
	led1.pin = PB0;

	// Push Button configuration.
	Push_Button btn1;
	btn1.DDR = &DDRA;
	btn1.PRT = &PORTA;
	btn1.PIN = &PINA;
	btn1.pin = PA7;

	// LCD 4bit mode configuration.
	Liquid_Crystal_Display_4_bits lcd;
	lcd.data_DDR = &DDRA;
	lcd.data_PRT = &PORTA;

	lcd.control_DDR = &DDRB;
	lcd.control_PRT = &PORTB;

	lcd.data_pins[0]    = PA3; //D4
	lcd.data_pins[1]    = PA4; //D5
	lcd.data_pins[2]    = PA5; //D6
	lcd.data_pins[3]    = PA6; //D7

	lcd.control_pins[0] = PB2; //RS
	lcd.control_pins[1] = PB3; //EN

	// Keypad configuration.
	Keypad_4X4 keypad;
	keypad.col_DDR = &DDRC;
	keypad.col_PIN = &PINC;
	keypad.col_PRT = &PORTC;
	keypad.col_pins[0] = PC0;
	keypad.col_pins[1] = PC1;
	keypad.col_pins[2] = PC2;
	keypad.col_pins[3] = PC3;

	keypad.row_DDR = &DDRC;
	keypad.row_PRT = &PORTC;
	keypad.row_pins[0] = PC4;
	keypad.row_pins[1] = PC5;
	keypad.row_pins[2] = PC6;
	keypad.row_pins[3] = PC7;

	uint8_t keymap[4][3] = { {'1', '2', '3'},
						     {'4', '5', '6'},
							 {'7', '8', '9'},
							 {'*', '0', '#'}
							};
	keypad.key_map = &keymap;

	// led1 init.
	led_init(&led1);
	asm("nop");	// for synchronization.

	// btn1 init.
	btn_init(&btn1);
	asm("nop");	// for synchronization.

	// lcd 4bit mode init.
	lcd_4bits_mode_init(&lcd);
	lcd_4bits_mode_print_character(&lcd, (*keypad.key_map)[3][2]);

	// keypad init.
	keypad_init(&keypad);
	uint8_t key='a';

	for(;;){
			btn_click_to_toggle_pin(&btn1, led1.PRT, led1.pin);
			keypad_read(&keypad, &key);
			if(key!='\0'){
				lcd_4bits_mode_print_character(&lcd, key);
				key='\0';
			}

	}
	return 0;
}

