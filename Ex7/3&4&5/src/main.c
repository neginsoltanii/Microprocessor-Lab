
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void UART_init(long USART_BAUDRATE)
{
	UCSRA = (0 << RXC) | (0 << TXC) | (0 << UDRE) | (0 << FE) | (0 << DOR) | (0 << PE) | (0 << U2X) | (0 << MPCM);
	UCSRB = (0 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN) | (1 << TXEN) | (0 << UCSZ2) | (0 << RXB8) | (0 << TXB8);
	UCSRC = (1 << URSEL) | (0 << UMSEL) | (0 << UPM1) | (0 << UPM0) | (0 << USBS) | (1 << UCSZ1) | (1 << UCSZ0) | (0 << UCPOL);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0)
		;
	return (UDR);
}

void UART_TxChar(char ch)
{
	while (!(UCSRA & (1 << UDRE)))
		;
	UDR = ch;
}

void UART_SendString(char *str)
{
	unsigned char i = 0;

	while (str[i] != 0)
	{
		UART_TxChar(str[i]);
		i++;
	}
}
void UART_GetString(char *str, uint8_t count)
{
	char i = 0;
	char data = 0;
	for (i = 0; i < count; i++)
	{
		data = UART_RxChar();
		if (data != '\r' && data != '\n')
		{
			*str = data;
			str++;
		}
		else
		{
			*str = '\0';
			return;
		}
		*str = '\0';
	}
}

char command[10];
unsigned int nOfRotates = 0;
unsigned char stepper[4] = {0x03, 0x6, 0x0c, 0x09};

int main()
{
	DDRA = 0x0f;
	PORTA = 0x00;
	UART_init(9600);

	while (1)
	{
		UART_GetString(command, 10);
		nOfRotates = atoi(command);

		for (unsigned int i = 0; i < nOfRotates; i++)
		{
			for (unsigned char j = 0; j < 4; j++)
			{
				PORTA = stepper[j];
				_delay_ms(50);
			}
		}
	}
}