/*
 * Gueltigkeitsbereich.c
 *
 * Created: 20/11/2025 08:53:35
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define PORT_SCHALTER                   (PINC)
#define DDR_SCHALTER                    (DDRC)

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define ALLE_PINS_IN                    (0x00)
#define ALLE_PINS_OUT                   (0xFF)



#define MASKE_OUT_LED_AUS               (0<<0)
#define MASKE_OUT_LED_UNTEN             (0b11111111<<0)
#define MASKE_OUT_LED_OBEN              (0b11111111<<8)

uint8_t Custom_Variable          = 8;

int main(void)
{
    
    
    uint16_t outLed             = 0;
    uint8_t outLedOben          = 0;
    uint8_t outLedUnten         = 0;
    
    DDR_SCHALTER        =   ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    =   ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   =   ALLE_PINS_OUT;
    
    
    while (1)
    {
  
     for (uint8_t Custom_Variable = 0; Custom_Variable <= 2; Custom_Variable++)
     {
         outLedOben = outLedOben + 1;
     }

     outLedOben |= (Custom_Variable << 4); //Bitmuster 1000 0011
     
     for (Custom_Variable = 0; Custom_Variable <= 2; Custom_Variable++)
     {
     outLedUnten = outLedUnten + 1;
     }

     outLedUnten |= (Custom_Variable << 4); //Bitmuster 0011 0011
        
     PORTB  =   outLedOben;
     while(1)

     PORT_LED_LOW_BYTE   =   outLedUnten;
        
    }
    
}    

