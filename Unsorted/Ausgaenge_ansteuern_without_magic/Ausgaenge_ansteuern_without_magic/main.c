/*
 * Ausgaenge_ansteuern_without_magic.c
 *
 * Created: 13/11/2025 09:47:12
 * Author : Lars
 */ 

#include <avr/io.h>

#define PORT_SCHALTER       (PINC)
#define DDR_SCHALTER        (DDRC)
#define PORT_LED_LOW_BYTE   (PORTA)
#define DDR_LED_LOW_BYTE    (DDRA)
#define ALLE_PINS_OUT       (0x00)
#define ALLE_PINS_IN        (0xFF)

#define MASKE_IN_SCHALTER1  (1<<1)
#define MASKE_IN_SCHALTER2  (1<<2)
#define MASKE_IN_SCHALTER3  (1<<3)

#define MASKE_OUT_LED0_1    (3<<0)
#define MASKE_OUT_LED6      (1<<6)
#define MASKE_OUT_LED7      (1<<7)

int main(void)
{
    uint8_t inSchalter  =   0;
    uint8_t outLed      =   0;
    DDR_SCHALTER        =   ALLE_PINS_OUT;
    DDR_LED_LOW_BYTE    =   ALLE_PINS_IN;
    
    PORT_LED_LOW_BYTE   =   0xFF;
    outLed=0xFF;
    while (1)
    {
        inSchalter      =   PORT_SCHALTER;
        if (inSchalter  &   MASKE_IN_SCHALTER3){
            outLed      |=  MASKE_OUT_LED7;
        }
        else {
            outLed      &= ~MASKE_OUT_LED7 ;
        }

        if (inSchalter  &   MASKE_IN_SCHALTER2){
            outLed      &= ~MASKE_OUT_LED6;
        }
        else {
            outLed      |=  MASKE_OUT_LED6;
        }
        
        if (inSchalter  &   MASKE_IN_SCHALTER1){
            outLed      |=  MASKE_OUT_LED0_1;
        }
        else {
            outLed      &= ~MASKE_OUT_LED0_1;
        }
    PORT_LED_LOW_BYTE   =   outLed;

        
    }
}