/*
 * Labor_Prufung_Spannungsregler.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR
#define STEUERUNGMCU (PORTL)
#define MASKE_STEUERUNG_OUT (1<<5)

#define MASKE_10V (10000)
#define ADC_UNTERTEILUNGEN (1023)
#define BLINKPERIODE_ms (20)
#define EINHUNDERT_PROZENT (100)
#define DUTYCYCLE_PROZENT (50)
#define FAKTOR (EINHUNDERT_PROZENT/DUTYCYCLE_PROZENT)
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
    uint16_t inadc_Kondensator = 0;
    uint16_t in_SpannungKondensator_mV = 0;
    uint8_t outSpannung = 0;
    
    uint16_t systemZeit_ms = 0;
    uint16_t startBlinkzeit = 0;
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
       //Einlesen ADC Signal
       inadc_Kondensator = adcRead(ADC_00_X4_PORTF_BIT0);
       //Aktualisieren der Systemzeit
       systemZeit_ms = getSystemTimeMs();      
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        //Umrechnung des ADC Signals zur Eingangspannung
        in_SpannungKondensator_mV = (uint32_t)inadc_Kondensator*3*MASKE_10V/ADC_UNTERTEILUNGEN/2;
        //Einschalten des PWM Signals
        if (in_SpannungKondensator_mV < MASKE_10V)
        {
            //PWM Generationslogik=> Normaler BlinkCode
            if ((systemZeit_ms - startBlinkzeit) >= BLINKPERIODE_ms)
            {
                startBlinkzeit = systemZeit_ms;
            }
            if ((systemZeit_ms - startBlinkzeit) >= BLINKPERIODE_ms/FAKTOR)
            {
                STEUERUNGMCU &= ~MASKE_STEUERUNG_OUT;
            }
            else
            {
                STEUERUNGMCU |= MASKE_STEUERUNG_OUT;
            }
        }
        else
        {
            startBlinkzeit = systemZeit_ms;
        }
        //Ausschalten des FET's wenn über 10V
        if (in_SpannungKondensator_mV >= MASKE_10V)
        {STEUERUNGMCU &= ~MASKE_STEUERUNG_OUT;
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

