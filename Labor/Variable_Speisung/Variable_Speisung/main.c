/*
 * Program_Template2.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

#define STEUERUNGMCU (PORTL)
#define MASKE_STEUERUNG_OUT (1<<2)


//Maske_IN

//Maske_OUT

//Maske_Divers

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    DDRL |= MASKE_STEUERUNG_OUT;
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint16_t inADCWert      = 0;
    uint16_t inAdcPoti  = 0;
    uint8_t outSteuerung    = 0;
    //Andere Variablen
    uint16_t inSpannung_Messung_mV = 0;
    uint16_t inSpannung_Eingestellt_mV = 0;
    uint16_t systemZeit_ms = 0;
    uint16_t lcdDelaystart_ms = 0;
    
    lcdWriteText(0,0,"Buck-Converter V1.0");
    lcdWriteText(1,0,"Lars Maerki");
    lcdWriteText(2,0,"U Soll:      mV");
    lcdWriteText(3,0,"U ist:       mV");
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inADCWert = adcRead(ADC_03_X4_PORTF_BIT3);
        inAdcPoti = adcRead(ADC_08_POTI_1); 
        systemZeit_ms = getSystemTimeMs();    
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        inSpannung_Messung_mV = (uint32_t)inADCWert*5000/1023;
        inSpannung_Eingestellt_mV = (uint32_t)inAdcPoti*5000/1023;
        
        if (inSpannung_Messung_mV<inSpannung_Eingestellt_mV)
        {STEUERUNGMCU &= ~MASKE_STEUERUNG_OUT;}
        
        if (inSpannung_Messung_mV>inSpannung_Eingestellt_mV)
        {STEUERUNGMCU |= MASKE_STEUERUNG_OUT;}  
               
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        if((systemZeit_ms - lcdDelaystart_ms) >= 50){
            lcdWriteText(2,8,"%04u",inSpannung_Eingestellt_mV);
            lcdWriteText(3,8,"%04u",inSpannung_Messung_mV);
            lcdDelaystart_ms = systemZeit_ms;
        }
              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

