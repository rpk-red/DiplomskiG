
#include "uartG.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include "core_cm4.h"

int main(void)
{
	GPIO_Struct GPIOD_Init_LED;

	GPIOD_Init_LED.PinNo = (GPIO_PIN_N12 | GPIO_PIN_N13 | GPIO_PIN_N14 | GPIO_PIN_N15);
	GPIOD_Init_LED.OutputType = GPIO_OMODE_PP;
	GPIOD_Init_LED.PinMode = GPIO_PIN_MODE_OUTPUT;
	GPIOD_Init_LED.PinSpeed = GPIO_PIN_SPEED_HIGH;
	GPIOD_Init_LED.PullType = GPIO_PULL_NONE;

	gpio_d_clk_enable();
	gpio_init_port(GPIOD, &GPIOD_Init_LED);

//	RCC -> AHB1ENR |= (1<<3);
//
//	GPIOD -> MODER |= (1<<24);
//
//	GPIOD -> OSPEEDR |= (1<<24);
	gpio_set_pin(GPIOD,GPIO_PIN_N12);
	gpio_set_pin(GPIOD,GPIO_PIN_N13);
	gpio_set_pin(GPIOD,GPIO_PIN_N14);
	gpio_set_pin(GPIOD,GPIO_PIN_N15);
	InitUart();

	uint32_t i = 500000;
	SendString("Pozdrav:)\n\r");

    while(1)
    {
    	while(i)
    		i--;
    	i = 500000;
    	gpio_toggle_pin(GPIOD, GPIO_PIN_N15);
    	AnalyzeRecivedData();
    }
}
