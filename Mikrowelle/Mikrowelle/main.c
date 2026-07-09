/*
 * Mikrowelle.c
 *
 * Created: 21/11/2025 08:21:23
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define PORT_LED                    (PORTA)
#define DDR_LED                     (DDRA)

#define PORT_RGB                    (PORTH)
#define DDR_RGB                     (DDRH)

#define PORT_SCHALTER               (PINC)
#define DDR_SCHALTER                (DDRC)

#define ALLE_PINS_IN                (0x00)
#define ALLE_PINS_OUT               (0xFF)
#define RGB_OUT                     (0b111<<3)

#define MASKE_IN_ZEITEINSTELLUNG    (0b1111<<1)
#define MASKE_IN_TUERSENSOR         (1)
#define MASKE_IN_LEISTUNG_LOW       (1<<5)
#define MASKE_IN_LEISTUNG_MEDIUM    (1<<6)
#define MASKE_IN_LEISTUNG_HIGH      (1<<7)

#define MASKE_OUT_SUMMER            (1<<1)
#define MASKE_OUT_DREHMOTOR         (1<<2)
#define MASKE_OUT_BELEUCHTUNG       (1<<7)

#define MASKE_OUT_RGB_LOW           (1<<5)
#define MASKE_OUT_RGB_MEDIUM        (1<<4)
#define MASKE_OUT_RGB_HIGH          (1<<3) 

#define SYSTEMTAKT_ms               (10)
#define SEKUNDE                     (1000)

int main(void)
{
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED             = ALLE_PINS_OUT;
    DDR_RGB             = RGB_OUT;
    
    uint8_t inSchalter              = 0;
    uint8_t inLeistung_Low          = 0;
    uint8_t inLeistung_Medium       = 0;
    uint8_t inLeistung_High         = 0;
    uint8_t inZeiteinstellung       = 0;
    uint8_t inTuersensor            = 0;

    
    
    uint16_t outLed                 = 0;
    uint16_t outRGB                 = 0;
    uint8_t outBeleuchtung          = 0;
    uint8_t outSummer               = 0;
    uint8_t outHeizelement_LOW      = 0;
    uint8_t outHeizelement_MEDIUM   = 0;
    uint8_t outHeizelement_HIGH     = 0;
    uint8_t outDrehmotor            = 0;
    
    uint16_t startsystem_Time               = 0;
    uint16_t systemZeit_ms                  = 0;
    uint8_t flag_heizvorgang_lauft          = 0;
    uint8_t flag_heizvorgang_abgeschlossen  = 0;
    uint16_t wartezeit                      = 0;
    
    while (1) 
    {    

        
        //Eingabe
        inSchalter = PORT_SCHALTER;
        inZeiteinstellung   = (inSchalter & MASKE_IN_ZEITEINSTELLUNG) >> 1;
        inTuersensor        = (inSchalter & MASKE_IN_TUERSENSOR)        > 0;
        inLeistung_Low      = (inSchalter & MASKE_IN_LEISTUNG_LOW)      > 0;
        inLeistung_Medium   = (inSchalter & MASKE_IN_LEISTUNG_MEDIUM)   > 0;
        inLeistung_High     = (inSchalter & MASKE_IN_LEISTUNG_HIGH)     > 0;
        //Verarbeitung
        
        outBeleuchtung  = inTuersensor;
        
        
        
         if (!inTuersensor)
        {
            outSummer                       = 0;
            flag_heizvorgang_lauft          = 0;
            outDrehmotor                    = 0;
            outHeizelement_LOW              = 0;
            outHeizelement_MEDIUM           = 0;
            outHeizelement_HIGH             = 0;
            flag_heizvorgang_abgeschlossen  = 0;            
            
        }
        
        if (inTuersensor && (inLeistung_Low ^ inLeistung_Medium ^ inLeistung_High) && inZeiteinstellung && !flag_heizvorgang_lauft)
        {            
            outDrehmotor            = 1;
            outHeizelement_LOW      = inLeistung_Low;
            outHeizelement_MEDIUM   = inLeistung_Medium;
            outHeizelement_HIGH     = inLeistung_High;
            startsystem_Time        = systemZeit_ms;
            wartezeit               = (inZeiteinstellung * SEKUNDE);
            flag_heizvorgang_lauft  = 1;
        }
        
        
        if (((systemZeit_ms - startsystem_Time) >= wartezeit) && flag_heizvorgang_lauft)
        {
            outDrehmotor                    = 0;
            outHeizelement_LOW              = 0;
            outHeizelement_MEDIUM           = 0;
            outHeizelement_HIGH             = 0;
            flag_heizvorgang_abgeschlossen  = 1;            
        }
        
        if (flag_heizvorgang_lauft && flag_heizvorgang_abgeschlossen && ((systemZeit_ms - startsystem_Time) >= SEKUNDE))
        {
            if (!outSummer)
            {
                outSummer = 1;
                startsystem_Time = systemZeit_ms;
            }
            else
            {
                outSummer = 0;
                startsystem_Time = systemZeit_ms;
            }
        }


    
        
        
        //Ausgabe
        outLed  =   ((outBeleuchtung    * MASKE_OUT_BELEUCHTUNG)
                    |(outDrehmotor      * MASKE_OUT_DREHMOTOR)
                    |(outSummer         * MASKE_OUT_SUMMER));
                    
        outRGB  =   ((outHeizelement_LOW    * MASKE_OUT_RGB_LOW)
                    |(outHeizelement_MEDIUM * MASKE_OUT_RGB_MEDIUM)
                    |(outHeizelement_HIGH   * MASKE_OUT_RGB_HIGH));
        
        PORT_LED = outLed;
        PORT_RGB = outRGB;
       
       //Warten
       _delay_ms(10);
       systemZeit_ms += SYSTEMTAKT_ms;
    }
}

