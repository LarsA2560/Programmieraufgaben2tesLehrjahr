/*
 * Blinkvorlage_Lars.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration
#define DDR_LED             (DDRA)
#define PORT_LED            (PORTA)

#define DDR_SCHALTER        (DDRC)
#define PIN_Schalter        (PINC)
//Maske DDR

//Maske_IN
#define MASKE_IN_SCHALTER_ON_OFF        (1<<0)

#define EINZEIT (300)
#define PERIODENZEIT (700)
//Maske_OUT
#define MASKE_OUT_LED_BLINKEN (1<<0)
//Maske_Divers
#define STANDARTZUSTAND_LED_BLINKEN (0) //0 oder 1
#define EINSCHALTZUSTAND_SCHALTER_ONOFF (1)
int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint8_t inSchalter      = 0;
    uint8_t outLed          = 0;
    uint32_t inZeit_ms      = 0;
    
    uint8_t inOnOffSchalter = 0;
    uint8_t outLedBlinkend  = STANDARTZUSTAND_LED_BLINKEN;
    
    uint8_t flagLedBlinkend = 0;
    uint32_t blinkLedstart  = 0;
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter      = PIN_Schalter;
        
        inOnOffSchalter = (inSchalter & MASKE_IN_SCHALTER_ON_OFF) > 0;  
        
        inZeit_ms = getSystemTimeMs(); //Systemzeit inZeit_ms abspeichern
        
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        if (inOnOffSchalter ^ !EINSCHALTZUSTAND_SCHALTER_ONOFF)
        {
            flagLedBlinkend = 1;
        }
        else{
            flagLedBlinkend = 0;
            outLedBlinkend  = STANDARTZUSTAND_LED_BLINKEN;
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
         if (flagLedBlinkend)
         {
             if (inZeit_ms - blinkLedstart >= PERIODENZEIT)
             {
                 blinkLedstart = inZeit_ms;
             }
             if (inZeit_ms - blinkLedstart <= EINZEIT)
             {
                outLedBlinkend = flagLedBlinkend^STANDARTZUSTAND_LED_BLINKEN;
             }
             else{
                outLedBlinkend = STANDARTZUSTAND_LED_BLINKEN;
             }
         }
         else{blinkLedstart=inZeit_ms;
         }     
             
         outLed = outLedBlinkend * MASKE_OUT_LED_BLINKEN;
         
         PORT_LED = outLed;
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

