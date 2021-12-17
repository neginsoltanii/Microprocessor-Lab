#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

#define SENS_CENTER ((PINB & 0X01) >> 0)
#define SENS_RIGHT ((PINB & 0X02) >> 1)
#define SENS_LEFT ((PINB & 0X04) >> 2)

unsigned char stepper[4] = {0x03, 0x6, 0x0c, 0x09};
char i = 0, j = 0; // motor counter

int main()
{
	DDRB &= 0xf8;
	PORTB |= 0x07;
	DDRD = 0xff;

	lcd_init(LCD_DISP_ON);

	while (1)
	{
		if (SENS_CENTER == 0)
		{
			if (SENS_RIGHT == 1 && SENS_LEFT == 1)
			{
				lcd_clrscr();
				lcd_puts_P("Line Follow...");
				PORTD = stepper[i] | (stepper[j] << 4);
				_delay_ms(50);
			}
			else if (SENS_RIGHT == 0 && SENS_LEFT == 1)
			{
				lcd_clrscr();
				lcd_puts_P("A BIT LEFT.");
				PORTD = (stepper[i]);
				_delay_ms(150);
			}
			else if (SENS_RIGHT == 1 && SENS_LEFT == 0)
			{
				lcd_clrscr();
				lcd_puts_P("A BIT RIGHT.");
				PORTD = (stepper[j] << 4);
				_delay_ms(150);
			}
		}
		if (SENS_CENTER == 1 && SENS_RIGHT == 0)
		{
			lcd_clrscr();
			lcd_puts_P("TURNED LEFT.");
			PORTD = (stepper[i]);
			_delay_ms(50);
		}
		if (SENS_CENTER == 1 && SENS_LEFT == 0)
		{
			lcd_clrscr();
			lcd_puts_P("TURNED RIGHT.");
			PORTD = (stepper[j] << 4);
			_delay_ms(50);
		}

		i++;
		j++;
		if (i == 4)
			i = 0;
		if (j == 4)
			j = 0;
	}
}