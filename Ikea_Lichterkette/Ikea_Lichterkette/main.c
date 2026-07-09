/*
 * Ikea_Lichterkette.c
 *
 * Created: 12/12/2025 11:30:09
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define  SYSTEMTAKT_ms      (10)

#define PORT_LED_LOW        (PORTA)
#define DDR_LED_LOW         (DDRA)

#define PORT_LED_High       (PORTB)
#define DDR_LED_High        (DDRB)

#define DDR_SCHALTER        (DDRC)
#define Pin_SCHALTER        (PINC)

#define ALLE_PINS_IN                (0x00)
#define ALLE_PINS_OUT               (0xFF)

#define MASKE_IN_SCHALTER_ON_OFF_ALTER_MODUS    (1<<0)
#define MASKE_IN_SCHALTER_ON_OFF_NEUER_MODUS    (1<<1)

#define MASKE_OUT_LED_ALLE_ON       (0xFFFF)

#define MASKE_ANIMATION_ALTER_MODUS (0x5555)

#define ALTER_MODUS_ON              (1)
#define ALTER_MODUS_OFF             (2)

#define NEUER_MODUS_FRUEHER_ABEND   (3)
#define NEUER_MODUS_NACHT           (4)
#define NEUER_MODUS_FRUEHER_MORGEN  (5)
#define NEUER_MODUS_TAG             (6)

int main(void)
{
    DDR_LED_LOW     = ALLE_PINS_OUT;
    DDR_LED_High    = ALLE_PINS_OUT;
    DDR_SCHALTER    = ALLE_PINS_IN;
    
    uint8_t inSchalter          = 0;
    uint16_t outLED             = 0;
    
    uint8_t inSchalter_ON_OFF_alter_Modus   = 0;
    uint8_t inSchalter_ON_OFF_neuer_Modus   = 0;
    
    uint8_t outLED_alle_ON      = 0;
    uint16_t outLED_alter_Modus = 0;
    
    uint16_t systemZeit_ms      = 0;
    uint16_t start_ms           = 0;
    uint16_t now                = 0;
    uint16_t start_Animation_ms  = 0;
    
    uint8_t toggle_Animation    = 0;
    uint8_t flag_Mode           = 0;
    uint8_t flag_Schalter_ON    = 0;
    uint8_t flag_Animation_Alter_Modus = 0;
    while (1) 
    {    
    
    //Eingabe
    inSchalter    = Pin_SCHALTER;
    
    inSchalter_ON_OFF_alter_Modus   = (inSchalter & MASKE_IN_SCHALTER_ON_OFF_ALTER_MODUS) > 0;
    inSchalter_ON_OFF_neuer_Modus   = (inSchalter & MASKE_IN_SCHALTER_ON_OFF_NEUER_MODUS) > 0;
    
    //Verarbeitung
    now = systemZeit_ms;
    
    if (!inSchalter_ON_OFF_alter_Modus && !inSchalter_ON_OFF_neuer_Modus)
    {
        start_ms = now;
        start_Animation_ms = now;
        flag_Mode = 0;
        flag_Schalter_ON = 0;
        flag_Animation_Alter_Modus = 0;        
    }
    if (inSchalter_ON_OFF_alter_Modus && !flag_Schalter_ON)
    {
        flag_Mode = ALTER_MODUS_ON;
        flag_Schalter_ON = 1;
    }
    

    if (inSchalter_ON_OFF_neuer_Modus && !flag_Schalter_ON)
    {
        flag_Mode = NEUER_MODUS_FRUEHER_ABEND;
        flag_Schalter_ON = 1;
    }    
    
    switch (flag_Mode)
    {
    case ALTER_MODUS_ON:
        flag_Animation_Alter_Modus = 1;
        if ((now - start_ms) >= 6000)
        {
            flag_Mode = ALTER_MODUS_OFF;
            start_ms = now;
        }
    	break;
    case ALTER_MODUS_OFF:
        flag_Animation_Alter_Modus = 0;
        outLED_alter_Modus = 0;
        if ((now - start_ms) >= 18000)
        {
            flag_Mode = ALTER_MODUS_ON;
            start_ms = now;
        }     
        break;
    case NEUER_MODUS_FRUEHER_ABEND:
        outLED_alle_ON = 1;
        if ((now - start_ms) >= 6000)
        {
            flag_Mode = NEUER_MODUS_NACHT;
            start_ms = now;
        }
        break;
    case NEUER_MODUS_NACHT:
        outLED_alle_ON = 0;
        if ((now - start_ms) >= 6000)
        {
            flag_Mode = NEUER_MODUS_FRUEHER_MORGEN;
            start_ms = now;
        }
        break;
    case NEUER_MODUS_FRUEHER_MORGEN:
        outLED_alle_ON = 1;
        if ((now - start_ms) >= 3000)
        {
            flag_Mode = NEUER_MODUS_TAG;
            start_ms = now;
        }
        break;
    case NEUER_MODUS_TAG:
        outLED_alle_ON = 0;
        if ((now - start_ms) >= 9000)
        {
            flag_Mode = NEUER_MODUS_FRUEHER_ABEND;
            start_ms = now;
        }
        break;
    default:
        outLED_alle_ON = 0;
        outLED_alter_Modus = 0;
    }   
    
     if (flag_Animation_Alter_Modus)
     {
        if ((now - start_Animation_ms) >= 500)
        {
            toggle_Animation = !toggle_Animation;
            start_Animation_ms = now;
        }

        outLED_alter_Modus = (toggle_Animation * MASKE_OUT_LED_ALLE_ON) ^ MASKE_ANIMATION_ALTER_MODUS;         
     }
     
    
    //Ausgabe
    outLED =    ((outLED_alle_ON   * MASKE_OUT_LED_ALLE_ON)
                |(outLED_alter_Modus));
    

    PORT_LED_LOW    =   outLED;
    PORT_LED_High   =   (outLED>>8);
    
    //Warten
    _delay_ms(SYSTEMTAKT_ms);
    systemZeit_ms += SYSTEMTAKT_ms;
    }
}

