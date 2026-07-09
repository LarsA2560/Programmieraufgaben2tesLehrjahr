/*
 * Labor_Pruefung_IO-Ports.c
 *
 * Created: 08/12/2025 13:44:46
 * Author : Lars
 */ 

#include <avr/io.h>

#define PORT_LICHT_KLASSENZIMMER        (PORTA)
#define DDR_LICHT_KLASSENZIMMER         (DDRA)

#define PORT_AUSSEN                     (PORTB)
#define DDR_AUSSEN                      (DDRB)
#define PIN_AUSSEN                      (PINB)

#define PORT_SCHALTER_KLASSENZIMMER     (PINC)
#define DDR_SCHALTER_KLASSENZIMMER      (DDRC)

#define ALLE_PINS_IN            (0x00)
#define PINOUT_KLASSENZIMMER    (0b11   << 6)
#define PINOUT_AUSSEN           (0b110101  << 0)

#define MASKE_IN_SCHALTER_LICHTSCHALTER         (1<<0)
#define MASKE_IN_SCHALTER_LEHRERPULT            (1<<1)
#define MASKE_IN_SCHALTER_NICHT_STOEREN         (1<<2)
#define MASKE_IN_SCHALTER_LABORTISCH_LEHRERPULT (1<<3)
#define MASKE_IN_SCHALTER_LABORTISCH_LABORTISCH (1<<1)
#define MASKE_IN_SCHALTER_NOTAUS                (1<<3)

#define MASKE_OUT_LICHT_LEHRERPULT      (1<<6)   
#define MASKE_OUT_LICHT_KLASSE          (1<<7)
#define MASKE_OUT_LICHT_NICHT_STOEREN   (1<<8)
#define MASKE_OUT_LICHT_STROMVERSORGUNG (1<<10)
#define MASKE_OUT_LICHT_NOTAUS_ROT      (1<<12)
#define MASKE_OUT_LICHT_NOTAUS_ORANGE   (1<<13)


int main(void)
{
    DDR_SCHALTER_KLASSENZIMMER  = ALLE_PINS_IN;
    DDR_LICHT_KLASSENZIMMER     = PINOUT_KLASSENZIMMER;
    DDR_AUSSEN                  = PINOUT_AUSSEN;
    PORT_AUSSEN |= (1<<3);
    
    uint8_t inSchalter_Klassenzimmer            = 0;
    uint8_t inSchalter_Aussen                   = 0;    
    uint16_t outLed                             = 0; 
    
    uint8_t inSchalter_Lichtschalter            = 0;
    uint8_t inSchalter_Lehrerpult               = 0;
    uint8_t inSchalter_nicht_Stoeren            = 0;
    uint8_t inSchalter_Labortisch_Lehrerpult    = 0;
    uint8_t inSchalter_Labortisch_Labortisch    = 0;
    uint8_t inSchalter_Notaus                   = 1;
    
    uint8_t outLicht_Lehrerpult                 = 0;
    uint8_t outLicht_Klasse                     = 0;
    uint8_t outLicht_nicht_Stoeren              = 0;
    uint8_t outLicht_Stromversorgung            = 0;
    uint8_t outLicht_Notaus_Rot                 = 0;
    uint8_t outLicht_Notaus_Orange              = 0;
    
    while (1) 
    {
        outLicht_Lehrerpult     = 0;
        outLicht_Klasse         = 0;      
        
        //Eingabe
        inSchalter_Klassenzimmer    = PORT_SCHALTER_KLASSENZIMMER;  
        inSchalter_Aussen           = PIN_AUSSEN;
        inSchalter_Aussen = PIN_AUSSEN;
        
        inSchalter_Lichtschalter            = (inSchalter_Klassenzimmer & MASKE_IN_SCHALTER_LICHTSCHALTER)          > 0;
        inSchalter_Lehrerpult               = (inSchalter_Klassenzimmer & MASKE_IN_SCHALTER_LEHRERPULT)             > 0;
        inSchalter_nicht_Stoeren            = (inSchalter_Klassenzimmer & MASKE_IN_SCHALTER_NICHT_STOEREN)          > 0;
        inSchalter_Labortisch_Lehrerpult    = (inSchalter_Klassenzimmer & MASKE_IN_SCHALTER_LABORTISCH_LEHRERPULT)  > 0;
        inSchalter_Labortisch_Labortisch    = (inSchalter_Aussen        & MASKE_IN_SCHALTER_LABORTISCH_LABORTISCH)  > 0;
        inSchalter_Notaus                   = (inSchalter_Aussen        & MASKE_IN_SCHALTER_NOTAUS)                 > 0;
        
        //Verarbeitung
        
        //Aufgabe 1
        if (inSchalter_Lichtschalter)
        {
            outLicht_Klasse = 1;
            if (inSchalter_Lehrerpult)
            {
                outLicht_Lehrerpult = 0;
            }
            else{
                outLicht_Lehrerpult = 1;
            }           
        }
        //Aufgabe 2
        outLicht_nicht_Stoeren = inSchalter_nicht_Stoeren; 
        
        //Aufgabe 3
        outLicht_Stromversorgung = (inSchalter_Labortisch_Labortisch||inSchalter_Labortisch_Lehrerpult) > 0;
        
        //Aufgabe 4
        outLicht_Notaus_Rot = inSchalter_Notaus;
        

        if (!inSchalter_Notaus)
        {
            outLicht_Notaus_Orange = 1;
        }
        
        //Ausgabe     
        outLed      =   ((outLicht_Lehrerpult       * MASKE_OUT_LICHT_LEHRERPULT)       |
                        (outLicht_Klasse            * MASKE_OUT_LICHT_KLASSE)           |
                        (outLicht_nicht_Stoeren     * MASKE_OUT_LICHT_NICHT_STOEREN)    |
                        (outLicht_Stromversorgung   * MASKE_OUT_LICHT_STROMVERSORGUNG)  |
                        (outLicht_Notaus_Rot        * MASKE_OUT_LICHT_NOTAUS_ROT)       |
                        (outLicht_Notaus_Orange     * MASKE_OUT_LICHT_NOTAUS_ORANGE)    );
        
        
        PORT_LICHT_KLASSENZIMMER    = outLed;
        PORT_AUSSEN                 = (outLed>>8)|(1<<3);
    }
}

