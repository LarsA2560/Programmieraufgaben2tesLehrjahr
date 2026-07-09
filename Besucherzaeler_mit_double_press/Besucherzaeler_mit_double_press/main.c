/*
 * Besucherzaeler_mit_double_press.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration
#define PORT_LED_LOW_BYTE   (PORTA)
#define DDR_LED_LOW_BYTE    (DDRA)

#define PORT_LED_HIGH_BYTE  (PORTB)
#define DDR_LED_HIGH_BYTE   (DDRB)

#define PIN_TASTER  (PINL)
#define DDR_TASTER  (DDRL)
//Maske DDR

//Maske_IN
#define MASKE_IN_TASTER (1<<0)

//Maske_OUT

//Maske_Divers
#define LONG_PRESS_DURATION_ms  (500)
#define DOUBLE_PRESS_TIME_ms    (100)


int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint16_t outLed         = 0;
    uint8_t inTaster        = 0;
    uint16_t inZeit_ms      = 0;
    
    uint8_t inTaster_neu    = 0;
    uint8_t inTaster_alt    = 0;
    
    uint8_t inFlank     = 0;
    uint8_t inPosFlanke = 0;
    uint8_t inNegFlanke = 0;
    
    uint8_t inBtnPressed    = 0;
    uint8_t inBtnPosFlank   = 0;
    uint8_t inBtnNegFlank   = 0;
    
    uint8_t inBtnLongPress          = 0;
    uint8_t inBtnShortPress         = 0;
    uint8_t inBtnDoublePress        = 0;
    uint8_t inBtnSingleShortPress   = 0;
    
    //Andere Variablen
    
    uint16_t startZeitButtonPress       = 0;
    uint16_t startZeitGapDoublePress    = 0;
    
    uint8_t flagBtnLongPress            = 0;
    uint8_t flagBtnShortPress           = 0;
    uint8_t flagBtnDoublePress          = 0;
    uint8_t flagbtnDoublePressActive    = 0;
    
    uint16_t anzahlBesucher_Besucht = 0;
    uint16_t anzahlBesucher_Verlassen = 0;
    
    lcdWriteText(0,0,"AnzahlBesucher");
    lcdWriteText(1,0,"Besucht");
    lcdWriteText(2,0,"Verlassen");
   
    while (1) 
    {
        //Variablem Always Low/High
        inBtnLongPress          = 0;
        inBtnShortPress         = 0;
        inBtnDoublePress        = 0;
        inBtnSingleShortPress   = 0;
        
        //Flankenerkennung
       inTaster = PIN_TASTER;
       
       inTaster_alt = inTaster_neu;
       inTaster_neu = inTaster;
       
       inFlank      = inTaster_neu ^ inTaster_alt;
       inPosFlanke  = inTaster_neu &~ inTaster_alt;
       inNegFlanke  = ~inTaster_neu & inTaster_alt;       
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inBtnPressed    = (inTaster & MASKE_IN_TASTER) > 0;
        inBtnPosFlank   = (inPosFlanke & MASKE_IN_TASTER) > 0;
        inBtnNegFlank   = (inNegFlanke & MASKE_IN_TASTER) > 0; 
        
        inZeit_ms = getSystemTimeMs();     
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        if (inBtnPosFlank)
        {
            startZeitButtonPress = inZeit_ms;
            flagBtnLongPress = 0;
        }
        if(inBtnPressed){
            if (inZeit_ms-startZeitButtonPress >= LONG_PRESS_DURATION_ms) //Long Press aktivation
            { 
                inBtnLongPress = 1;
                flagBtnLongPress = 1;
            }
        }        
        if (inBtnNegFlank) 
        {
            
            if(!flagBtnLongPress){
                inBtnShortPress     = 1;
                flagBtnShortPress   = 1;
            }
            if (flagBtnDoublePress && !flagBtnLongPress) 
            {
                inBtnDoublePress = 1;
                flagBtnDoublePress = 0;
                flagBtnShortPress = 0;  
            }
            
                      
            
        }
        if (!flagBtnShortPress) //zeit start
        {
             startZeitGapDoublePress = inZeit_ms;
        }
       
        if (inZeit_ms-startZeitGapDoublePress <= DOUBLE_PRESS_TIME_ms)//Double Press erkennung
        {
            if (inBtnPosFlank && flagBtnShortPress)
            {
                flagBtnDoublePress = 1;
                flagbtnDoublePressActive = 1;
                
            }
        } 
        if (inZeit_ms-startZeitGapDoublePress >= DOUBLE_PRESS_TIME_ms) //Single short Press activation
        {
            
            if(!flagbtnDoublePressActive){
                inBtnSingleShortPress = 1;
                
            }
            flagBtnShortPress = 0; 
            flagbtnDoublePressActive = 0;                                   
        }
                     
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
         if (inBtnSingleShortPress)
         {
             outLed += 1;
             anzahlBesucher_Besucht += 1;
         }
         if (inBtnDoublePress && outLed) //outled => überlaufsschutz
         {
             outLed -= 1;
             anzahlBesucher_Verlassen += 1;
         }
         if (inBtnLongPress)
         {
             outLed = 0;
             anzahlBesucher_Besucht = 0;
             anzahlBesucher_Verlassen = 0;
         }
     
         PORT_LED_LOW_BYTE   = outLed;
         PORT_LED_HIGH_BYTE  = (outLed>>8);
         lcdWriteZahl(0,15,outLed,4,0);
         lcdWriteZahl(1,15,anzahlBesucher_Besucht,4,0);
         lcdWriteZahl(2,15,anzahlBesucher_Verlassen,4,0);              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

