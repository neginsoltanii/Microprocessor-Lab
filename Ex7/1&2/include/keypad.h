#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

#define KEY_DDR DDRA
#define KEY_PIN PINA
#define KEY_PORT PORTA

void KeyPad_init(void);
uint8_t Read_keyPad(void);

#endif