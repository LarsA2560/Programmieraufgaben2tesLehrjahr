/*
 * Stirnlampe.c
 *
 * Created: 18/12/2025 08:05:49
 * Author : Lars
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL 
#include <util/delay.h>
#define  SYSTEMTAKT_ms      (10)

#define PORT_LED    (PORTA)
#define DDR_LED     (DDRA)

#define PIN_SCHALTER    (PINK)
#define DDR_SCHALTER    (DDRK)

#define PIN_TASTER  (PINE)
#define DDR_TASTER  (DDRE)

#define ALLE_PINS_IN    (0x00)
#define ALLE_PINS_OUT   (0xFF)

#define MASKE_IN_TASTER_ON_OFF                  (1<<4)
#define MASKE_IN_SCHALTER_HELLIGKEIT            (0b111<<0)
#define MASKE_IN_SCHALTER_LADUNG_KLEINER_10     (1<<6)
#define MASKE_IN_SCHALTER_LADUNG_LEER           (1<<7)

#define MASKE_OUT_LED_A                 (1 << 0)
#define MASKE_OUT_LED_B                 (1 << 1)
#define MASKE_OUT_LED_C                 (1 << 2)
#define MASKE_OUT_LED_D                 (1 << 3)
#define MASKE_OUT_LED_E                 (1 << 4)
#define MASKE_OUT_LED_F                 (1 << 5)
#define MASKE_OUT_LED_G                 (1 << 6)
#define MASKE_OUT_LED_H                 (1 << 7)

#define MASKE_OUT_LED_A_E               (MASKE_OUT_LED_A|MASKE_OUT_LED_E)
#define MASKE_OUT_LED_B_D               (MASKE_OUT_LED_B|MASKE_OUT_LED_D)
#define MASKE_OUT_LED_F_H               (MASKE_OUT_LED_F|MASKE_OUT_LED_H)

#define ANZAHL_HELLIGKEITSSTUFEN        (8)
#define ANZAHL_BLINKEN                  (3)
#define BLINKEN_FREQUENZ_HZ             (2)
#define DUTY_CYCLE                      (50)

#define BLINKEN_FREQUENZ_MS             (500/BLINKEN_FREQUENZ_HZ*DUTY_CYCLE/100)

int main(void)
{
    DDR_SCHALTER    = ALLE_PINS_IN;
    DDR_TASTER      = ALLE_PINS_IN;
    DDR_LED         = ALLE_PINS_OUT;
    
    uint8_t inSchalter      = 0;
    uint8_t inTaster_neu    = 0;
    uint8_t inTaster_alt    = 0;
    uint8_t outLed          = 0;
    uint16_t systemZeit_ms   = 0;
    
    uint8_t inPosFlanke_Taster  = 0;
    
    uint8_t inPosTaster_On_Off              = 0;
    uint8_t inSchalter_Helligkeit           = 0;
    uint8_t inSchalter_Ladung_groesser_10   = 0;
    uint8_t inSchalter_Ladung_Leer          = 0;
    
    uint8_t outLed_A    = 0;
    /*   
    uint8_t outLed_A_E  = 0;
    uint8_t outLed_B_D  = 0;
    uint8_t outLed_C    = 0;
    uint8_t outLed_F_H  = 0;
    uint8_t outLed_G    = 0;
    */
    uint8_t Lampe_On        = 0;
    uint8_t anzahlBlinken   = 0;
    
    uint16_t startTime       = 0;
    
    const uint8_t HELLIGKEITSSTUFE_ZU_MUSTER[ANZAHL_HELLIGKEITSSTUFEN] =
    {   MASKE_OUT_LED_C,
        MASKE_OUT_LED_C|MASKE_OUT_LED_G,
        MASKE_OUT_LED_B|MASKE_OUT_LED_C|MASKE_OUT_LED_D,
        MASKE_OUT_LED_C|MASKE_OUT_LED_F|MASKE_OUT_LED_G|MASKE_OUT_LED_H,
        MASKE_OUT_LED_A|MASKE_OUT_LED_B|MASKE_OUT_LED_C|MASKE_OUT_LED_D|MASKE_OUT_LED_E,
        MASKE_OUT_LED_B|MASKE_OUT_LED_C|MASKE_OUT_LED_D|MASKE_OUT_LED_F|MASKE_OUT_LED_G|MASKE_OUT_LED_H,
        MASKE_OUT_LED_A|MASKE_OUT_LED_B|MASKE_OUT_LED_C|MASKE_OUT_LED_D|MASKE_OUT_LED_E|MASKE_OUT_LED_F|MASKE_OUT_LED_H,
        MASKE_OUT_LED_A|MASKE_OUT_LED_B|MASKE_OUT_LED_C|MASKE_OUT_LED_D|MASKE_OUT_LED_E|MASKE_OUT_LED_F|MASKE_OUT_LED_G|MASKE_OUT_LED_H  
    };
    
    while (1) 
    {
        
        /*
        outLed_A_E  = 0;
        outLed_B_D  = 0;
        outLed_C    = 0;
        outLed_F_H  = 0;
        outLed_G    = 0;
        */
        //Eingabe
        inTaster_alt    = inTaster_neu;
        
        inSchalter      = PIN_SCHALTER;
        inTaster_neu    = PIN_TASTER;
        
        inPosFlanke_Taster  = inTaster_neu & (inTaster_alt ^ inTaster_neu);
        
        inPosTaster_On_Off              = (inPosFlanke_Taster   & MASKE_IN_TASTER_ON_OFF)               > 0;
        inSchalter_Helligkeit           = (inSchalter           & MASKE_IN_SCHALTER_HELLIGKEIT);
        inSchalter_Ladung_groesser_10   = (inSchalter           & MASKE_IN_SCHALTER_LADUNG_KLEINER_10)  > 0;
        inSchalter_Ladung_Leer          = (inSchalter           & MASKE_IN_SCHALTER_LADUNG_LEER)        > 0;
        
        //Verarbeitung
        
        if (inPosTaster_On_Off)
        {
            Lampe_On = (!Lampe_On)>0;
            
        }
        if (!inSchalter_Ladung_Leer && inPosTaster_On_Off)
            {
                anzahlBlinken   = ANZAHL_BLINKEN;
                startTime       = systemZeit_ms;
                               
            }
        if (inSchalter_Ladung_Leer)
        {
            outLed_A = 0;
        }
            
        /*
        if (Lampe_On)
        {
            
            outLed_C = 1;
            switch (inSchalter_Helligkeit)
            {
            case 1:
                outLed_G    = 1;
                break;
            case 2:
                outLed_B_D  = 1;
                break;
            case 3:
                outLed_F_H  = 1;
                outLed_G    = 1;
                break;
            case 4:
                outLed_A_E  = 1;
                outLed_B_D  = 1;
                break;
            case 5:                
                outLed_B_D  = 1;
                outLed_F_H  = 1;
                outLed_G    = 1;
                break;
            case 6:
                outLed_A_E  = 1;
                outLed_B_D  = 1;
                outLed_F_H  = 1;
                break;
            case 7:
                outLed_A_E  = 1;
                outLed_B_D  = 1;
                outLed_F_H  = 1;
                outLed_G    = 1;
                break;                
            }
            
            
        }
        */

        
        if ((!inSchalter_Ladung_groesser_10) && (inSchalter_Helligkeit>3))
        {
            inSchalter_Helligkeit = 3;
        }
        Lampe_On = (Lampe_On && inSchalter_Ladung_Leer);
        
        //Ausgabe
        
        if(anzahlBlinken && ((systemZeit_ms - startTime) >= BLINKEN_FREQUENZ_MS)){            
            if (outLed_A)
            {
                anzahlBlinken = anzahlBlinken - 1;
                outLed_A=0;
            }
            else{
                outLed_A = 1; 
            }
            startTime = systemZeit_ms;
        } 
        
        
        outLed =    ((Lampe_On * HELLIGKEITSSTUFE_ZU_MUSTER[inSchalter_Helligkeit])
                    |(outLed_A * MASKE_OUT_LED_A));
               
        /*
        outLed =    ((outLed_A_E    * MASKE_OUT_LED_A_E)   
                    | ( outLed_B_D  * MASKE_OUT_LED_B_D)
                    | ( outLed_C    * MASKE_OUT_LED_C)
                    | ( outLed_F_H  * MASKE_OUT_LED_F_H)
                    | ( outLed_G    * MASKE_OUT_LED_G));
        */
        PORT_LED = outLed;
        
        //Warten
        _delay_ms(SYSTEMTAKT_ms);
        systemZeit_ms += SYSTEMTAKT_ms;
    }
}

