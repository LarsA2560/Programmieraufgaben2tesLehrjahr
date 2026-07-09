/*
 * Binaerhochzaehlen.c
 *
 * Created: 11/12/2025 16:04:01
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define SYSTEMTAKT_ms   (10)

#define PORT_LED        (PORTA)
#define DDR_LED         (DDRA)

#define Pin_SCHALTER    (PINC)
#define DDR_SCHALTER    (DDRC)

#define ALLE_PINS_IN                (0x00)
#define ALLE_PINS_OUT               (0xFF)

#define MASKE_IN_SCHALTER_GESCHWINDIGKEITSSCHALTER  (0b111<<0)

#define ANZAHL_KOMMASTELLEN_HOCH_10 (10) 

#define KLEINSTER_INTERVALL (1000 / ANZAHL_KOMMASTELLEN_HOCH_10)

#define MULTIPLIKATOR_GESCHWINDIGKEIT_1   (1)               
#define MULTIPLIKATOR_GESCHWINDIGKEIT_2   (2)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_3   (5)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_4   (10)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_5   (15)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_6   (20)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_7   (50)
#define MULTIPLIKATOR_GESCHWINDIGKEIT_8   (100)


int main(void)
{
    DDR_SCHALTER    = ALLE_PINS_IN;
    DDR_LED         = ALLE_PINS_OUT;
    
    uint8_t inSchalter      = 0;
    uint8_t outLed          = 0;
    uint8_t Zaehler         = 0;
    uint8_t multiplikator   = 0;
    
    uint8_t inSchalter_Geschwindigkeitsschalter  = 0;
    
    uint16_t systemZeit_ms  = 0;
    uint16_t lastCount_ms   = 0;
    uint16_t wartezeit      = 0;
    
    while (1) 
    {   

        //Eingabe
        inSchalter    = Pin_SCHALTER;
        inSchalter_Geschwindigkeitsschalter = (inSchalter & MASKE_IN_SCHALTER_GESCHWINDIGKEITSSCHALTER);
        //Verarbeitung
        
        wartezeit = (KLEINSTER_INTERVALL * multiplikator);
        

        
        if((systemZeit_ms - lastCount_ms) >= wartezeit){
            Zaehler += 1;
            lastCount_ms = systemZeit_ms;
        }        
        
         
        switch (inSchalter_Geschwindigkeitsschalter)
        {
        case 0:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_1;
            break;
        case 1:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_2;
            break;
        case 2:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_3;
            break;
        case 3:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_4;
            break;
        case 4:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_5;
            break;      
        case 5:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_6;
            break;
        case 6:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_7;
            break;
        case 7:
            multiplikator = MULTIPLIKATOR_GESCHWINDIGKEIT_8;
            break;
                                              
        }
        
            
        
    //Ausgabe        
        
    PORT_LED = Zaehler;
        
    //Warten
        
    _delay_ms(SYSTEMTAKT_ms);
    systemZeit_ms += SYSTEMTAKT_ms;

    }
}

