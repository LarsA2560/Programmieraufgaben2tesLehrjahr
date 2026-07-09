/*
 * Loetstation.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN
#define MASKE_SCHALTER_AN_AUS (1<<7)
#define MASKE_TEMP_SENSOR (0B1111<<0)
#define MASKE_TASTER_UP (1<<0)
#define MASKE_TASTER_DOWN (1<<1)
//Maske_OUT

//Maske_Divers

int main(void)
{
    //Hardware Initialisieren
    initBoard(0);
    //Alle Variablen Initialisieren und auf 0 setzten
    //Einlese/Ausgabe Variablen
    uint8_t inTaster = 0;
    uint8_t inTaster_alt = 0;
    uint8_t inPosTaster = 0;
    uint8_t inSchalter = 0;
    uint8_t inSchalterAnAus = 0;
    uint8_t inTempSensor = 0;
    uint8_t inTasterUp = 0;
    uint8_t inTasterDown = 0;
    
    uint8_t outLed = 0;
    uint8_t outBetriebsanzeige = 0;
    uint8_t outHeizen = 0;
    uint8_t outTempAnzeige = 0;
    
    uint8_t istTemperatur = 0;
    uint8_t sollTemperatur = 0;
    uint8_t flagBlinkenTempDif = 0;
    uint8_t flagBlinken_Fehler = 0;
    uint8_t flagFunktionTaster = 0;
    uint8_t blinken = 0;
    uint8_t temperaturanzeige = 0;
   
    //Andere Variablen
    
    //zustand_Loetstation definieren
    typedef enum zustand_Loetstation_t        {AUS, FEHLER , STANDBY, TEMP_DIFF}             zustand_Loetstation_t;
    //zustand_Loetstation = Aus
    zustand_Loetstation_t zustand_Loetstation = AUS;
    //array einlesenTemparaturSensor definieren
    const uint8_t einlesenTemparaturSensor [16] =
    { 0,1,3,2,7,6,5,15,14,12,13,8,9,11,10};
    //array ausgabeTemparaturAnzeige definieren   
    const uint8_t ausgabeTemparaturAnzeige [12] =
    { 1,3,2,6,4,12,8,24,16,48,32,0b111111};
    
    //Unendlichschlaufe   
    while (1) 
    {
        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        //Flankenerkennung
        //den Wert von inTaster in inTaster_alt speichern
        inTaster_alt = inTaster;
        //Taster einlesen und in inTaster speichern
        inTaster = buttonReadAllPL();
        //Positive Flanken mit Bitlogik bestimmen
        //inPosTaster = inTaster_alt nicht gesetzt UND inTaster gesetzt     
        inPosTaster = inTaster & ~inTaster_alt;
        //Schalter einlesen und in inSchalter speichern
        inSchalter = switchReadAll();
        
        //inSchalterAnAus aus inSchalter Maskieren
        inSchalterAnAus     = (inSchalter & MASKE_SCHALTER_AN_AUS) > 0;
        //inTempSensor aus inSchalter Maskieren
        inTempSensor     = (inSchalter & MASKE_TEMP_SENSOR);
        //inTasterUp aus inPosTaster Maskieren
        inTasterUp     = (inSchalter & MASKE_TASTER_UP)>0;
        //inTasterDown aus inPosTaster Maskieren
        inTasterDown     = (inSchalter & MASKE_TASTER_DOWN)>0;
        
        
        //Systemzeit in inZeit_ms abspeichern
                 
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        if (!inSchalterAnAus)
            {temperaturanzeige = 0; zustand_Loetstation = AUS;}
        else
        { 
            temperaturanzeige = 1;
            if (!inTempSensor)
                {zustand_Loetstation = FEHLER;}
        }
        
        switch (zustand_Loetstation)
        {
        case AUS:
        outBetriebsanzeige = 0;
        outHeizen = 0;
        outTempAnzeige = 0;
        flagBlinkenTempDif = 0;
        flagBlinken_Fehler = 0;
        flagFunktionTaster = 0;
         if (inSchalterAnAus)
         {
             zustand_Loetstation = STANDBY;
         }
        	break;
        case FEHLER:
        outBetriebsanzeige = 1;
        outHeizen = 0;
        flagBlinkenTempDif = 0;
        flagBlinken_Fehler = 1;
        flagFunktionTaster = 0;
        sollTemperatur = 11;
            break; 
        case STANDBY:
        outBetriebsanzeige = 1;
        outHeizen = 0;
        flagBlinkenTempDif = 0;
        flagBlinken_Fehler = 0;
        flagFunktionTaster = 1;
            break;
        case TEMP_DIFF:
        outBetriebsanzeige = 1;
        flagBlinkenTempDif = 1;
        flagBlinken_Fehler = 0;
        flagFunktionTaster = 1;
            break;   
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

