/*
 * Farbuebergang.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"



//Pin Configuration
#define PORT_LED_RGB    (PORTH)
#define DDR_LED_RGB     (PORTH)

#define PIN_TASTER      (PINL)
#define DDR_TASTER      (DDRL)

//Maske DDR

//Maske_IN
#define MASKE_TASTER_ONOFF (1<<0)
#define MASKE_TASTER_FARBE (1<<1)
//Maske_OUT
#define MASKE_OUT_RGB_R (1<<3)
#define MASKE_OUT_RGB_G (1<<4)
#define MASKE_OUT_RGB_B (1<<5)
//Maske_Divers
#define HELLIGKEIT (1023)
int main(void)
{
    initBoard(0);
    
      
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint8_t inTaster_alt = 0;
    uint8_t inTaster_neu = 0;
    
    uint8_t inPosFlanke = 0;
    
    uint8_t inTasterOnOff = 0;
    uint8_t inTasterFarbe = 0;
    
    uint8_t outRGB = 0;
    uint8_t outRGB_R = 0;
    uint8_t outRGB_G = 0;
    uint8_t outRGB_B = 0;
    //Andere Variablen
    
    typedef enum zustandEinAus_t {EIN, AUS} zustandEinAus_t;
    typedef enum zustandFarbe_t {ROT, GRUN, BLAU} zustandFarbe_t;
       
    zustandFarbe_t zustand_Farbe   = ROT;    
    zustandEinAus_t zustand_EinAus  = AUS;     
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
       
        inTaster_alt = inTaster_neu;
        inTaster_neu = buttonReadAllPL();
       
        inPosFlanke  = inTaster_neu &~ inTaster_alt;        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inTasterOnOff = (inPosFlanke & MASKE_TASTER_ONOFF) > 0;
        inTasterFarbe = (inPosFlanke & MASKE_TASTER_FARBE) > 0;   
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        
        if (zustand_EinAus != EIN)
        {
            zustand_Farbe = ROT;
        }
        
        switch (zustand_EinAus)
        {
        case EIN:
            switch (zustand_Farbe)
            {
            case ROT:
                outRGB_R = 1;
                outRGB_G = 0;
                outRGB_B = 0;
                
                if (inTasterFarbe)
                {
                    zustand_Farbe = GRUN;
                }
            	break;
            case GRUN:
                outRGB_R = 0;
                outRGB_G = 1;
                outRGB_B = 0;
                
                if (inTasterFarbe)
                {
                    zustand_Farbe = BLAU;
                }
                break;
            case BLAU:
                outRGB_R = 0;
                outRGB_G = 0;
                outRGB_B = 1;
                
                if (inTasterFarbe)
                {
                    zustand_Farbe = ROT;
                }
                break;
            }
            if (inTasterOnOff)
            {
                zustand_EinAus =AUS;
            }
        	break;
        case AUS:
            outRGB_R = 0;
            outRGB_G = 0;
            outRGB_B = 0;
            if (inTasterOnOff)
            {
                zustand_EinAus = EIN;
            }
            break;
        }
        
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        rgbWrite(outRGB_R * HELLIGKEIT, outRGB_G * HELLIGKEIT, outRGB_B * HELLIGKEIT);
        
              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

