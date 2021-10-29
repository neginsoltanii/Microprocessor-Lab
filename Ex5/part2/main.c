
#include <avr/io.h>     /* Include AVR std. library file */
#include <util/delay.h> /* Include delay header file */
#include <avr/interrupt.h>
//#define F_CPU 8000000L

char i = 0, dir = 0;
unsigned char stepper[4] = {0x09, 0x0c, 0x06, 0x03};

ISR(TIMER0_OVF_vect)
{

    if (dir)
    {
        i++;
        if (i > 3)
            i = 0;
    }
    else
    {
        i--;
        if (i < 0)
            i = 3;
    }
    PORTA = stepper[i];
    TCNT0 = 255 - 39 + 1;
}

int main(void)
{
    DDRA = 0x0f;
    PORTA = 0x00;
    DDRC &= ~(0x04);
    PORTC |= 0x04;

    TCCR0 = 0x05;
    TCNT0 = 255 - 39 + 1;
    TIMSK = 0x01;
    sei();
    while (1)
    {
        if ((PINC & 0x04) == 0)
        {
            _delay_ms(250);
            dir = !dir;
        }
    }
}