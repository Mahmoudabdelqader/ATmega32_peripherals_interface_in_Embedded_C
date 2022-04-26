/*
 * GPIO.c
 *
 *  Created on: Apr 26, 2022
 *      Author: Mahmoud Mohamed Abd-Elqader
 */
/* includes */
#include "GPIO.h"

/*============================ 1.LED functions ==========================================*/
void led_init(Light_Emitting_Diode *pled){
	*pled->DDR |= (1<<pled->pin);	// configure pin as an output pin.
	*pled->PRT &= ~(1<<pled->pin);	// pin is initially low.
	return;	// exit function.
}
void led_turn_on(Light_Emitting_Diode *pled){
	*pled->PRT |= (1<<pled->pin);	// set pin.
	return; // exit function.
}
void led_turn_off(Light_Emitting_Diode *pled){
	*pled->PRT &= ~(1<<pled->pin); 	// reset pin.
	return;	// exit function.
}
void led_toggle(Light_Emitting_Diode *pled){
	*pled->PRT ^= (1<<pled->pin);	// toggle pin value.
}

/*============================ 2.push button functions ==================================*/
void btn_init(Push_Button *pbtn){
	*pbtn->DDR &= ~(1<<pbtn->pin);	// configure pin as an input pin.
	pbtn->click = 0;
	return;	// exit function.
}
void btn_click_to_toggle_pin(Push_Button *pbtn,volatile uint8_t *PRT, uint8_t pin){
	if(btn_state(pbtn) == 1){
		if(pbtn->click == 0){
			*PRT ^= (1<<pin);
			pbtn->click = 1;
			asm("nop"); // for synchronization.
		}
	}
	else if(btn_state(pbtn) == 0){
		pbtn->click = 0;
	}
	return;	// exit function.
}
int16_t btn_state(Push_Button *pbtn){
	return ( (*pbtn->PIN&(1<<pbtn->pin)) >> pbtn->pin );
}

/*============================ 3.LCD functions===========================================*/
/***********4 Bits mode*****************/
static void lcd_4bits_mode_send_command(Liquid_Crystal_Display_4_bits *plcd, uint8_t CMD){
	// send upper nibble of command.
	for(uint8_t i=4; i<=7; i++){
		if((int)((CMD&(1<<i))>>i) == 1){
			*plcd->data_PRT |= (1<<plcd->data_pins[i-4]);
		}
		else if((int)((CMD&(1<<i))>>i) == 0){
			*plcd->data_PRT &= ~(1<<plcd->data_pins[i-4]);
		}
		else{
			// Nothing.
		}
		asm("nop");	// Synchronization
	}
	*plcd->control_PRT &= ~(1<<plcd->control_pins[0]);	// RS=0, command register.
	// Enable pulse.
	*plcd->control_PRT |= (1<<plcd->control_pins[1]);
	_delay_ms(10);
	*plcd->control_PRT &= ~(1<<plcd->control_pins[1]);
	_delay_ms(10);

	// send lower nibble of command.
	for(uint8_t i=0; i<=3; i++){
		if((int)((CMD&(1<<i))>>i) == 1){
			*plcd->data_PRT |= (1<<plcd->data_pins[i]);
		}
		else if((int)((CMD&(1<<i))>>i) == 0){
			*plcd->data_PRT &= ~(1<<plcd->data_pins[i]);
		}
		else{
			// Nothing.
		}
		asm("nop");	// Synchronization
	}
	// Enable pulse.
	*plcd->control_PRT |= (1<<plcd->control_pins[1]);
	_delay_ms(10);
	*plcd->control_PRT &= ~(1<<plcd->control_pins[1]);
	_delay_ms(10);
	return; // exit function.
}
void lcd_4bits_mode_init(Liquid_Crystal_Display_4_bits *plcd){
	// configure data pins as an output pins;
	for(uint8_t i=0; i<4; i++)
		*plcd->data_DDR |= (1<<plcd->data_pins[i]);

	// configure control pins as an output pins.
	for(uint8_t i=0; i<2; i++)
		*plcd->control_DDR |= (1<<plcd->control_pins[i]);

	_delay_ms(20);	// LCD Power ON delay always > 15 ms.

	// send initialization commands.
	lcd_4bits_mode_send_command(plcd, __init__);
	_delay_ms(1);

	lcd_4bits_mode_send_command(plcd, _4_bits_2_lines_5x8_dots);
	_delay_ms(1);

	lcd_4bits_mode_send_command(plcd, _display_on_cursor_on_no_blinking);
	_delay_ms(1);

	lcd_4bits_mode_send_command(plcd, _increment_cursor);
	_delay_ms(1);

	lcd_4bits_mode_send_command(plcd, _clear_display);
	_delay_ms(1);

	return;	// exit function.
}

