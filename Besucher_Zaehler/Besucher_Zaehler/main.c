/*
 * Besucher_Zaehler.c
 *
 * Created: 17/12/2025 15:28:54
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

#define ALLE_PINS_IN                    (0)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_IN_TASTER_HOCH            (1<<0)
#define MASKE_IN_TASTER_RUNTER          (1<<1)
#define MASKE_IN_TASTER_RESET           (1<<6)

int main(void)
{
    DDR_TASTER          = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint16_t outLed                 = 0;
    uint8_t inTaster_neu            = 0;
    uint8_t inTaster_alt            = 0;

    uint8_t inTasterHoch            = 0;
    uint8_t inTasterRunter          = 0;
    uint8_t inTasterReset           = 0;
    
    uint8_t inPosFlanke             = 0;
    while (1) 
    {
       inTaster_alt = inTaster_neu;
       
       inTaster_neu = PIN_TASTER;
       
       inPosFlanke  = inTaster_neu & (inTaster_alt ^ inTaster_neu);
       
       inTasterHoch     = (inPosFlanke & MASKE_IN_TASTER_HOCH) > 0;
       inTasterRunter   = (inPosFlanke & MASKE_IN_TASTER_RUNTER) > 0;
       inTasterReset    = (inPosFlanke & MASKE_IN_TASTER_RESET) > 0;
       
       if (inTasterHoch)
       {
           outLed += 1;
       }
       if (inTasterRunter && outLed) //outled => überlaufsschutz
       {
           outLed -= 1;
       }
       if (inTasterReset)
       {
           outLed = 0;
       }
       
       PORT_LED_LOW_BYTE   = outLed;
       PORT_LED_HIGH_BYTE  = (outLed>>8);
    }
}

