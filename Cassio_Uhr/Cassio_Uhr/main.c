/*
 * Cassio_Uhr.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 
//#pragma GCC optimize 0

#include "ucBoardDriver.h"


//Pin Configuration

//Maske DDR

//Maske_IN
#define MASK_IN_TASTER_LIGHT (1<<0)
#define MASK_IN_TASTER_MODE (1<<1)
#define MASK_IN_TASTER_FUNCTION (1<<6)
#define SONNTAG (6)
#define MONTAG (0)
#define AlARM (7)
#define STOPPUHR (8)

#define MASK_OUT_MAX_BRIGHTNESS_LCD (255)

#define MASK_SCHRITTGROSSE_s (1)
#define MASK_SCHRITTGROSSE_m (1)
#define MASK_SCHRITTGROSSE_h (1)
#define MASK_SCHRITTGROSSE_d (1)
#define MASK_SCHRITTGESCHWINDIGKEIT_ms (1000)
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
    typedef enum zustand_mode_t        {NORMALZEITANZEIGE, TAGLICHER_ALARM, STOPPUHRFUNKTION, ZEIT_KALENDER_EINSTELLFUNKTION}             zustand_mode_t;
    zustand_mode_t zustand_mode = NORMALZEITANZEIGE;
        
    uint8_t dd = 0;
    uint16_t LAP = 0;
    uint8_t hh = 0;
    uint8_t hh_24 = 0;
    uint8_t mm = 0;
    uint8_t ss = 0;
    uint8_t flagAlarmEin = 0;
    uint8_t flagBeepEin = 0;
    uint8_t flag_function = 0;
    uint8_t Wochentag = 0;
    uint8_t Wochentag_Ausgabe = 0;
    
    uint8_t inTaster = 0;
    uint8_t inTaster_alt = 0;
    uint8_t inPosFlanke_Taster = 0;
    uint8_t inTaster_Light = 0;
    uint8_t inTaster_Mode = 0;
    uint8_t inTaster_Function = 0;
    
    uint8_t outLCDbrightness = 0;
    
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
        Wochentag_Ausgabe = Wochentag;
        if (inTaster_Light)
        {
            outLCDbrightness = !outLCDbrightness;
        }
        if (inTaster_Mode)
        {
            zustand_mode += 1;
            if (zustand_mode > 3)
            {
                zustand_mode = NORMALZEITANZEIGE;
            }
        }
        
        switch (zustand_mode)
        {
        case NORMALZEITANZEIGE:
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
        
            if((systemZeit_ms - sekunde) >= MASK_SCHRITTGESCHWINDIGKEIT_ms){
                ss += MASK_SCHRITTGROSSE_s;
                sekunde = systemZeit_ms;
            }
            if (ss>= 60)
            {
                ss = 0;
                mm += MASK_SCHRITTGROSSE_m;
            }
            if (mm>=60)
            {
                mm = 0;
                hh_24 += MASK_SCHRITTGROSSE_h;
            }
            if (hh_24 >= 24)
            {
                hh_24 = 0;
                dd += MASK_SCHRITTGROSSE_d;
                Wochentag += 1;
            }
            if (Wochentag == SONNTAG)
            {
                Wochentag = MONTAG;
                Wochentag_Ausgabe = Wochentag;
            }
        	break;
        case TAGLICHER_ALARM:
            Wochentag_Ausgabe = AlARM; 
            break;
        case STOPPUHRFUNKTION:
            Wochentag_Ausgabe = STOPPUHR;
            
            break;
        case ZEIT_KALENDER_EINSTELLFUNKTION:
            
            break;
    	}     
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        lcdLight(outLCDbrightness*MASK_OUT_MAX_BRIGHTNESS_LCD);
        lcdWriteText(0,0,"%s %s %2u %3u",MMM[flag_function],DD[Wochentag_Ausgabe],dd,LAP);
        lcdWriteText(1,0,"%2u:%02u:%02u",hh,mm,ss);
        //lcdWriteText(2,0,"%c %c", );     
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

