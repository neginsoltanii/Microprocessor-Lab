
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "keypad.h"

char txt[32];
uint8_t user_count = 0;
uint8_t admin_pass[4] = {1, 2, 3, 4}, admin_input[4];

int main()
{
	DDRB = 0x03;
	PORTB = 0x00;
	DDRC = 0xff;

	KeyPad_init();
	lcd_init(LCD_DISP_ON);

	while (1)
	{

		// _delay_ms(5000);

		lcd_puts_P("ADMIN PASS?!");
		lcd_gotoxy(0, 1);
		for (uint8_t i = 0; i < 4; i++) //get admin pass
		{
			admin_input[i] = Read_keyPad();
			lcd_putc(admin_input[i] + 48);
		}
		while (Read_keyPad() != '=')
			;

		lcd_clrscr();
		if (admin_input[0] == admin_pass[0] && admin_input[1] == admin_pass[1] && //chack admin pass
			admin_input[2] == admin_pass[2] && admin_input[3] == admin_pass[3])
		{
			lcd_puts_P("WELCOM ADMIN");
			PORTB |= 0x01; //green
			PORTB &= ~0x02;

			lcd_clrscr();
			lcd_puts_P("number of users?");
			lcd_gotoxy(0, 1);
			user_count = Read_keyPad();
			lcd_putc(user_count + 48);

			while (Read_keyPad() != '=')
				;

			uint8_t user_pass[user_count][4]; //create user arrays
			uint8_t user_input[user_count][4];
			uint8_t user_id = 0;

			for (uint8_t i = 0; i < user_count; i++) //enter user data by admin
			{
				sprintf(txt, "user %d pass:", i + 1);
				lcd_clrscr();
				lcd_puts(txt);

				lcd_gotoxy(0, 1);
				for (uint8_t j = 0; j < 4; j++)
				{
					user_pass[i][j] = Read_keyPad();
					lcd_putc(user_pass[i][j] + 48);
				}
				while (Read_keyPad() != '=')
					;
			}

			lcd_clrscr(); //get user id from user
			lcd_puts_P("Enter user ID");
			lcd_gotoxy(0, 1);
			user_id = Read_keyPad();
			lcd_putc(user_id + 48);
			user_id--;
			while (Read_keyPad() != '=')
				;

			lcd_clrscr(); //get user pass from user
			lcd_puts_P("user pass??");
			lcd_gotoxy(0, 1);
			for (uint8_t i = 0; i < 4; i++)
			{
				user_input[user_id][i] = Read_keyPad();
				lcd_putc(user_input[user_id][i] + 48);
			}
			while (Read_keyPad() != '=')
				;

			lcd_clrscr();										   //check user pass
			if (user_input[user_id][0] == user_pass[user_id][0] && //chack user pass
				user_input[user_id][1] == user_pass[user_id][1] &&
				user_input[user_id][2] == user_pass[user_id][2] &&
				user_input[user_id][3] == user_pass[user_id][3])
			{
				lcd_puts_P("WELCOM user");
				PORTB |= 0x01; //green
				PORTB &= ~0x02;
			}
			else
			{
				lcd_puts_P("invalid user pass");
				PORTB |= 0x02;
				PORTB &= ~0x01; //red
			}
		}

		else //admin incorrect pass
		{
			lcd_puts_P("ACCESS DENIED");
			PORTB |= 0x02;
			PORTB &= ~0x01; //red
		}

		_delay_ms(50000);
	}
}