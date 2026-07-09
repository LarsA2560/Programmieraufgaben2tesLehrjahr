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
#define MASKE_IN_SCHALTER_ALLE_AN       (1<<0)

#define MASKE_OUT_LED_ALLE_AN           (0xFF)

int main(void)
{
    DDR_TASTER          = ALLE_PINS_IN;
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint16_t outLed                 = 0;
    uint8_t inTaster                = 0;    
    uint8_t inSchalter              = 0;
    
    uint8_t inTaster_alle_an        = 0;
    uint8_t inSchalter_alle_an      = 0;
    uint8_t outLed_alle_an          = 0;
    
    uint8_t toggle_Tasterdruck      = 0;
    
    while (1) 
    {
        //Eingabe
        
        
        inTaster            = PIN_TASTER;
        inSchalter          = PIN_SCHALTER;
        inTaster_alle_an    = (inTaster & MASKE_IN_TASTER_ALLE_AN) > 0;
        inSchalter_alle_an  = (inSchalter & MASKE_IN_SCHALTER_ALLE_AN) > 0;  
        //Verarbeitung
        
        if ((inTaster_alle_an|inSchalter_alle_an) && !toggle_Tasterdruck)
        {
            outLed_alle_an = !outLed_alle_an;
            toggle_Tasterdruck = 1;
        }
        if (!(inTaster_alle_an|inSchalter_alle_an))
        {
            toggle_Tasterdruck = 0;
        }

        //Ausgabe
        outLed = (outLed_alle_an *  MASKE_OUT_LED_ALLE_AN);
        
        PORT_LED_LOW_BYTE   = outLed;
        PORT_LED_HIGH_BYTE  = outLed;     
    }
}

