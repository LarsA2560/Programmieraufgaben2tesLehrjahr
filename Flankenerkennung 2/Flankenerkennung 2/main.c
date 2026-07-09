/*
 * Flankenerkennung 1.c
 *
 * Created: 17/12/2025 13:42:43
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define PIN_TASTER                      (PINL)
#define DDR_TASTER                      (DDRL)

#define PIN_SCHALTER                    (PINC)
#define DDR_SCHALTER                    (DDRC)

#define ALLE_PINS_IN                    (0)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_IN_TASTER_ALLE_AN         (1<<0)
#define MASKE_IN_TASTER_ALLE_AUS        (1<<1)

#define MASKE_OUT_LED_ALLE_AN           (0xFF)

int main(void)
{
    DDR_TASTER          = ALLE_PINS_IN;
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint16_t outLed                 = 0;
    uint8_t inTaster_neu            = 0;
    uint8_t inTaster_alt            = 0;

    
    uint8_t inPower_an              = 0;
    uint8_t inPower_aus             = 0;
    uint8_t outLed_alle_an          = 0;
    
    uint8_t inFlanken               = 0;
    uint8_t inPosFlank              = 0;
    uint8_t inNegFlank              = 0;
    
    uint8_t toggle_Tasterdruck      = 0;
    
    while (1) 
    {
        //Eingabe
        inTaster_alt = inTaster_neu;
        inTaster_neu        = PIN_TASTER;
        inFlanken           = (inTaster_alt ^ inTaster_neu);
        inPosFlank          = inTaster_neu & inFlanken;
        inNegFlank          = inTaster_alt & inFlanken;
        inPower_an          = (inPosFlank & MASKE_IN_TASTER_ALLE_AN) > 0;
        inPower_aus         = (inNegFlank & MASKE_IN_TASTER_ALLE_AUS) > 0;  
        //Verarbeitung
        
        if (inPower_an)
        {
            outLed_alle_an = inPower_an;
        }
        if (inPower_aus)
        {
            outLed_alle_an = 0;
        }

        //Ausgabe
        outLed = (outLed_alle_an *  MASKE_OUT_LED_ALLE_AN);
        
        PORT_LED_LOW_BYTE   = outLed;
        PORT_LED_HIGH_BYTE  = outLed;     
    }
}

