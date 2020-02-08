/*
 * LCD.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: MCLEANS
 */

#include "LCD.h"

namespace custom_drivers {

LCD::LCD() {}

LCD::~LCD() {}

void LCD::enable_port(GPIO_TypeDef* PORT){
	if(PORT == GPIOA ) RCC->AHB1ENR |= (1<<0);
	if(PORT== GPIOB ) RCC->AHB1ENR |= (1<<1);
	if(PORT == GPIOC ) RCC->AHB1ENR |= (1<<2);
	if(PORT == GPIOD ) RCC->AHB1ENR |= (1<<3);
	if(PORT == GPIOE ) RCC->AHB1ENR |= (1<<4);
}

void LCD::configure_pin_for_output(GPIO_TypeDef *PORT, uint8_t PIN){


		PORT->MODER |= (1<<(PIN*2));
		PORT->MODER &= ~(1<<((PIN*2)+1));

		PORT->OTYPER |= (1<<PIN);

		PORT->OSPEEDR &= ~(1<<((PIN*2)+1));
		PORT->OSPEEDR |= (1<<(PIN*2));


		PORT->PUPDR &= ~(1<<(PIN*2));
		PORT->PUPDR &= ~(1<<((PIN*2)+1));


}

void LCD::configure_pins(){
	configure_pin_for_output(D0PORT,D0PIN);
	configure_pin_for_output(D1PORT,D1PIN);
	configure_pin_for_output(D2PORT,D2PIN);
	configure_pin_for_output(D3PORT,D3PIN);
	configure_pin_for_output(D4PORT,D4PIN);
	configure_pin_for_output(D5PORT,D5PIN);
	configure_pin_for_output(D6PORT,D6PIN);
	configure_pin_for_output(D7PORT,D7PIN);
	configure_pin_for_output(ENABLEPORT,ENABLEPIN);
	configure_pin_for_output(RWPORT,RWPIN);
	configure_pin_for_output(RSPORT,RSPIN);

}



void LCD::delay_ms(int duration){
	int i = duration*168;
	while(i--){}

}
void LCD::send_bit_to_pin(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t state){
	if(state) PORT->ODR |= (1<<PIN);
	if(!state) PORT->ODR &= ~(1<<PIN);
}

void LCD::send_byte(char byte){
	send_bit_to_pin(D0PORT,D0PIN,byte & 0b00000001);
	send_bit_to_pin(D1PORT,D1PIN,byte & 0b00000010);
	send_bit_to_pin(D2PORT,D2PIN,byte & 0b00000100);
	send_bit_to_pin(D3PORT,D3PIN,byte & 0b00001000);
	send_bit_to_pin(D4PORT,D4PIN,byte & 0b00010000);
	send_bit_to_pin(D5PORT,D5PIN,byte & 0b00100000);
	send_bit_to_pin(D6PORT,D6PIN,byte & 0b01000000);
	send_bit_to_pin(D7PORT,D7PIN,byte & 0b10000000);

	delay_ms(TIMEDELAYBEFOREDISABLE);
	//turn off enable when we are done sending data to lcd
	send_bit_to_pin(ENABLEPORT,ENABLEPIN,0);
}

void LCD::enable(){
	delay_ms(TIMEDELAYBEFOREENABLE);
	send_bit_to_pin(ENABLEPORT,ENABLEPIN,1);
}

void LCD::set_to_read_mode(){
	send_bit_to_pin(RWPORT,RWPIN,1);
}

void LCD::set_to_write_mode(){
	send_bit_to_pin(RWPORT,RWPIN,0);
}

void LCD::set_to_character_mode(){
	send_bit_to_pin(RSPORT,RSPIN,1);
}

void LCD::set_to_instruction_mode(){
	send_bit_to_pin(RSPORT,RSPIN,0);
}

void LCD::send_character(char character){
	set_to_write_mode();
	set_to_character_mode();
	enable();
	send_byte(character);

}

void LCD::send_instruction(char byte){
	set_to_write_mode();
	set_to_instruction_mode();
	enable();
	send_byte(byte);
}

void LCD::clear(){
	send_instruction(0b00000001);
}

void LCD::newline(){
	send_instruction(0b11000000);
}

void LCD::initialize(){
	configure_pins();
	send_instruction(0b00111000); //set the LCD TO 8 bit mode
	send_instruction(0b00001110); //turn on display and cursor
	send_instruction(0b00000110); //set to increment cursor by 1
	send_instruction(0b10000000);
	send_instruction(0b00001100); //turn of cursor
}

void LCD::send_string(char *data){
	for(;*data;data++){
		send_character(*data);
	}
}

void LCD::set_cursor_to_location(int row, int column){


}

} /* namespace custom_drivers */
