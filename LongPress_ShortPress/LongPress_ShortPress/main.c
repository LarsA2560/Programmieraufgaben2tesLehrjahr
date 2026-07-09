/*
 * LongPress_ShortPress.c
 *
 * Created: 17/12/2025 16:17:29
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define  SYSTEMTAKT_ms                  (10)

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define PIN_TASTER                      (PINL)
#define DDR_TASTER                      (DDRL)

#define ALLE_PINS_IN                    (0)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_IN_TASTER                 (1<<0)
#define MASKE_IN_TASTER_AN              (1<<0)


int main(void)
{
    DDR_TASTER          = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint8_t systemZeit_ms   = 0;
    uint8_t now             = 0;
    uint8_t start_time      = 0;
    
    uint16_t outLed                 = 0;
    uint8_t inTaster_neu            = 0;
    uint8_t inTaster_alt            = 0;
    uint8_t inTasterAn              = 0;

    uint8_t inFlanke                = 0;
    uint8_t inPosFlanke             = 0;
    uint8_t inNegFlanke             = 0;
    
    while (1) 
    {
       inTaster_alt = inTaster_neu;
       
       inTaster_neu = PIN_TASTER;
       inFlanke     = (inTaster_alt ^ inTaster_neu);
       inPosFlanke  = inFlanke & inTaster_neu;
       inNegFlanke  = inFlanke & inTaster_alt;
       inTasterAn   = (inPosFlanke & MASKE_IN_TASTER_AN) > 0;  
            
       
       now  = systemZeit_ms;
     
       if (inPosFlanke)
       {
       }
       
       PORT_LED_LOW_BYTE = outLed;
       PORT_LED_HIGH_BYTE = outLed<<8;
       
        _delay_ms(SYSTEMTAKT_ms);
        systemZeit_ms += SYSTEMTAKT_ms;
    }
}

