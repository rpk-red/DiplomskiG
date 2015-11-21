#include "gpio.h"

/* Initiates port pins. Returns 0 if OK, -1 if faulty input. */
void gpio_init_port(GPIO_TypeDef *Target_GPIO, GPIO_Struct *GPIO_Setup)
{
	uint8_t i = 0;
	uint32_t reg = 0;

	/* Faulty input protection. */
	if(GPIO_Setup->PinMode <= 3 && GPIO_Setup->PinSpeed <= 3 && GPIO_Setup->PullType <= 2) {
		/* Loop to check which pins are selected. */
		for( i = 0; i < GPIO_MAX_PINS; i++) {
			if( (1 << i) & GPIO_Setup->PinNo ) {
				/* Set pins modes. */
				reg = Target_GPIO->MODER;					/* Memorize the values. */
				reg &= ~(3 << i*2);							/* Reset the pins to be changed. */
				reg |= (GPIO_Setup->PinMode << i*2);		/* Write new pin values to variable. */
				Target_GPIO->MODER = reg;					/* Write new pin values to port. */

				/* If pin is set as output set it's type. */
				if( GPIO_Setup->PinMode == GPIO_PIN_MODE_OUTPUT ) {
					Target_GPIO->OTYPER |= GPIO_Setup->OutputType << i;
				}

				/* Set pin pull types. */
				reg = Target_GPIO->PUPDR;					/* Memorize the values. */
				reg &= ~(3 << i*2);							/* Reset the pins to be changed. */
				reg |= (GPIO_Setup->PullType << i*2);		/* Write new pin values to variable. */
				Target_GPIO->PUPDR = reg;					/* Write new pin values to port. */

				/* Set pins speed. Depends on Vdd. */
				reg = Target_GPIO->OSPEEDR;					/* Memorize the values. */
				reg &= ~(3 << i*2);							/* Reset the pins to be changed. */
				reg |= (GPIO_Setup->PinSpeed << i*2);		/* Write new pin values to variable. */
				Target_GPIO->OSPEEDR = reg;					/* Write new pin values to port. */
			}
		}
	}
}

void gpio_set_pin(GPIO_TypeDef *Port, uint16_t pin)
{
	Port->BSRRL |= pin;
}

void gpio_clear_pin(GPIO_TypeDef *Port, uint16_t pin)
{
	Port->BSRRH |= pin;
}

void gpio_toggle_pin(GPIO_TypeDef* Target_GPIO, uint16_t targetPin)
{
	uint32_t reg = 0;
	reg = Target_GPIO->ODR;

	/* Faulty input protection. */
	if( targetPin <= (1 << (GPIO_MAX_PINS -1)) ) {
		if( (reg & (uint32_t)targetPin) != 0 ) {
			Target_GPIO->BSRRH |= targetPin;
		} else {
			Target_GPIO->BSRRL |= targetPin;
		}
	}
}

/* Reads pin state. Returns GPIO_PIN_STATE_LOW/HIGH upon successful read, 2 by default or -1 if faulty input. */
int gpio_read_pin(GPIO_TypeDef* Target_GPIO, uint16_t targetPin)
{
	uint32_t reg = 0;
	reg = Target_GPIO->IDR;

	/* Faulty input protection. */
	if( targetPin <= (1 << (GPIO_MAX_PINS - 1)) ) {
		if( (reg & targetPin) != 0 ) {
			return (int)GPIO_PIN_STATE_HIGH;
		} else {
			return (int)GPIO_PIN_STATE_LOW;
		}
	} else {
		return (int)-1;
	}
}


void gpio_d_clk_enable(void)
{
	RCC->AHB1ENR |= GPIO_CLKEN_D;
}

void gpio_a_clk_enable(void)
{
	RCC->AHB1ENR |= GPIO_CLKEN_A;
}

void gpio_b_clk_enable(void)
{
	RCC->AHB1ENR |= GPIO_CLKEN_B;
}
