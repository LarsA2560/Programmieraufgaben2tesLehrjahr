/*
 * Cassio_Uhr.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN
#define MASK_IN_TASTER_LIGHT (1<<0)
#define MASK_IN_TASTER_MODE (1<<1)
#define MASK_IN_TASTER_FUNCTION (1<<6).
//Maske_OUT

//Maske_Divers

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    char * MMM[3] = { "AM ","PM ","24h"};
    char * DD[9] = { "MO","TU","WE","TH","FR","SA","SO","AL","ST"};
    const uint8_t zwolfSunden_UHR[24] = { 120,10,20,30,40,50,60,70,80,90,100,110,121,11,21,31,41,51,61,71,81,91,101,111};
    uint8_t dd = 0;
    uint16_t LAP = 0;
    uint8_t hh = 0;
    uint8_t hh_24 = 0;
    uint8_t mm = 0;
    uint8_t ss = 0;
    uint8_t flagAlarmEin = 0;
    uint8_t flagBeepEin = 0;
    uint8_t flag_function = 0;
    
    uint8_t inTaster = 0;
    uint8_t inTaster_alt = 0;
    uint8_t inPosFlanke_Taster = 0;
    uint8_t inTaster_Light = 0;
    uint8_t inTaster_Mode = 0;
    uint8_t inTaster_Function = 0;
    
    uint64_t systemZeit_ms = 0;
    uint64_t sekunde = 0;
    
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        inTaster_alt = inTaster;
        inTaster = buttonReadAllPL();
        inPosFlanke_Taster = inTaster & ~inTaster_alt;
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inTaster_Light = (inPosFlanke_Taster & MASK_IN_TASTER_LIGHT)>0;
        inTaster_Mode = (inPosFlanke_Taster & MASK_IN_TASTER_MODE)>0;
        inTaster_Function = (inPosFlanke_Taster & MASK_IN_TASTER_FUNCTION)>0;
        systemZeit_ms = getSystemTimeMs();     
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________          
        hh = hh_24;
        lcdLight(255);
        if (inTaster_Function)
        {
            if (flag_function==2)
            {
                flag_function = 0;
            }
            else
            {
                flag_function = 2;
            }
        }
        if (flag_function<2)
        {
            hh = zwolfSunden_UHR[hh_24];
            flag_function = (hh % 2);
            hh = hh/10;
            
        }
        else{
            hh = hh_24;
        }
        if((systemZeit_ms - sekunde) >= 10){
            ss += 60;
            sekunde = systemZeit_ms;
        }
        if (ss>= 60)
        {
            ss = 0;
            mm += 1;
        }
        if (mm>=60)
        {
            mm = 0;
            hh_24 += 1;
        }
        if (hh_24 >= 24)
        {
            hh_24 = 0;
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        lcdWriteText(0,0,"%s %s %2u %3u",MMM[flag_function],DD[1],dd,LAP);
        lcdWriteText(1,0,"%2u:%02u:%02u",hh,mm,ss);
        //lcdWriteText(2,0,"%c %c", );     
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

