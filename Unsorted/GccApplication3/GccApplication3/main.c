/*
 * GccApplication3.c
 *
 * Created: 20/11/2025 15:35:22
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL

#define PORT_LED            (PORTA)
#define DDR_LED             (DDRA)

#define PORT_SCHALTER       (PINC)
#define DDR_SCHALTER        (DDRC)

#define ALLE_PINS_IN        (0x00)
#define ALLE_PINS_OUT       (0xFF)

#define MASKE_IN_SCHALTER_0  (1<<0)
#define MASKE_IN_SCHALTER_1  (1<<1)
#define MASKE_IN_SCHALTER_2  (1<<2)
#define MASKE_IN_SCHALTER_3  (1<<3)
#define MASKE_IN_SCHALTER_4  (1<<4)
#define MASKE_IN_SCHALTER_5  (1<<5)
#define MASKE_IN_SCHALTER_6  (1<<6)
#define MASKE_IN_SCHALTER_7  (1<<7)

#define MASKE_OUT_LED_0      (1)
#define MASKE_OUT_LED_1      (1<<1)
#define MASKE_OUT_LED_2      (1<<2)
#define MASKE_OUT_LED_3      (1<<3)
#define MASKE_OUT_LED_4      (1<<4)
#define MASKE_OUT_LED_5      (1<<5)
#define MASKE_OUT_LED_6      (1<<6)
#define MASKE_OUT_LED_7      (1<<7)

int main(void)
{
    
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED             = ALLE_PINS_OUT;
    
    uint8_t inschalter      = 0;    
    uint16_t outLed         = 0;
    
    uint8_t inSchalter_0    = 0;
    uint8_t inSchalter_1    = 0;
    uint8_t inSchalter_2    = 0;
    uint8_t inSchalter_3    = 0;
    uint8_t inSchalter_4    = 0;
    uint8_t inSchalter_5    = 0;
    uint8_t inSchalter_6    = 0;
    uint8_t inSchalter_7    = 0;
    uint8_t inSchalter_0_7  = 0;
    uint8_t inSchalter_1_3  = 0;
    uint8_t inschalter_4_6  = 0;
     
    while (1) 
    {
        uint8_t outInverter = 0;
        
        uint8_t outLed_0    = 0;
        uint8_t outLed_1    = 0;
        uint8_t outLed_2    = 0;
        uint8_t outLed_3    = 0;
        uint8_t outLed_4    = 0;
        uint8_t outLed_5    = 0;
        uint8_t outLed_6    = 0;
        uint8_t outLed_7    = 0;
        
        outLed = 0;
        
        //Eingabe
        inschalter      = PORT_SCHALTER;
        inSchalter_0    = (inschalter & MASKE_IN_SCHALTER_0) > 0;
        inSchalter_1    = (inschalter & MASKE_IN_SCHALTER_1) > 0;
        inSchalter_2    = (inschalter & MASKE_IN_SCHALTER_2) > 0;
        inSchalter_3    = (inschalter & MASKE_IN_SCHALTER_3) > 0;
        inSchalter_4    = (inschalter & MASKE_IN_SCHALTER_4) > 0;
        inSchalter_5    = (inschalter & MASKE_IN_SCHALTER_5) > 0;
        inSchalter_6    = (inschalter & MASKE_IN_SCHALTER_6) > 0;
        inSchalter_7    = (inschalter & MASKE_IN_SCHALTER_7) > 0;
        inSchalter_0_7  = inschalter;
        inSchalter_1_3  = (inSchalter_3<<2) | (inSchalter_2<<1) | inSchalter_1;
        inschalter_4_6  = (inSchalter_6<<2) | (inSchalter_5<<1) | inSchalter_4;
                
        //Verarbeitung
        if (inSchalter_7)
        {
            outLed_0 = 1;
        }

        if (!inSchalter_5 & inSchalter_6 & inSchalter_7)
        {
            outLed_1 = 1;
        }        
        
        if (!inSchalter_4 | !inSchalter_5 | !inSchalter_6)
        {
            outLed_2 = 1;
        }
        if (inSchalter_1)
        {
            outLed_3 = 1;
        }
        else
        {
            outLed_4 = 1;
        }
        
        inSchalter_0_7 = (inSchalter_0_7 % 17) == 0;
        if (inSchalter_0_7)
        {
            outLed_5 = 1;
        }
        
        if (inSchalter_1_3 > inschalter_4_6)
        {
            outLed_6 = 1;
        }
        
        if (inSchalter_1_3 == inschalter_4_6)
        {
            outLed_7 = 1;
        }
        
        if (inSchalter_0)
        {
            outInverter = 1;
        }
        
        //Ausgabe
        outLed      =   ((outLed_0 * MASKE_OUT_LED_0)
                        |(outLed_1 * MASKE_OUT_LED_1)
                        |(outLed_2 * MASKE_OUT_LED_2)
                        |(outLed_3 * MASKE_OUT_LED_3)
                        |(outLed_4 * MASKE_OUT_LED_4)
                        |(outLed_5 * MASKE_OUT_LED_5)
                        |(outLed_6 * MASKE_OUT_LED_6)
                        |(outLed_7 * MASKE_OUT_LED_7));
                      
        outLed      =   ((~outLed) * outInverter)
                        |(outLed * !outInverter);
        PORT_LED    = outLed;
    }
}

