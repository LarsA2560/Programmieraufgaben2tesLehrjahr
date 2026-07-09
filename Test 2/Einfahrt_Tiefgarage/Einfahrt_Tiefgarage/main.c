/*
 * Einfahrt_Tiefgarage.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"


//Maske_IN
#define MASKE_IN_SCHALTER_LICHTSCHRANKE (1<<0)
#define MASKE_IN_TASTER_EINFAHRT        (1<<0)
#define MASKE_IN_TASTER_AUSFAHRT        (1<<1)

//Maske_OUT
#define MASKE_OUT_LED_EINFAHRT_R    (1<<0)
#define MASKE_OUT_LED_EINFAHRT_G    (1<<1)
#define MASKE_OUT_LED_AUSFAHRT_R    (1<<3)
#define MASKE_OUT_LED_AUSFAHRT_G    (1<<4)
#define MASKE_OUT_LED_TOR           (1<<7)

#define ZEIT_BEDIENUNGFEHLER_ms (5000)
#define ZEIT_Lichtschranke_ms   (10000)
#define ZEIT_BLINKEN_ms         (1000)
#define ZEIT_ENTPRELLEN_ms      (0)

//Maske_Divers

int main(void)
{
    //Hardware initialisieren
    initBoard(0);

    //Alle Variablen Initialisieren und auf 0 setzten
    //Einlese/Ausgabe Variablen
    uint8_t inSchalter      = 0;
    uint8_t inTaster        = 0;
    uint8_t outLed          = 0;
    uint16_t systemZeit_ms  = 0;
    
    uint8_t inSchalterLichtschranke = 0;
    uint8_t inTaster_Einfahrt       = 0;
    uint8_t inTaster_Ausfahrt       = 0;
    
    uint8_t outLedEinfahrt_R    = 0;
    uint8_t outLedEinfahrt_G    = 0;
    uint8_t outLedAusfahrt_R    = 0;
    uint8_t outLedAusfahrt_G    = 0;
    uint8_t outLedTor           = 0;
    
    uint8_t flag_Blinken            = 0;
    uint16_t timerBedienungsfehler  = 0;
    uint16_t timerLichtschranke     = 0;
    uint16_t timerBlinken           = 0;
    
    //Andere Variablen
    //zustand_Tiefgarage definieren
    typedef enum zustand_Tiefgarage_t        {ROT_NORMALBETRIEB, EINFAHRT, AUSFAHRT, LICHTSCHRANKE_UNTERBROCHEN, FEHLER}             zustand_Tiefgarage_t;
    //zustand_Tiefgarage = ROT_NORMALBETRIEB
    zustand_Tiefgarage_t zustand_Tiefgarage = ROT_NORMALBETRIEB;
    
    //Unendlichschlaufe   
    while (1) 
    {
        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        
        /*Schalter einlesen und in inTaster speichern
        Taster einlesen und in inTaster speichern*/        
        inSchalter  = switchReadAll();
        inTaster    = buttonReadAllPL();
        
        
        //inSchalterLichtschranke aus inSchalter maskieren
        inSchalterLichtschranke = (inSchalter & MASKE_IN_SCHALTER_LICHTSCHRANKE) > 0;
        //inTaster_Einfahrt aus inTaster maskieren
        inTaster_Einfahrt       = (inTaster & MASKE_IN_TASTER_EINFAHRT) > 0;
        //inTaster_Ausfahrt aus inTaster maskieren
        inTaster_Ausfahrt       = (inTaster & MASKE_IN_TASTER_AUSFAHRT) > 0;
        
        //Systemzeit in systemZeit_ms speichern
        systemZeit_ms = getSystemTimeMs() ; 
              
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        //zustand_Tiefgarage
        switch (zustand_Tiefgarage)
        {
        //ROT_NORMALBETRIEB
        case ROT_NORMALBETRIEB:            
            outLedEinfahrt_R    = 1;
            outLedEinfahrt_G    = 0;
            outLedAusfahrt_R    = 1;
            outLedAusfahrt_G    = 0;
            outLedTor           = 1;
            flag_Blinken        = 0;
            
            //inTaster_Einfahrt = 1
            if (inTaster_Einfahrt)
            {
                //zustand_Tiefgarage = EINFAHRT
                zustand_Tiefgarage = EINFAHRT;
                //BedienungsfehlerTimer 5s starten
                timerBedienungsfehler = systemZeit_ms;
            }
            
            //inTaster_Ausfahrt = 1
            if (inTaster_Ausfahrt)
            {
                //zustand_Tiefgarage = AUSFAHRT
                zustand_Tiefgarage = AUSFAHRT;
                //BedienungsfehlerTimer 5s starten
                timerBedienungsfehler = systemZeit_ms;
            }            
        	break;
        //EINFAHRT
        case EINFAHRT:
            outLedEinfahrt_R    = 0;
            outLedEinfahrt_G    = 1;
            outLedAusfahrt_R    = 1;
            outLedAusfahrt_G    = 0;
            outLedTor           = 0;
            flag_Blinken        = 0;
            
            //BedienungsfehlerTimer abgelaufen
            if((systemZeit_ms - timerBedienungsfehler) >= ZEIT_BEDIENUNGFEHLER_ms)
            {
                //zustand_Tiefgarage = ROT_NORMALBETRIEB
                zustand_Tiefgarage = ROT_NORMALBETRIEB;
            }
            //inSchalterLichtschranke = 0
            if (!inSchalterLichtschranke)
            {
                //zustand_Tiefgarage = LICHTSCHRANKE_UNTERBROCHEN
                zustand_Tiefgarage = LICHTSCHRANKE_UNTERBROCHEN;
                //LichtschrankenTimer 10s starten
                timerLichtschranke = systemZeit_ms;
            }
            break;
        //AUSFAHRT
        case AUSFAHRT:
            outLedEinfahrt_R    = 1;
            outLedEinfahrt_G    = 0;
            outLedAusfahrt_R    = 0;
            outLedAusfahrt_G    = 1;
            outLedTor           = 0;
            flag_Blinken        = 0;
            
            //BedienungsfehlerTimer abgelaufen
            if((systemZeit_ms - timerBedienungsfehler) >= ZEIT_BEDIENUNGFEHLER_ms)
            {
                //zustand_Tiefgarage = ROT_NORMALBETRIEB
                zustand_Tiefgarage = ROT_NORMALBETRIEB;
            }
            //inSchalterLichtschranke = 0
            if (!inSchalterLichtschranke)
            {
                //zustand_Tiefgarage = LICHTSCHRANKE_UNTERBROCHEN
                zustand_Tiefgarage = LICHTSCHRANKE_UNTERBROCHEN;
                //LichtschrankenTimer 10s starten
                timerLichtschranke = systemZeit_ms;
            }
            break;
        //LICHTSCHRANKE_UNTERBROCHEN
        case LICHTSCHRANKE_UNTERBROCHEN:
            flag_Blinken = 0;
            //inSchalterLichtschranke = 1 und Entprellzeit ist abgelaufen
            if (inSchalterLichtschranke && ((systemZeit_ms - timerLichtschranke) >= ZEIT_ENTPRELLEN_ms ))
            {
                //zustand_Tiefgarage = ROT_NORMALBETRIEB
                zustand_Tiefgarage = ROT_NORMALBETRIEB;    
            }
            //LichtschrankenTimer abgelaufen
            if((systemZeit_ms - timerLichtschranke) >= ZEIT_Lichtschranke_ms)
            {
                //zustand_Tiefgarage = FEHLER  
                zustand_Tiefgarage = FEHLER;
            }
            break;
        //FEHLER
        case FEHLER:
            outLedEinfahrt_R    = 0;
            outLedEinfahrt_G    = 0;
            outLedAusfahrt_R    = 0;
            outLedAusfahrt_G    = 0;
            outLedTor           = 0;
            flag_Blinken        = 1;
            
            //inSchalterLichtschranke = 1
            if (inSchalterLichtschranke)
            {
                //zustand_Tiefgarage = ROT_NORMALBETRIEB
                zustand_Tiefgarage = ROT_NORMALBETRIEB;
            }
            break;                                            
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        
        //Logik Blinken
        //flag_Blinken = 1
        if (flag_Blinken)
        {
            //Blinktimer 1s Vergangen
            if((systemZeit_ms - timerBlinken) >= ZEIT_BLINKEN_ms)
            {
                //BlinkTimer = sytemZeit_ms
                timerBlinken = systemZeit_ms;
            }
            
            //Blinktimer 500ms Vergangen
            if((systemZeit_ms - timerBlinken) >= (ZEIT_BLINKEN_ms/2))
            {
            /*  outLedEinfahrt_R    = 1
                outLedAusfahrt_R    = 1*/
                outLedEinfahrt_R    = 1;
                outLedAusfahrt_R    = 1;
            }
            else
            {
            /*  outLedEinfahrt_G    = 1
                outLedAusfahrt_G    = 1*/ 
                outLedEinfahrt_G    = 1;
                outLedAusfahrt_G    = 1;                   
            }
            
        }
        else
        {
            //BlinkTimer = sytemZeit_ms
            timerBlinken = systemZeit_ms;            
        }
        //Augabevariablen Maskieren und in outLed Speichern
        outLed =    ((outLedEinfahrt_G  * MASKE_OUT_LED_EINFAHRT_G) |
                    ( outLedEinfahrt_R  * MASKE_OUT_LED_EINFAHRT_R) |   
                    ( outLedAusfahrt_G  * MASKE_OUT_LED_AUSFAHRT_G) |
                    ( outLedAusfahrt_R  * MASKE_OUT_LED_AUSFAHRT_R) |
                    ( outLedTor         * MASKE_OUT_LED_TOR)        );
        //outLed an den Led's ausgeben
        ledWriteAll(outLed);      
        
    }
}

