
#include <avr/io.h>

int main(void)
{
    uint8_t inSchalter = 0;
    uint8_t outLed = 0;
    //PORTC auf Eingang
    DDRC = 0;
    //PORTA auf Ausgang setzen
    DDRA = 0xFF;
    
    PORTA = 0xFF;
   
    while (1)
    { 
        outLed=0xFF;
        inSchalter = PINC;
        if (inSchalter & 0b00001000){
            outLed |= 0b10000000;
        }
       else {
            outLed &= 0b01111111 ;            
        }

        if (inSchalter & 0b00000100){
            outLed &= 0b10111111;
        }
        else {
            outLed |= 0b01000000;            
        }
                
        if (inSchalter & 0b00000010){
            outLed |= 0b00000011;
        }
        else {
            outLed &= 0b11111100;            
        }
        PORTA |= outLed;
        PORTA &= outLed;
        
    }
}