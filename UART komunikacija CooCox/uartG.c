#include "uartG.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include <string.h>
#include "gpio.h"


#define TXEN		((uint8_t)3)
#define RXEN		((uint8_t)2)
#define TCIE		((uint8_t)6)
#define RCIE		((uint8_t)5)
#define USART2EN	((uint8_t)13)


#define DATA_SENT  ((uint32_t)(1<<6))
#define DATA_RECEVIED ((uint32_t)(1<<5))




static volatile uint8_t tx_flag = 0;
static volatile uint8_t BufferCounter= 0;
static volatile uint8_t DataRecevied_flag = 0;
static volatile uint8_t DataAnlyzed_flag = 0;
volatile char InputBufferUSART2[MAX_BUFFER_SIZE] = {'\0'};


void InitUart()
{


		RCC -> AHB1ENR |= 1 << 3;

		GPIOD -> MODER |= 0b00000000000000000010100000000000;

		GPIOD -> AFR[0] |= 0x07700000;

		GPIOD -> OSPEEDR |= 0b00000000000000000010100000000000;

		RCC -> APB1ENR |= 1 << 17;

		USART2 -> CR1 = 0x00000000;

		USART2 -> CR1 |= (1 << TXEN | 1 << RXEN | 1<< TCIE | 1 << RCIE);

		USART2 -> BRR |= 0x0683; //Baud rate 9600 @ 16 MHz

		NVIC_EnableIRQ(USART2_IRQn);

		USART2 -> CR1 |= (1 << USART2EN);


}

void SendChar(unsigned char c)
{
	USART2 -> DR = c;
	tx_flag = 1;
	while(tx_flag);

}
void SendString(unsigned char *str)
{
	while(*str != '\0')
	{
		SendChar(*str);
		str++;
	}
}

void ClearUSART2Buffer(void)
{
	BufferCounter = 0;
	InputBufferUSART2[BufferCounter] = '\0';
}

void USART2_IRQHandler()
{
	if(USART2 -> SR & DATA_SENT)
	{
		USART2 -> SR &= ~DATA_SENT;
		tx_flag = 0;
	}
	else if (USART2 -> SR & DATA_RECEVIED)
	{
		USART2 -> SR &= ~DATA_RECEVIED;

		char inputChar;

		inputChar = USART2 ->DR;

		if(inputChar == '\n' || inputChar == '\r')
		{
			DataRecevied_flag = 1;
		}
		else
		{
			InputBufferUSART2[BufferCounter] = inputChar;
			BufferCounter++;
			InputBufferUSART2[BufferCounter] = '\0';
		}

	}

}

void AnalyzeRecivedData()
{
	if(DataRecevied_flag == 1)
	{
		DataRecevied_flag = 0;

		if(strcasecmp((char*)InputBufferUSART2,"LEVO") == 0)
		{
			SendString("Pozdrav sa leve strane :)\n\r");
			gpio_set_pin(GPIOD,GPIO_PIN_N12);
			gpio_clear_pin(GPIOD,GPIO_PIN_N13);
			gpio_clear_pin(GPIOD,GPIO_PIN_N14);
			gpio_clear_pin(GPIOD,GPIO_PIN_N15);
			ClearUSART2Buffer();
		}
		else if (strcasecmp((char*)InputBufferUSART2,"DESNO") == 0)
		{
			SendString("Pozdrav sa desne strane :)\n\r");
			gpio_clear_pin(GPIOD,GPIO_PIN_N12);
			gpio_set_pin(GPIOD,GPIO_PIN_N13);
			gpio_clear_pin(GPIOD,GPIO_PIN_N14);
			gpio_clear_pin(GPIOD,GPIO_PIN_N15);
			ClearUSART2Buffer();
		}
		else if (strcasecmp((char*)InputBufferUSART2,"NAPRED") == 0)
		{
			SendString("Pozdrav unapred :)\n\r");
			gpio_clear_pin(GPIOD,GPIO_PIN_N12);
			gpio_clear_pin(GPIOD,GPIO_PIN_N13);
			gpio_set_pin(GPIOD,GPIO_PIN_N14);
			gpio_clear_pin(GPIOD,GPIO_PIN_N15);
			ClearUSART2Buffer();
		}
		else if (strcasecmp((char*)InputBufferUSART2,"NAZAD") == 0)
		{
			SendString("Pozdrav u rikverc :)\n\r");
			gpio_clear_pin(GPIOD,GPIO_PIN_N12);
			gpio_clear_pin(GPIOD,GPIO_PIN_N13);
			gpio_clear_pin(GPIOD,GPIO_PIN_N14);
			gpio_set_pin(GPIOD,GPIO_PIN_N15);
			ClearUSART2Buffer();
		} else {
			SendString("Ne valja ovo nista :)\n\r");
			ClearUSART2Buffer();
		}
	}

}






