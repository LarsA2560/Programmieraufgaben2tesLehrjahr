/*
 * Schalterzaehler.c
 *
 * Created: 12/11/2025 16:18:50
 * Author : Lars
 */ 

#include <avr/io.h>


int main(void)
{
    uint8_t inSchalter = 0;
    uint8_t outLed = 0;
    uint8_t cycler = 0;
    //PORTC auf Eingang
    DDRC = 0;
    //PORTA auf Ausgang setzen
    DDRA = 0xFF;
    
    
    /* Replace with your application code */
    while (1)
    {
        // EINGABE
        //Schalter an PortC einlesen und in inSchalter speichern
        inSchalter = PINC;
        outLed = 0;
        // VERARBEITUNG
        for (cycler = 0; cycler<8; cycler++)
        {
            if (inSchalter % 2)
            {
                outLed = outLed + 1;
                
            }
           inSchalter = inSchalter >> 1;
            
        }
        
        
        
        // AUSGABE
        PORTA = outLed;
        
    }
}