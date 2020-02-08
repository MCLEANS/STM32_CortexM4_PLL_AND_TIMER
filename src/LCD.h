/*
 * LCD.h
 *
 *  Created on: Aug 24, 2019
 *      Author: MCLEANS
 */

#ifndef LCD_H_
#define LCD_H_

#define D0PORT	GPIOD
#define D0PIN	0
#define D1PORT 	GPIOD
#define D1PIN 	1
#define D2PORT	GPIOD
#define D2PIN	2
#define D3PORT	GPIOD
#define D3PIN	3
#define D4PORT	GPIOD
#define D4PIN	4
#define D5PORT	GPIOD
#define D5PIN	5
#define D6PORT	GPIOD
#define D6PIN	6
#define D7PORT	GPIOD
#define D7PIN	7
#define ENABLEPORT	GPIOC
#define ENABLEPIN	12
#define RWPORT	GPIOC
#define RWPIN	10
#define RSPORT GPIOC
#define RSPIN 11

#define TIMEDELAYBEFOREENABLE 4
#define TIMEDELAYBEFOREDISABLE  8

#include "stm32f4xx.h"
#include "stdio.h"

namespace custom_drivers {



class LCD {
private:
	int duration = 0;
	char data[16];
	int row = 0;
	int column = 0;





public:
	LCD();
	virtual ~LCD();
	void enable_port(GPIO_TypeDef* PORT);
	void configure_pin_for_output(GPIO_TypeDef* PORT, uint8_t PIN);
	void configure_pins();
	void initialize();
	void delay_ms(int duration);
	void send_bit_to_pin(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t state);
	void send_byte(char byte);
	void enable();
	void set_to_read_mode();
	void set_to_write_mode();
	void set_to_instruction_mode();
	void set_to_character_mode();
	void send_character(char character);
	void send_instruction(char byte);
	void clear();
	void newline();
	void send_string(char *data);
	void set_cursor_to_location(int row, int column);


};

} /* namespace custom_drivers */

#endif /* LCD_H_ */
