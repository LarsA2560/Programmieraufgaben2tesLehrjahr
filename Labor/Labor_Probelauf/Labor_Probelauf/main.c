/*
 * Labor_Probelauf.c
 *
 * Created: 01/12/2025 14:24:19
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL

#define PORT_BUERO        (PORTA)
#define DDR_BUERO         (DDRA)

#define PORT_AUSSEN     (PORTB)
#define DDR_AUSSEN      (DDRB)
#define PIN_AUSSEN      (PINB)

#define PORT_SCHALTER   (PINC)
#define DDR_SCHALTER    (DDRC)

#define ALLE_PINS_IN        (0x00)
#define ALLE_PINS_OUT       (0xFF)
#define PINBELEGUNG_AUSSEN  (0b11011<<3)

#define MASKE_IN_EINTRETEN_BUERO    (1<<0)
#define MASKE_IN_KLINGEL_AUSSEN     (1<<0)
#define MASKE_IN_MAIL               (1<<6)
#define MASKE_IN_TELEFON            (1<<7)

#define MASKE_OUT_LED_EINTRETEN_BUERO       (1<<0)
#define MASKE_OUT_LED_NICHT_EINTRETEN_BUERO (1<<1)
#define MASKE_OUT_LED_EINTRETEN_AUSSEN      (1<<14)
#define MASKE_OUT_LED_WARTEN                (1<<15)
#define MASKE_OUT_LED_MAIL_BUERO            (1<<5)
#define MASKE_OUT_LED_MAIL_AUSSEN           (1<<11)
#define MASKE_OUT_LED_TELEFON_BUERO         (1<<4)
#define MASKE_OUT_LED_TELEFON_AUSSEN        (1<<10)
#define MASKE_OUT_LED_KLINGEL_BUERO         (1<<7)


int main(void)
{
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_BUERO           = ALLE_PINS_OUT;
    DDR_AUSSEN          = PINBELEGUNG_AUSSEN;
    PORT_AUSSEN |= (1<<0);
    
    uint8_t inSchalter                  = 0;
    uint8_t inSchalterAussen            = 0;
    uint16_t outLed                      = 0;
    
    uint8_t inSchalterEintretenBuero    = 0;    
    uint8_t inSchalterKlingelAussen     = 0;
    uint8_t inSchalterMail              = 0;
    uint8_t inSchalterTelefon           = 0;
    
    uint8_t outLedEintretenBuero        = 0;
    uint8_t outLedNichtEintretenBuero   = 0;
    int8_t outLedMailBuero              = 0;
    int8_t outLedTelefonBuero           = 0;
    int8_t outLedKlingelBuero           = 0;
    
    uint8_t outLedEintretenAussen       = 0;
    uint8_t outLedWartenAussen          = 0;  
    uint8_t outLedMailAussen            = 0; 
    uint8_t outLedTelefonAussen         = 0; 
    
    while (1) 
    {
        
        //Eingabe
        inSchalter          = PORT_SCHALTER;
        inSchalterAussen    = PIN_AUSSEN;
        
        inSchalterEintretenBuero    = (inSchalter           & MASKE_IN_EINTRETEN_BUERO)     > 0;
        inSchalterKlingelAussen     = (inSchalterAussen     & MASKE_IN_KLINGEL_AUSSEN)      > 0;
        inSchalterMail              = (inSchalter           & MASKE_IN_MAIL)                > 0;
        inSchalterTelefon           = (inSchalter           & MASKE_IN_TELEFON)             > 0;
        //Verarbeitung
        
        outLedEintretenBuero        = inSchalterEintretenBuero;
        outLedNichtEintretenBuero   = !inSchalterEintretenBuero;
        
        outLedMailBuero             = inSchalterMail;
        outLedTelefonBuero          = inSchalterTelefon;   
        
             
        outLedKlingelBuero          = inSchalterKlingelAussen;
        if (inSchalterKlingelAussen)
        {   
            outLedEintretenAussen       = !(inSchalterEintretenBuero);
            outLedWartenAussen          = !(!inSchalterEintretenBuero && !inSchalterMail && !inSchalterTelefon);
            

            
            outLedMailAussen            = !inSchalterMail;
            outLedTelefonAussen         = !inSchalterTelefon;
        }
        else
        {
            outLedEintretenAussen   = 1;
            outLedWartenAussen      = 1;
            
            outLedMailAussen        = 1;
            outLedTelefonAussen     = 1;
            
        }
        
        
        //Ausgabe
        outLed      =   ((outLedEintretenBuero      * MASKE_OUT_LED_EINTRETEN_BUERO)        |
                        (outLedNichtEintretenBuero  * MASKE_OUT_LED_NICHT_EINTRETEN_BUERO)  |
                        (outLedEintretenAussen      * MASKE_OUT_LED_EINTRETEN_AUSSEN)       |
                        (outLedWartenAussen         * MASKE_OUT_LED_WARTEN)                 |
                        (outLedMailBuero            * MASKE_OUT_LED_MAIL_BUERO)             |
                        (outLedMailAussen           * MASKE_OUT_LED_MAIL_AUSSEN)            |
                        (outLedTelefonBuero         * MASKE_OUT_LED_TELEFON_BUERO)          |
                        (outLedTelefonAussen        * MASKE_OUT_LED_TELEFON_AUSSEN)         |
                        (outLedKlingelBuero         * MASKE_OUT_LED_KLINGEL_BUERO));
                        
        //PORT_BUERO = outLedTelefonAussen;
        PORT_BUERO  = outLed;
        PORT_AUSSEN = (outLed>>8) |(1<<0);
    }
}

