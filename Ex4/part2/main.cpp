#ifndef F_CPU
# define F_CPU 1000000UL 
#endif
#include<avr/io.h>        
#include<util/delay.h>    
#include <util/atomic.h>
#include <stdio.h>
#include "lcd.h"
#define LCD_DATA PORTB         
#define ctrl PORTD              
#define en PD7                  
#define rw PD6                  
#define rs PD5                  

int tenth , sec , min , hour = 0 ;
char St[20];


ISR(TIMER2_COMP_vect){
	tenth ++;
	if(tenth == 10){
		tenth = 0;
		sec++;
		if(sec == 60){
			sec = 0;
			min++;
			if(min == 60){
				min = 0;
				hour++;
				if(hour = 24){
					hour = 0;
				}
			}
		}
		sprintf(St , "%02d:%02d:%02d" , hour , min , sec);
		LCD_cmd(0x80);
		LCD_Write_String(St);
	}
}

int main(void)
{

	DDRB=0xFF;              
	DDRD=0xE0;              
	init_LCD();             
	_delay_ms(10);         
	LCD_cmd(0x0C);          
	_delay_ms(10);
	TIMSK = 0x80;
	TCCR2 = 0x0F;
	OCR2 = 0x60;
	sei();
	while (1)
    {
    }
	return 0;
}



