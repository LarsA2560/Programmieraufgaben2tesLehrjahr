/*
 * Schalter eingabe.c
 *
 * Created: 12/11/2025 13:46:13
 * Author : Lars
 */ 

#include <avr/io.h>


int main(void)
{
    uint8_t inSchalter = 0;
    uint8_t outLed = 0;
    //PORTC auf Eingang
    DDRC = 0;
    //PORTA auf Ausgang setzen
    DDRA = 0xFF;
    DDRB = 0xFF;
    
    /* Replace with your application code */
    while (1)
    {
        // EINGABE
        //Schalter an PortC einlesen und in inSchalter speichern
        inSchalter = PINC;
        // VERARBEITUNG
        if (inSchalter & 0b00001000){
             outLed = 0b11001010;
        }            
         else{
             outLed = 0x00;
         }
        // AUSGABE
        PORTA = outLed;
        
        
    }
}
