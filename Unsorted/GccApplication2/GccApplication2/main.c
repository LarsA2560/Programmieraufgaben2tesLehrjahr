/*
 * GccApplication2.c
 *
 * Created: 10/11/2025 16:01:24
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    DDRB|=(1<<0);
    /* Replace with your application code */
    while (1) 
    {
        PORTB |= (1<<0);
        _delay_ms(500);
        PORTB &=~ (1<<0);
        _delay_ms(500); 
    }
}

