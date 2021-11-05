
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"

unsigned char treashold = 35, hysteresis = 1;

ISR(INT0_vect)
{
	_delay_ms(200);
	if (treashold < 150)
		treashold++;
}
ISR(INT1_vect)
{
	_delay_ms(200);
	if (treashold > 0)
		treashold--;
}
ISR(INT2_vect)
{
	_delay_ms(200);
	hysteresis++;
}

void adc_init()
{
	ADCSRA = (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
	SFIOR = (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
}
void interrupt_init()
{
	GICR |= (1 << INT1) | (1 << INT0) | (1 << INT2);
	MCUCR = (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
	MCUCSR = (0 << ISC2);
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
	DDRA = 0;
	DDRD &= ~0x0c;
	PORTD |= 0x0c;
	DDRB &= ~0x04;
	PORTB |= 0x04;
	DDRB |= 0x01;

	adc_init();
	lcd_init(LCD_DISP_ON);
	interrupt_init();

	uint16_t adc = 0;
	int temp = 0;
	char s[16];
	sei();

	while (1)
	{
		lcd_clrscr();
		adc = adc_read(0);
		temp = (int)(adc * 500.0 / 1023);
		sprintf(s, "Temp is= %d'c", temp);
		lcd_puts(s);

		lcd_gotoxy(0, 1);
		sprintf(s, "tr=%d   hy=%d", treashold, hysteresis);
		lcd_puts(s);

		if (temp > treashold)
			PORTB |= 0x01;
		else if (temp < treashold - hysteresis)
			PORTB &= ~0x01;

		_delay_ms(100);
	}
}