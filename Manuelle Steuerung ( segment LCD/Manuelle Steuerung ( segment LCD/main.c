/*
 * Manuelle_Steuerung___segment_LCD.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration
#define PORT_OUT_7_SEGMENT     (PORTB)
#define DDR_OUT_7_SEGMENT      (DDRB)

#define PIN_SCHALTER            (PINC)
#define DDR_SCHALTER            (DDRC)

#define PIN_TASTER            (PINL)
#define DDR_TASTER            (DDRL)
//Maske DDR
#define ALLE_PINS_IN    (0x00)
#define ALLE_PINS_OUT   (0xFF)
//Maske_IN

//Maske_OUT

//Maske_Divers

int main(void)
{
    //DDR Aktivieren
        DDR_OUT_7_SEGMENT   = ALLE_PINS_OUT;
        DDR_SCHALTER        = ALLE_PINS_IN;
        DDR_TASTER          = ALLE_PINS_IN;
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
        uint8_t outLed      = 0;
        uint8_t inSchalter  = 0;
        uint8_t inTaster    = 0;
        
        uint8_t inSchalter_DS   = 0;
        uint8_t inSchalter_Anode   = 0;
        uint8_t inTaster_CLK   = 0;
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter   = PIN_SCHALTER;
        inTaster     =PIN_TASTER;
        
        inSchalter_DS   = (inSchalter & (1<<0)) > 0;
        inSchalter_Anode    = (inSchalter & (111<<5))>>5; 
        inTaster_CLK        = (inTaster & (1<<0))>0;                    
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        /*
        A   15  Q
        B   14  Q
        C   13  Q
        D   12
        E   11  Q
        F   10  Q
        G   9
        H   8
        J   7   Q
        K   6
        L   5
        M   4
        N   3   Q
        P   2
        .   1
            
        */     
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        outLed =  (inTaster_CLK * (1<<0))|(inSchalter_DS * (1<<1))|(inSchalter_Anode<<3);
        PORT_OUT_7_SEGMENT = outLed;     
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

