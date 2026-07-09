/*
 * Test_SH_11_05_2026.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN
#define MASKE_IN_POS_TASTER_1 (1<<0)
//Maske_OUT
#define MASKE_OUT_LED_AUFGABE_2 (1<<0)
#define MASKE_OUT_LED_AUFGABE_3 (1<<1)
#define MASKE_OUT_LED_AUFGABE_4 (1<<2)

//Maske_Divers
#define MASKE_MAX_WERT_POTI                     (1023)
#define MASKE_SCHWELLENWERT_AUFGABE_3           (50)
#define MASKE_OBERER_SCHWELLENWERT_AUFGABE_4    (55)
#define MASKE_UNTERER_SCHWELLENWERT_AUFGABE_4   (45)

#define MASKE_AUSSCHALTZEIT_AUFGABE_2_ms (3000)

#define MASKE_ADC_100STEL   (100)
#define MASKE_ADC_5V_mV     (5000)
#define MASKE_ADC_3V_mV     (3000)
#define MASKE_ADC_2V_mV     (2000)
#define MASKE_ADC_1V_mV     (1000)

#define MASKE_ANZAHL_UNTERTEILUNGEN_AUFGABE_5   (40)
#define MASKE_MAX_WERT_AUFGABE_5                (50)
#define MASKE_MIN_WERT_AUFGABE_5                (10)

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint16_t    inADC_Aufgabe_1_3_4 = 0;
    uint16_t    inADC_Aufgabe_5     = 0;
    uint8_t     inPosTaster_1       = 0;
    uint16_t    inPoti2Spannung_mv  = 0;
    
    uint8_t inTaster        = 0;
    uint8_t inTaster_Alt    = 0;
    uint8_t inPosTaster     = 0;
    
    uint8_t outAufgabe_1 = 0;
    uint8_t outAufgabe_2 = 0;
    uint8_t outAufgabe_3 = 0;
    uint8_t outAufgabe_4 = 0;
    uint8_t outAufgabe_5 = 0;
    
    uint8_t     outLed              = 0;
    uint32_t    systemZeit_ms       = 0;
    uint32_t    ausschaltTimer_ms   = 0;
    
    
    
    //Andere Variablen
    lcdWriteText(0,4,"Prozent");
    lcdWriteText(1,0,"Wert:");
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        inTaster_Alt = inTaster;
        inTaster = buttonReadAllPL();
        inPosTaster = inTaster & ~inTaster_Alt;
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inADC_Aufgabe_1_3_4 =   adcRead (ADC_08_POTI_1);
        inADC_Aufgabe_5     =   adcRead (ADC_09_POTI_2);
        inPosTaster_1       =   (inPosTaster & MASKE_IN_POS_TASTER_1)   > 0;
        
        systemZeit_ms = getSystemTimeMs();    
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        //Aufgabe 1
        outAufgabe_1 = ((uint32_t)inADC_Aufgabe_1_3_4 * MASKE_ADC_100STEL) / MASKE_MAX_WERT_POTI;
        
        //Aufgabe 2
        if (inPosTaster_1)
        {
            outAufgabe_2 = 1;
            ausschaltTimer_ms = systemZeit_ms;
        }
        if((systemZeit_ms - ausschaltTimer_ms) >= MASKE_AUSSCHALTZEIT_AUFGABE_2_ms){
            outAufgabe_2 = 0;
        }
        
        //Aufgabe 3
        if (((uint32_t)inADC_Aufgabe_1_3_4 * MASKE_ADC_100STEL / MASKE_MAX_WERT_POTI) > MASKE_SCHWELLENWERT_AUFGABE_3)
        {
            outAufgabe_3 = 1;
        }
        else 
        {
            outAufgabe_3 = 0;
        }
        
        //Aufgabe 4
        if ((((uint32_t)inADC_Aufgabe_1_3_4 * MASKE_ADC_100STEL) / MASKE_MAX_WERT_POTI) > MASKE_OBERER_SCHWELLENWERT_AUFGABE_4)
        {
            outAufgabe_4 = 1;
        }
        if ((((uint32_t)inADC_Aufgabe_1_3_4 * MASKE_ADC_100STEL) / MASKE_MAX_WERT_POTI) < MASKE_UNTERER_SCHWELLENWERT_AUFGABE_4)
        {
            outAufgabe_4 = 0;
        }
        
        //Aufgabe 5
        inPoti2Spannung_mv = ((uint32_t)inADC_Aufgabe_5 * MASKE_ADC_5V_mV) / MASKE_MAX_WERT_POTI;
        
        if (inPoti2Spannung_mv <= MASKE_ADC_1V_mV)
        {
            outAufgabe_5 = MASKE_MIN_WERT_AUFGABE_5;
        }
        else if (inPoti2Spannung_mv >= MASKE_ADC_3V_mV)
        {
            outAufgabe_5 = MASKE_MAX_WERT_AUFGABE_5;
        }
        else
        {
            outAufgabe_5 = (((uint32_t)inPoti2Spannung_mv - MASKE_ADC_1V_mV) * MASKE_ANZAHL_UNTERTEILUNGEN_AUFGABE_5) / MASKE_ADC_2V_mV + MASKE_MIN_WERT_AUFGABE_5;
        }            
             
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        outLed =    (outAufgabe_2 * MASKE_OUT_LED_AUFGABE_2)|
                    (outAufgabe_3 * MASKE_OUT_LED_AUFGABE_3)|
                    (outAufgabe_4 * MASKE_OUT_LED_AUFGABE_4);
        ledWriteAll(outLed);
        lcdWriteText(0,0,"%3u",outAufgabe_1);
        lcdWriteText(1,6,"%2u",outAufgabe_5);    
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

