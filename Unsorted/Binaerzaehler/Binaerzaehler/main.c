/*
 * Binaerzaehler.c
 *
 * Created: 12/09/2025 15:00:45
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
uint8_t RunnerbitA = 0;
uint8_t RunnerbitB = 0;
uint8_t Sekunden = 0;
uint16_t Minutentief = 0;
uint16_t Minutenhoch = 0;
uint8_t Stunde = 0;
int main(void)
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    while (1)
    {   
        PORTA = Sekunden|Minutentief|RunnerbitA;
        PORTB = Minutenhoch|Stunde|RunnerbitB;

        _delay_ms(1000);
        
        Sekunden = Sekunden+1;
        if ( Sekunden == 60) {
            Minutentief = Minutentief+64;
            Sekunden = 0;

        }
        if (Minutentief == 256 )
        {
        Minutentief = 0;
        Minutenhoch = Minutenhoch+1;
        }
        if (Minutenhoch == 16)
        {
        Stunde = Stunde + 16;
        Minutenhoch = 0;
        }
    }
}