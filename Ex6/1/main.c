
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

void adc_init()
{
	ADCSRA = (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
	SFIOR = (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | ch;

	_delay_us(10);
	ADCSRA |= (1 << ADSC);

	while ((ADCSRA & (1 << ADIF)) == 0)
		;
	ADCSRA |= (1 << ADIF);

	return ADCW;
}

int main()
{
	DDRC = 0xff;
	DDRA=0;
	adc_init();
	lcd_init(LCD_DISP_ON);

	uint16_t adc=0;
	int temp=0;
	char s[16];

	while (1)
	{
		lcd_clrscr();
		adc=adc_read(0);
		temp=(int)(adc*500.0/1023);
		sprintf(s,"Temp is= %d'c",temp);
		lcd_puts(s);
		_delay_ms(100);
	}
}