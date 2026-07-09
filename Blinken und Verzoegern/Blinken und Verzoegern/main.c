/*
 * Blinken_und_Verzoegern.c
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

#define DDR_TASTER          (DDRL)
#define PIN_TASTER          (PINL)
//Maske DDR

//Maske_IN
#define MASKE_IN_SCHALTER_0 (1<<0)
#define MASKE_IN_SCHALTER_1 (1<<1)
#define MASKE_IN_SCHALTER_2 (1<<2)
#define MASKE_IN_TASTER_0   (1<<0)
//Maske_OUT
#define MASKE_OUT_LED_0     (1<<0)
#define MASKE_OUT_LED_1     (1<<1)
#define MASKE_OUT_LED_2     (1<<2)
#define MASKE_OUT_LED_7     (1<<7)
//Maske_Divers
#define STANDARTZUSTAND_LED_BLINKEN (0)
#define EINSCHALTZUSTAND_SCHALTER   (1)

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint8_t inSchalter      = 0;
    uint8_t inTaster        = 0;
    uint8_t outLed          = 0;
    uint32_t inZeit_ms      = 0;
    
    uint8_t inSchalter_alt  = 0;
    uint8_t inSchalter_neu  = 0;
    uint8_t inTaster_alt    = 0;
    uint8_t inTaster_neu    = 0;
    
    uint8_t inFlankSchalter     = 0;
    uint8_t inPosFlankTaster    = 0;
    
    uint8_t inSchalter0         = 0;
    uint8_t inSchalter1         = 0;
    uint8_t inSchalter2         = 0;
    uint8_t inFlank_Schalter2   = 0;
    uint8_t inTaster0           = 0;
    
    uint8_t flagSchalter0       = 0;
    uint8_t flagSchalter1       = 0;
    uint8_t flagLed2ausschalten = 0;
    uint8_t flagTaster0         = 0;
    
    uint8_t outLed0     = 0;
    uint8_t outLed1     = 0;
    uint8_t outLed2     = 0;
    uint8_t outLed7     = 0;
    
    uint32_t startZeitLed0an         = 0;
    uint32_t startZeitLed1an         = 0;
    uint32_t startZeitLed7an         = 0;
    uint32_t startZeitLed2ausschalten = 0;
    
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        
        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter  = PIN_Schalter;
        inTaster    = PIN_TASTER;
            
        inSchalter0 = (inSchalter & MASKE_IN_SCHALTER_0) > 0;
        inSchalter1 = (inSchalter & MASKE_IN_SCHALTER_1) > 0;
        inSchalter2 = (inSchalter & MASKE_IN_SCHALTER_2) > 0;
        
        //Flankenerkennung
        
        inSchalter_alt  = inSchalter_neu;
        inSchalter_neu  = inSchalter;
        
        inTaster_alt = inTaster_neu;
        inTaster_neu = inTaster;
        
        inFlankSchalter = inSchalter_alt ^ inSchalter_neu;
        inPosFlankTaster = inTaster_neu &~ inTaster_alt;
               
        inFlank_Schalter2   = (inFlankSchalter & MASKE_IN_SCHALTER_2) > 0;
        inTaster0           = (inPosFlankTaster & MASKE_IN_TASTER_0) > 0;
        
        inZeit_ms = getSystemTimeMs();      
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        //Punkt 1
        if (inSchalter0){
            flagSchalter0 = 1;}
        else {flagSchalter0 = 0; outLed0 = 0;}
             
        //Punkt 2
        if (inSchalter1){
            flagSchalter1 = 1;}
        else {flagSchalter1 = 0; outLed1 = 0;}
            
        //Punkt 3
        if (inTaster0){
            flagTaster0 = 1;}
        
            
        //Punkt 4
        if (inFlank_Schalter2){
            if(inSchalter2){
                outLed2 = 1;}
            else{flagLed2ausschalten = 1;}
                
        }
                    
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        //Punkt 1
        if (flagSchalter0)
            {
            if (inZeit_ms-startZeitLed0an >= 1000)
                {startZeitLed0an = inZeit_ms;}
                    
            if (inZeit_ms-startZeitLed0an <= 10*60)
                {outLed0 = 0;}
            else{outLed0 = 1;}
        }
        else{startZeitLed0an = inZeit_ms;}
            
        //Punkt 2
        if (flagSchalter1)
            {
            if (inZeit_ms-startZeitLed1an >= 1000){
                startZeitLed1an = inZeit_ms;}
                
            if (inZeit_ms-startZeitLed1an <= 1000*3/4){
                outLed1 = 0;}
            else{outLed1 = 1;}
                
        }
        else{startZeitLed1an = inZeit_ms;}
             
        //Punkt 3
        if (flagTaster0){
            
            outLed7 = 1;
            
            if ((inZeit_ms-startZeitLed7an) >= 3000){   
                outLed7 = 0; flagTaster0 = 0;}
            
        }
        else{
            startZeitLed7an = inZeit_ms;}
        //Punkt 4
        if (flagLed2ausschalten){
            
            if (inZeit_ms-startZeitLed2ausschalten >= 5000){
            
                outLed2 = 0;
                flagLed2ausschalten = 0;
            }
        }
        else 
            {startZeitLed2ausschalten = inZeit_ms;} 
            
        outLed =    ((outLed0 * MASKE_OUT_LED_0)    |
                    (outLed1 * MASKE_OUT_LED_1)     |
                    (outLed2 * MASKE_OUT_LED_2)     |
                    (outLed7 * MASKE_OUT_LED_7));
                    
        PORT_LED = outLed;
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

