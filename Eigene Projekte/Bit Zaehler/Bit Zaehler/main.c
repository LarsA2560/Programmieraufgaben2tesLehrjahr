/*
 * Bit Zaehler.c
 *
 * Created: 16/11/2025 20:33:54
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU CPU 16000000UL

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define PORT_TASTER                     (PINL)
#define DDR_TASTER                      (DDRL)

#define TASTER_PINS_IN                  (0)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_BIT_RESET_TASTER          (1)
#define MASKE_BIT_Zeahler               (1<<1)
#define MASKE_RAUF                      (1<<6)
#define MASKE_RUNTER                    (1<<7)

#define NICHTGEPRESST                   (0)
#define GEPRESST                        (1)
#define RAUF                            (2)
#define RUNTER
                   


int main(void)
{
    DDR_TASTER          = TASTER_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint16_t outbit_Position    = 0;
    uint8_t inTaster            = 0;
    uint8_t inReset             = 0;
    uint8_t inZaehler           = 0;
    uint8_t loopPraevention     = NICHTGEPRESST;
    uint8_t inRauf                = 0;
    uint8_t inRunter              = 0;
    
    while (1) 
    {
        inTaster    = PORT_TASTER;
        inReset     = (inTaster & MASKE_BIT_RESET_TASTER) > 0;
        inZaehler   = (inTaster & MASKE_BIT_Zeahler) > 0;
        inRauf      = (inTaster & MASKE_RAUF) > 0;
        inRunter    = (inTaster & MASKE_RUNTER) > 0;       
        
        if (inReset)
        {
            outbit_Position = 0;
        }
         
        
        switch (loopPraevention)
        {
        case NICHTGEPRESST:
        if (inZaehler)
        {
            loopPraevention     = GEPRESST;
            outbit_Position     = outbit_Position + 1;
        }
        
        if (inRauf)
        {
            loopPraevention     = GEPRESST;
            outbit_Position     = outbit_Position << 1;
        }    
        
        if (inRunter)
        {
            loopPraevention     = GEPRESST;
            outbit_Position     = outbit_Position >> 1;
        }            
        break;
        
        case GEPRESST:
        if (!inZaehler & !inRauf & !inRunter)
        {
            loopPraevention = NICHTGEPRESST;
        }
        	break;
        }
        
        
        PORT_LED_LOW_BYTE   = outbit_Position;
        PORT_LED_HIGH_BYTE  = outbit_Position >> 8;
    }
}

