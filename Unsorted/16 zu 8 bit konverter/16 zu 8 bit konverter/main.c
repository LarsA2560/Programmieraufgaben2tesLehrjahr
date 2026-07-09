/*
 * 16 zu 8 bit konverter.c
 *
 * Created: 12/11/2025 13:38:06
 * Author : Lars
 */ 

#include <avr/io.h>


int main(void)
{
    uint16_t mein16BitWert = 20001;
    DDRA = 0xFF;
    DDRB = 0xFF;
    
    PORTA = mein16BitWert;
    PORTB = mein16BitWert >> 8;
    /* Replace with your application code */
    while (1) 
    {
    }
}

