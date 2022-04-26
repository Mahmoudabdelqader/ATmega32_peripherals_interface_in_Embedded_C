/*
 * GPIO.h
 *
 *  Created on: Apr 26, 2022
 *      Author: Mahmoud Mohamed Abd-Elqader
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

/* includes */
#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>
#include<string.h>
#include "lcd_alpha_16x2_commands.h"

/* 1.1.LED structure */
typedef struct light_emitting_diode{
	volatile uint8_t* DDR;	// data direction register.
	volatile uint8_t* PRT;	// port register.
			 uint8_t  pin;	// pin number.
}Light_Emitting_Diode;

/* 1.2.LED functions */
void led_init(Light_Emitting_Diode *pled);
void led_turn_on(Light_Emitting_Diode *pled);
void led_turn_off(Light_Emitting_Diode *pled);
void led_toggle(Light_Emitting_Diode *pled);


/* 2.1.push button structure */
typedef struct push_button{
	volatile uint8_t* DDR;	// data direction register.
	volatile uint8_t* PRT;	// port register, used to activate pull-up resistor.
	volatile uint8_t* PIN;	// PIN register.
			 uint8_t  pin; 	// pin number.
			 uint8_t  click;
}Push_Button;

/* 2.2. push button functions */
void btn_init(Push_Button *pbtn);
void btn_click_to_toggle_pin(Push_Button *pbtn, volatile uint8_t *PRT, uint8_t pin);
int16_t btn_state(Push_Button *pbtn);


/* 3.1.LCD structure */
typedef struct liquid_crystal_display_4_bits{
	volatile uint8_t* data_DDR;
	volatile uint8_t* control_DDR;

	volatile uint8_t* data_PRT;
	volatile uint8_t* control_PRT;
			 uint8_t  data_pins[4];	// D4, D5, D6, D7
			 uint8_t  control_pins[2];	//RS, EN
			 uint8_t  mode;
}Liquid_Crystal_Display_4_bits;

typedef struct liquid_crystal_display_8_bits{
	volatile uint8_t* data_DDR;
	volatile uint8_t* control_DDR;

	volatile uint8_t* data_PRT;
	volatile uint8_t* control_PRT;
			 uint8_t  control_pins[2];	//RS, EN
}Liquid_Crystal_Display_8_bits;
/* 3.2.LCD functions */
/* 4 BITS MODE */
void lcd_4bits_mode_init(Liquid_Crystal_Display_4_bits *plcd);
void lcd_4bits_mode_print_character(Liquid_Crystal_Display_4_bits *plcd, uint8_t character);

/* 4.1.Keypad structure */
typedef struct keypad_4x4{
	volatile uint8_t* row_DDR;
	volatile uint8_t* col_DDR;
	volatile uint8_t* row_PRT;
	volatile uint8_t* col_PRT;
	volatile uint8_t* col_PIN;

	uint8_t row_pins[4];
	uint8_t col_pins[4];

	uint8_t (*key_map)[4][3];
}Keypad_4X4;

/* 4.2.keypad functions */
void keypad_init(Keypad_4X4 *pk);
void keypad_read(Keypad_4X4 *pk, uint8_t* key);

#endif /* GPIO_GPIO_H_ */
