/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "LCD.h"
#include <stdlib.h>

#define ARR_VALUE 640
#define PSC_VALUE 65535

char counter_value[5];
uint64_t  count = 0;

custom_drivers::LCD lcd;
char space[] = "      ";


extern "C" void TIM3_IRQHandler(void){
	if(TIM3->SR & TIM_SR_UIF){
					TIM3->SR &= ~TIM_SR_UIF;
					count++;
					itoa(count,counter_value,10);
					lcd.clear();
					lcd.send_string(space);
					lcd.send_string(counter_value);


				}

}

int main(void)
{


	//SET FLASH MEMORY LATENCY AND ENABLE PREFETCH
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
	FLASH->ACR |= FLASH_ACR_PRFTEN;

	//Enable HSI
	RCC->CR |= RCC_CR_HSION;
	//check if HSI is ready
	while(!(RCC->CR & (1<<1))){}
	//set PLL SOURCE to HSI
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	//set Division factor for the main PLL division clock to 8
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_3;
	//set main PLL multiplication factor to 168
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_3;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_5;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_7;
	//set PLL division factor for main system clock to 2
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;

	//Set APB1 clock frequency to 42MHz(prescaler of 4)
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	//Set APB2 clock frequency to 84MHz(prescaler of 2)
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	//Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	//check if PLL is ready
	while(!(RCC->CR & (1<<25))){}
	//Select PLL as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	//check to confirm PLL being used
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL )){}


	lcd.enable_port(GPIOD);
	lcd.enable_port(GPIOC);
	lcd.initialize();
	lcd.clear();

	char info[] = "  complete";
	lcd.newline();
	lcd.send_string(info);

	for(int i = 0; i<10000;i++);

	//Enable TIM5 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;




	//SET PRESCALER
	TIM3->PSC = PSC_VALUE;
	//set ARR VALUE
	TIM3->ARR = ARR_VALUE;
	//Initiate update event
	TIM3->EGR |= TIM_EGR_UG;
	//Enable update interrupt
	TIM3->DIER |= TIM_DIER_UIE;
	//only timer overflow generates update event
	TIM3->CR1 |= TIM_CR1_URS;
	//Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;

	//set NVIC priority
	NVIC_SetPriority(TIM3_IRQn,0x03);
		//enable NVIC
	NVIC_EnableIRQ(TIM3_IRQn);




		lcd.clear();




	while(1){




	}
}
