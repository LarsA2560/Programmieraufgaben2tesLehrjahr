/*
 * Toaster_Struktogramm_Aufgabe.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

#define  SYSTEMTAKT_ms      (10)
//Pin Configuration
#define DDR_LED             (DDRA)
#define PORT_LED            (PORTA)

#define DDR_SCHALTER        (DDRC)
#define PIN_Schalter        (PINC)

#define DDR_TASTER          (DDRL)
#define PIN_TASTER          (PINL)

//Maske DDR
#define ALLE_PINS_IN                    (0x00)
#define LED_PINS_OUT                    (0X15)
//Maske_IN
#define MASKE_IN_SCHALTER_ON_OFF        (1<<0)
#define MASKE_IN_DREHSCHALTER           (0b1111<<4)

#define MASKE_IN_TASTER_FLANKEHEBEL     (1<<0)
#define MASKE_IN_TASTER_FLANKENSTOPP    (1<<1)
//Maske_OUT
#define MASKE_OUT_LED_POWER             (1<<0)
#define MASKE_OUT_LED_HEIZELEMENT       (1<<1)
#define MASKE_OUT_LED_KLAPPE            (1<<2)
#define MASKE_OUT_LED_HEISS             (1<<3)
//Maske_Divers
#define ABKUEHLZEIT_MIN     (3)
#define S_ZU_MS_UMWANDLUNG  (1000)
#define MIN_ZU_S_UMWANDLUNG (60)

int main(void)
{
    initBoard(0);
    //Hardware Initialisieren
    
    //Alle Variablen Initialisieren und auf 0 setzten
    uint8_t inSchalter      = 0;
    uint8_t inTaster        = 0;
    uint8_t outLed          = 0;
    uint32_t inZeit_ms      = 0;
    
    uint8_t inOnOffSchalter = 0;
    uint8_t inDrehschalter  = 0;
    uint32_t inBackzeit     = 0;
 
    uint8_t inTaster_neu    = 0;
    uint8_t inTaster_alt    = 0;
    
    uint8_t inPosFlanke_Taster  = 0;
    uint8_t inFlankeHebel       = 0;
    uint8_t inFlankestopp       = 0;
    
    uint8_t outPowerLed         = 0;
    uint8_t outLedHeizelement   = 0;
    uint8_t outLedKlappe        = 0;
    uint8_t outLedHeiss         = 0;
    
    uint8_t flagKlappe = 0;
    
    uint32_t startZeitToastvorgang  = 0;
    uint32_t startZeitKlappe        = 0;
    int64_t startZeitBackende       = 0;
    
    while (1) //Wiederhole für immer
    {
        //Variablem Always Low/High
        
        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter      = PIN_Schalter;
        
        inOnOffSchalter = (inSchalter & MASKE_IN_SCHALTER_ON_OFF) > 0;              //OnOff-Schalter einlesen und in inOnOffSchalter Speichern
        inDrehschalter  = (inSchalter & MASKE_IN_DREHSCHALTER)>>4;                  //4Bit Drehschalter einlesen und in inDrehschalter Speichern
        inBackzeit      = ((uint32_t)inDrehschalter * 15 + 15) *S_ZU_MS_UMWANDLUNG; //inBackzeit berechnen
        
        inTaster        = PIN_TASTER;                       //Taster einlesen und in inTaster Speichern
        //Flankenerkennung      
        inTaster_alt        = inTaster_neu;                 //den Wert von Taster_neu in Taster_alt speichern
        inTaster_neu        = inTaster;                   //den Wert von inTaster in Taster_neu speichern
        inPosFlanke_Taster  = inTaster_neu & ~inTaster_alt; //Positive Flanken mit Bitlogik bestimmen
        
        inFlankeHebel = (inPosFlanke_Taster & MASKE_IN_TASTER_FLANKEHEBEL) > 0;     //inFlankeHebel aus inPosFlank maskieren
        inFlankestopp = (inPosFlanke_Taster & MASKE_IN_TASTER_FLANKENSTOPP) > 0;    //inFlankestopp aus inPosFlank isolieren
        
        inZeit_ms = getSystemTimeMs(); //Systemzeit inZeit_ms abspeichern
        
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        if (inOnOffSchalter) //ist inOnOffSchalter aktiv
        {
            outPowerLed = 1; //outPower-LED an
            
            if (inFlankeHebel) //ist inFlankehebel aktiv
            {
                outLedHeizelement       = 1;            //outLedHeizelement ist wahr
                startZeitToastvorgang   = inZeit_ms;    //inZeit_ms in startZeitToastvorgang speichern
                outLedHeiss             = 1;            //outLedHeiss an
                startZeitBackende       = -1;            //startZeitBackende falsch
            }
            if (inFlankestopp) //ist inFlankestopp aktiv
            {
                if (outLedHeizelement) //ist outLedHeizelement aktiv
                {
                    flagKlappe = 1;                 //flagKlappe ist wahr
                    startZeitKlappe = inZeit_ms;    //inZeit_ms in startZeitKlappe speichern
                    outLedHeizelement = !1;             //outLedHeizelement ist nicht wahr                
                    startZeitBackende   = inZeit_ms;    //inZeit_ms in startZeitBackende speichern
                }   
            }
        }
        else{
            outPowerLed         = 0;    // outPowerLed aus          
            outLedHeizelement   = 0;    //outLedHeizelement aus
            outLedKlappe        = 0;    //outLedKlappe aus
            flagKlappe          = 0;    //flagKlappe ist nicht wahr
            
            if (!startZeitBackende)             //ist startZeitBackende noch nicht aktiv
            {startZeitBackende = inZeit_ms;}    //inZeit_ms in startZeitBackende speichern            
        }
        
        if (outLedHeizelement) //ist outLedHeizelement wahr
        {
            if ((inZeit_ms - startZeitToastvorgang) >= inBackzeit) //inZeit_ms minus startZeitToastvorgang grösser oder gleich gross wie inBackzeit_s
            {
                flagKlappe          = 1;            //flagKlappe ist wahr
                startZeitKlappe     = inZeit_ms;    //inZeit_ms in startZeitKlappe speichern
                outLedHeizelement   = !1;           //outLedHeizelement ist nicht wahr
                startZeitBackende   = inZeit_ms;    //inZeit_ms in startZeitBackende speichern
            }
        }
        if (flagKlappe) //ist flagKlappe wahr
        {
            outLedKlappe = 1; //outLedKlappe an
            if ((inZeit_ms - startZeitKlappe) >= 500) //inZeit_ms minus startZeitKlappe grösser oder gleich gross wie 500
            {            
                flagKlappe          = !1;           //flagKlappe ist nicht wahr                
                outLedKlappe        = 0;            //outLedKlappe aus
            }                            
        }
        if (startZeitBackende >= 0) //ist startZeitBackende wahr
        {
            if (inZeit_ms - startZeitBackende >= (uint32_t)ABKUEHLZEIT_MIN * MIN_ZU_S_UMWANDLUNG * S_ZU_MS_UMWANDLUNG) //inZeit_ms minus startZeitBackende grösser oder gleich gross wie 3*60*1000
            {
                outLedHeiss         = 0;    //outLedHeiss aus
                startZeitBackende   = -1;   //startZeitBackende falsch
            }
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        outLed =    ((outPowerLed       * MASKE_OUT_LED_POWER)          //outLedPower oder
                    |(outLedHeizelement * MASKE_OUT_LED_HEIZELEMENT)    //outLedHeizelement oder
                    |(outLedKlappe      * MASKE_OUT_LED_KLAPPE)         //outLedKlappe oder
                    |(outLedHeiss       * MASKE_OUT_LED_HEISS));        //outLedHeiss
                                                                        //in outLed speichern
        PORT_LED = outLed; //outLed an den Led's ausgeben              
        //Warten_____________________________________________________________________________________________________________________________________________________________
         
    }
}

