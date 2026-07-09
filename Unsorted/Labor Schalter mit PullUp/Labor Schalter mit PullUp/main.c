/*
 * Labor Schalter mit PullUp.c
 *
 * Created: 17/11/2025 16:25:08
 * Author : Lars
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0xFF;
    DDRB &=~(1<<3);
    
    
    PORTB |= (1<<3);
    
    while (1) 
    {
       uint8_t inB3 = PINB & (1<<3);
       
       if (!inB3)
       {
           PORTA = 0xFF;
       }
       else{
           PORTA = 0;
       }
    }
}

