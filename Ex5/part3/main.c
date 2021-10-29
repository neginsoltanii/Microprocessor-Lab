
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#define F_CPU 8000000L
#define MAX_DELAY 256
#define MIN_DELAY 39
#define SPEED_STEP 1

char i = 0, dir = 1, speed = 1;
int delay = MAX_DELAY;
char stop = 1;
unsigned char stepper[4] = {0x09, 0x0c, 0x06, 0x03};

ISR(TIMER0_OVF_vect)
{

    if (dir)
    {
        i++;
        if (i > 3)
            i = 0;

        if (speed)
        {
            delay -= SPEED_STEP;
            if (delay <= MIN_DELAY)
            {
                speed = 0;
            }
        }
        else
        {
            delay += SPEED_STEP;
            if (delay >= MAX_DELAY)
            {
                dir = 0;
                speed = 1;
            }
        }
    }
    else
    {
        i--;
        if (i < 0)
            i = 3;

        if (speed)
        {
            delay -= SPEED_STEP;
            if (delay <= MIN_DELAY)
            {
                speed = 0;
            }
        }
        else
        {
            delay += SPEED_STEP;
            if (delay >= MAX_DELAY)
            {
                dir = 1;
                speed = 1;
            }
        }
    }
    PORTA = stepper[i];
    TCNT0 = 255 - (delay + 1);
}

int main(void)
{
    DDRA = 0x0f;
    PORTA = 0x00;
    DDRC &= ~(0x04);
    PORTC |= 0x04;

    TCNT0 = 255 - delay + 1;
    TIMSK = 0x01;
    sei();
    while (1)
    {
        if ((PINC & 0x04) == 0)
        {
            _delay_ms(250);
            stop = !stop;
        }
        if (stop)
        {
            TCCR0 = 0;
        }
        else
        {
            TCCR0 = 0x05;
        }
    }
}