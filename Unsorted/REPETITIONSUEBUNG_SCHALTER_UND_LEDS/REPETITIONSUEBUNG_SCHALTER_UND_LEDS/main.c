/*
* REPETITIONSUEBUNG_SCHALTER_UND_LEDS.c
*
* Created: 13/11/2025 10:20:37
* Author : Lars
*/

#include <avr/io.h>

#define PORT_SCHALTER                   (PINC)
#define DDR_SCHALTER                    (DDRC)

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define ALLE_PINS_IN                    (0x00)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_IN_HAUPTSCHALTER          (1<<0)
#define MASKE_IN_SCHALTER_LICHT_UNTEN   (1<<1)
#define MASKE_IN_SCHALTER_LICHT_OBEN    (1<<2)
#define MASKE_IN_NOTSCHALTER            (1<<7)

#define MASKE_OUT_LED_AUS               (0<<0)
#define MASKE_OUT_LED_UNTEN             (0b111111<<0)
#define MASKE_OUT_LED_OBEN              (0b111111<<6)
#define MASKE_OUT_LED_NOTLICHT          (0b11111<<12)

int main(void)
{
    uint8_t inSchalter          = 0;
    uint8_t inHauptschalter     = 0;
    uint8_t inSchalterUnten     = 0;
    uint8_t inSchalterOben      = 0;
    uint8_t inNotschalter       = 0;
    
    uint16_t outLed             = 0;
    uint16_t outLedOben         = 0;
    uint16_t outLedUnten        = 0;
    uint16_t outNotlicht        = 0;
    
    DDR_SCHALTER        =   ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    =   ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   =   ALLE_PINS_OUT;
    
    while (1)
    {
        //Eingabe
        inSchalter      = PORT_SCHALTER;
        inHauptschalter = (inSchalter & MASKE_IN_HAUPTSCHALTER)         > 0;
        inSchalterUnten = (inSchalter & MASKE_IN_SCHALTER_LICHT_UNTEN)  > 0;
        inSchalterOben  = (inSchalter & MASKE_IN_SCHALTER_LICHT_OBEN)   > 0;
        inNotschalter   = (inSchalter & MASKE_IN_NOTSCHALTER)           > 0;
        
        //verarbeitung
        if (inHauptschalter)
        {
            outLedUnten = inSchalterUnten;
            
            outLedOben  = inSchalterOben;
            
        }
        else{
            outLedUnten = 0;
            outLedOben  = 0;
        }
        
        outNotlicht = inNotschalter;
        //Ausgabe
        outLed =    ((outLedUnten   * MASKE_OUT_LED_UNTEN)
                    |(outLedOben    * MASKE_OUT_LED_OBEN)
                    |(outNotlicht   * MASKE_OUT_LED_NOTLICHT));
        PORT_LED_LOW_BYTE   =   outLed;
        PORT_LED_HIGH_BYTE  =   (outLed>>8);
    }
    
}