void lcd_4bits_mode_print_character(Liquid_Crystal_Display_4_bits *plcd, uint8_t character){
	// send upper nibble of command.
	for(uint8_t i=4; i<=7; i++){
		if((int)((character&(1<<i))>>i) == 1){
			*plcd->data_PRT |= (1<<plcd->data_pins[i-4]);
		}
		else if((int)((character&(1<<i))>>i) == 0){
			*plcd->data_PRT &= ~(1<<plcd->data_pins[i-4]);
		}
		else{
			// Nothing.
		}
		asm("nop");	// Synchronization
	}
	*plcd->control_PRT |= (1<<plcd->control_pins[0]);	// RS=1, data register.
	// Enable pulse.
	*plcd->control_PRT |= (1<<plcd->control_pins[1]);
	_delay_ms(10);
	*plcd->control_PRT &= ~(1<<plcd->control_pins[1]);
	_delay_ms(10);

	// send lower nibble of command.
	for(uint8_t i=0; i<=3; i++){
		if((int)((character&(1<<i))>>i) == 1){
			*plcd->data_PRT |= (1<<plcd->data_pins[i]);
		}
		else if((int)((character&(1<<i))>>i) == 0){
			*plcd->data_PRT &= ~(1<<plcd->data_pins[i]);
		}
		else{
			// Nothing.
		}
		asm("nop");	// Synchronization
	}
	// Enable pulse.
	*plcd->control_PRT |= (1<<plcd->control_pins[1]);
	_delay_ms(10);
	*plcd->control_PRT &= ~(1<<plcd->control_pins[1]);
	_delay_ms(10);
	return; // exit function.
}

/*============================ 4. Keypad functions===========================================*/
void keypad_init(Keypad_4X4 *pk){
	// configure rows pins as an output pins & columns pins as an input pins.
	for(uint8_t i=0; i<4; i++){
		// rows pins >> output.
		*pk->row_DDR |= (1<<pk->row_pins[i]);
		asm("nop");

		// all rows pins >> low.
		*pk->row_PRT &= ~(1<<pk->row_pins[i]);
		asm("nop");

		// columns pins >> input.
		*pk->col_DDR &= ~(1<<pk->col_pins[i]);
		asm("nop");

		// activate PUR.
		*pk->col_PRT |= (1<<pk->col_pins[i]);
		asm("nop");
	}
	return;
}
void keypad_read(Keypad_4X4 *pk, uint8_t* key){
	/* Idea: activate only one row then read all columns.
	 * when a key is pressed, I already know it's row and it's column.
	 * Note that: row is active low.
	 */
	for(uint8_t i=0; i<4; i++){
		// Deactivate all rows.
		*pk->row_PRT |= (1<<pk->row_pins[0]);
		*pk->row_PRT |= (1<<pk->row_pins[1]);
		*pk->row_PRT |= (1<<pk->row_pins[2]);
		*pk->row_PRT |= (1<<pk->row_pins[3]);

		// Activate specific row.
		*pk->row_PRT &= ~(1<<pk->row_pins[i]);

		// Read all columns.
		for(uint8_t j=0; j<4; j++){
			if( ((*pk->col_PIN&(1<<pk->col_pins[j]))>>pk->col_pins[j]) == 0){
				*key = (*pk->key_map)[i][j];
				while(((*pk->col_PIN&(1<<pk->col_pins[j]))>>pk->col_pins[j]) == 0);
			}
			asm("nop");
		}
	}
}
