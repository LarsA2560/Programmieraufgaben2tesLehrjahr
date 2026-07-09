/*
 * ErstePruefung.c
 *
 * Created: 28/11/2025 07:48:05
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL

#define PORT_LED        (PORTA)
#define DDR_LED         (DDRA)

#define PORT_SCHALTER   (PINC)
#define DDR_SCHALTER    (DDRC)

#define ALLE_PINS_IN    (0x00)
#define ALLE_PINS_OUT   (0xFF)

#define POSITION_DREHKNOPF                      (4)
#define POSITION_HEIZSTUFENANZEIGE              (3)
#define GROSSE_KOCHPLATTE_AKTIVIEREN            (15)

#define MASKE_IN_SCHALTER_ON_OFF                (1<<0)
#define MASKE_IN_SCHALTER_TEMPERATURSENSOR      (1<<1)
#define MASKE_IN_SCHALTER_WERT_DREHKNOPF        (0B1111<<POSITION_DREHKNOPF)

#define MASKE_OUT_LED_POWER                     (1<<0)
#define MASKE_OUT_LED_TEMPERATURSENSOR          (1<<1)
#define MASKE_OUT_LED_GROSSESKOCHFELD           (1<<2)
#define MASKE_OUT_LED_HEIZSTUFE                 (0b11111<<POSITION_HEIZSTUFENANZEIGE)

#define MASKE_LED_BALKENANZEIGE                 (0b11111)
#define STUFE_MAX                                 (5)   


int main(void)
{
        DDR_SCHALTER    = ALLE_PINS_IN;
        DDR_LED         = ALLE_PINS_OUT;       
    
        uint8_t inschalter                  = 0;
        uint8_t outLed                      = 0;
        
        uint8_t inSchalterOnOff             = 0;
        uint8_t inSchalterTemperatursensor  = 0;
        uint8_t inSchalterWertDrehknopf     = 0;
            
        uint8_t outLedPower                 = 0;
        uint8_t outLedTemperatursensor      = 0;
        uint8_t outLedGrossesKochfeld       = 0;
        uint8_t outLedHeizstufe             = 0;
        
    while (1) 
    {
        outLed                  = 0; //Alle Ausgänge ausser die Anzeige für das Grosse Kochfeld sind Typically off
        outLedPower             = 0;
        outLedTemperatursensor  = 0;
        outLedHeizstufe         = 0;
        
        //Eingabe
        inschalter  = PORT_SCHALTER;
        inSchalterOnOff             = (inschalter & MASKE_IN_SCHALTER_ON_OFF)               > 0;
        inSchalterTemperatursensor  = !(inschalter & MASKE_IN_SCHALTER_TEMPERATURSENSOR)     > 0;
        inSchalterWertDrehknopf     = (inschalter & MASKE_IN_SCHALTER_WERT_DREHKNOPF)       >> POSITION_DREHKNOPF;       
        //Verarbeitung
        
        
        if (inSchalterOnOff)
        {
            outLedPower = inSchalterOnOff;
            if (inSchalterWertDrehknopf == GROSSE_KOCHPLATTE_AKTIVIEREN)
                {
                    outLedGrossesKochfeld = 1;
                }       
            inSchalterWertDrehknopf = inSchalterWertDrehknopf / 2;
            if (inSchalterWertDrehknopf > STUFE_MAX)
            {
                inSchalterWertDrehknopf = STUFE_MAX;
            }
            outLedHeizstufe = (MASKE_LED_BALKENANZEIGE<<(inSchalterWertDrehknopf-STUFE_MAX));
            
        }            
    }
        
    outLedTemperatursensor = inSchalterTemperatursensor;
        
    if (!inSchalterOnOff)
    {
        outLedGrossesKochfeld = 0;
    }

       
       
    //Ausgabe
    outLed      =   ((outLedPower           * MASKE_OUT_LED_POWER)              |
                    (outLedTemperatursensor * MASKE_OUT_LED_TEMPERATURSENSOR)   |
                    (outLedGrossesKochfeld  * MASKE_OUT_LED_GROSSESKOCHFELD)    |
                    (outLedHeizstufe        << POSITION_HEIZSTUFENANZEIGE));
        
    PORT_LED    = outLed;
}
}

