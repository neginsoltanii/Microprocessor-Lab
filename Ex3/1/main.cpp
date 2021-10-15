#define F_CPU 1500000UL
#include <avr/io.h>
#include <util/atomic.h>

int count = 0;
int dly = 0;

ISR(TIMER1_OVF_vect){
    dly++;
    if(dly > 12){
        if(count == 10){
            count = 0;
        }
    switch(count){
        case 0:
            PORTD = 0b0111111;
            break;
        case 1:
            PORTD = 0b0000110;
            break;
        case 2:
            PORTD = 0b1011011;
            break;
        case 3:
            PORTD = 0b1001111;
            break;
        case 4:
            PORTD = 0b1100110;
            break;
        case 5:
            PORTD = 0b1101101;
            break;
        case 6:
            PORTD = 0b1111101;
            break;
        case 7:
            PORTD = 0b0000111;
            break;
        case 8:
            PORTD = 0b1111111;
            break;
        case 9:
            PORTD = 0b1101111;
            break;
        default:
            PORTD = 0b0000000;
    }
    dly = 0;
    }
        
}


int main() 
{
    TIMSK = 0b00000100;
    TCCR1B = 0b0000010;
    DDRD = 0b11111111;
    DDRC = 0b11111111;
    sei();
    PORTC = 0;
    PORTD = 0;

    while (1){}
    return 0;
}