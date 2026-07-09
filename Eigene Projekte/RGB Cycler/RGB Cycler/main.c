/*
 * RGB Cycler.c
 *
 * Created: 12/09/2025 14:45:05
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define RGB_R_Ein PORTH = PORTH | BIT3
#define RGB_G_Ein PORTH = PORTH | BIT4
#define RGB_B_Ein PORTH = PORTH | BIT5
#define RGB_R_Aus PORTH = PORTH & ~ BIT3
#define RGB_G_Aus PORTH = PORTH & ~ BIT4
#define RGB_B_Aus PORTH = PORTH & ~ BIT5
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
uint8_t Zaehler = 255;

int main(void)
{
    DDRA = 0xFF;
    DDRB = 0XFF;
    DDRH = 0x38;
    
    while (1)
    {
        PORTA = Zaehler;
        RGB_R_Ein;
        _delay_ms(500);
        RGB_B_Ein;
        _delay_ms(500);
        RGB_R_Aus;
        _delay_ms(500);
        RGB_G_Ein;
        _delay_ms(500);
        RGB_B_Aus;
        _delay_ms(500);
        RGB_R_Ein;
        _delay_ms(500);
        RGB_B_Ein;
        _delay_ms(500);
        RGB_R_Aus;
        RGB_G_Aus;
        RGB_B_Aus;
        

        
    }
}
