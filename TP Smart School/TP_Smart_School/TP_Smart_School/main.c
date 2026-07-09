/*
 * TP_Smart_School.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"
#define MASK_IN_SENSOR_PRESENCE_DETECTOR (1<<0)
#define MASK_IN_SENSOR_CLOCK (1<<1)
#define MASK_IN_BUTTON_CLASSROOM (1<<0)
#define MASK_IN_BUTTON_CARETAKER (1<<7)

#define MASK_OUT_LED_LIGHTING (1<<0)
#define MASK_OUT_LED_PRESENCE_DETECTOR (1<<1)
#define MASK_OUT_LED_LED_SENSOR_CLOCK (1<<2)
#define MASK_OUT_LED_MANUAL (1<<3)
#define MASK_OUT_LED_AUTOMATIC (1<<4)
#define MASK_OUT_LED_STATUS (1<<5)

#define MASK_TIMER_2_SEKUNDEN (2000)
#define MASK_TIMER_500_MILISEKUNDEN (500)
int main(void)
{
    /*Hardware initialisieren*/
    initBoard(0);
//     alle Variablen initialisieren und auf 0 setzen
    uint8_t inTaster = 0;
    uint8_t inTaster_alt = 0;
    uint8_t inTasterPosFlanke = 0;
    uint8_t inTasterNegFlanke = 0;    
    uint8_t inSchalter = 0;
    uint8_t outLed = 0;
    
    uint8_t sensor_presence_detector = 0;
    uint8_t sensor_clock = 0;
    uint8_t button_classroomPosFlank = 0;
    uint8_t button_classroomNegFlank = 0;
    uint8_t button_caretaker = 0;
    
    uint8_t outLed_lighting = 0;
    uint8_t outLed_presence_detector = 0;
    uint8_t outLed_sensor_clock = 0;
    uint8_t outLed_manual = 0;
    uint8_t outLed_automatic = 0;
    uint8_t outLed_status = 0;
    
    uint16_t systemZeit_ms = 0;
    uint16_t start_buttonPress = 0;
    uint16_t blinktimer = 0;
//     zustand_Klassenzimmer definieren
    typedef enum zustand_Klassenzimmer_t {AUTOMATISCH_AUS,AUTOMATISCH_EIN,MANUELL} zustand_Klassenzimmer_t; 
//     zustand_Klassenzimmer = AUTOMATIC_AUS
    zustand_Klassenzimmer_t zustand_Klassenzimmer = AUTOMATISCH_AUS; 
//     unendlichschlaufe  
    while (1) 
    {

        //Eingabe____________________________________________________________________________________________________________________________________________________________  
//         inTaster in inTaster_alt speichern
        inTaster_alt = inTaster;
//         Taster einlesen und in inTaster speichern
        inTaster = buttonReadAllPL();
//         inTasterPosFlanke mit Bitlogik bestimmen
        inTasterPosFlanke = inTaster & ~inTaster_alt;
//         inTasterNegFlanke mit Bitlogik bestimmen
        inTasterNegFlanke = inTaster_alt & ~inTaster;        
//         Schalter einlesen und in inSchalter speichern
        inSchalter = switchReadAll();
//         
//         sensor_presence_detector aus inSchalter Maskieren
        sensor_presence_detector = (inSchalter & MASK_IN_SENSOR_PRESENCE_DETECTOR)>0;
//         sensor_clock aus inSchalter Maskieren
        sensor_clock = (inSchalter & MASK_IN_SENSOR_CLOCK)>0;
//         button_classroomPosFlank aus inTasterPosFlanke Maskieren
        button_classroomPosFlank = (inTasterPosFlanke & MASK_IN_BUTTON_CLASSROOM)>0;
//         button_classroomNegFlank aus inTasterNegFlanke Maskieren
        button_classroomNegFlank = (inTasterNegFlanke & MASK_IN_BUTTON_CLASSROOM)>0;
//         button_caretaker aus inTasterPosFlanke Maskieren
        button_caretaker = (inTasterPosFlanke &MASK_IN_BUTTON_CARETAKER)>0;
//      Systemzeit in systemzeit_ms speichern        
        systemZeit_ms = getSystemTimeMs();
              
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
//         button_classroomPosFlank = 1
        if(button_classroomPosFlank){
//             start_buttonPress = systemZeit_ms
            start_buttonPress = systemZeit_ms; 
        }   
//         button_classroomNegFlank = 1
        if(button_classroomNegFlank){
//             Tasterdruck länger als 2 Sekunden
            if((systemZeit_ms-start_buttonPress) >= MASK_TIMER_2_SEKUNDEN){
//             zustand_Klassenzimmer = MANUELL
                zustand_Klassenzimmer = MANUELL;                
            }
            else {           
//              zustand_Klassenzimmer = AUTOMATISCH_AUS
//              und
//              sensor_presence_detector = 1
//              sensor_clock = 1
                if ((zustand_Klassenzimmer == AUTOMATISCH_AUS)&& sensor_presence_detector&& sensor_clock)
                {
//                  zustand_Klassenzimmer = AUTOMATISCH_EIN
                    zustand_Klassenzimmer = AUTOMATISCH_EIN;
                }
                else{                
//                  zustand_Klassenzimmer = AUTOMATISCH_AUS
                    zustand_Klassenzimmer = AUTOMATISCH_AUS;
                }                
            }            
        }        
//         
//         zustand_Klassenzimmer
           switch (zustand_Klassenzimmer)
           {
           
//             AUTOMATISCH_AUS
                case AUTOMATISCH_AUS:
//                 outLed_lighting = 0
                    outLed_lighting = 0;
//                 outLed_manual = 0
                    outLed_manual = 0;
//                 outLed_automatic = 1
                    outLed_automatic = 1;
//                 outLed_status = 0
                    outLed_status = 0;
               break;
//         
//             AUTOMATISCH_EIN
                case AUTOMATISCH_EIN:
//                 outLed_lighting = 1
                    outLed_lighting = 1;
//                 outLed_manual = 0
                    outLed_manual = 0;
//                 outLed_automatic = 1
                    outLed_automatic = 1;
//                 outLed_status = 0
                    outLed_status = 0;
//                 
//                 sensor_presence_detector = 0
//                 oder
//                 sensor_clock = 0
                    if((!sensor_presence_detector)|(!sensor_clock)){
//                     zustand_Klassenzimmer = AUTOMATISCH_AUS
                        zustand_Klassenzimmer = AUTOMATISCH_AUS;
                    }                    
                break;         
//             MANUELL
                case MANUELL:
//                 outLed_lighting = 1
                    outLed_lighting = 1;
//                 outLed_manual = 1
                    outLed_manual = 1;
//                 outLed_automatic = 0
                    outLed_automatic = 0;
//                 
//                 sensor_presence_detector = 0
//                 und
//                 sensor_clock = 0
                    if((!sensor_presence_detector)&&(!sensor_clock)){
//                     button_caretaker = 1 
                        if(button_caretaker){   
//                         zustand_Klassenzimmer = AUTOMATISCH_AUS
                            zustand_Klassenzimmer = AUTOMATISCH_AUS;
                        }                        
//                     Blinktimer 500ms vergangen
                        if((systemZeit_ms-blinktimer) >= MASK_TIMER_500_MILISEKUNDEN){
//                          outLed_status gleich nicht outLed_status
                            outLed_status = (!outLed_status);
//                          blinktimer = systemZeit_ms
                            blinktimer = systemZeit_ms;
                        }                            
                    }                    
                    else {
//                 blinktimer = systemZeit_ms
                    blinktimer = systemZeit_ms;
//                 outLed_status = 0
                    outLed_status = 0;
                    }                    
                break;
            }        
//         outLed_presence_detector = sensor_presence_detector
        outLed_presence_detector = sensor_presence_detector;
//         outLed_sensor_clock = sensor_clock
        outLed_sensor_clock = sensor_clock;
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
//         Ausgabevariablen Maskieren und in outLed Speichern
        outLed = (outLed_lighting           * MASK_OUT_LED_LIGHTING)
                |(outLed_presence_detector  * MASK_OUT_LED_PRESENCE_DETECTOR)
                |(outLed_sensor_clock       * MASK_OUT_LED_LED_SENSOR_CLOCK)
                |(outLed_manual             * MASK_OUT_LED_MANUAL)
                |(outLed_automatic          * MASK_OUT_LED_AUTOMATIC)
                |(outLed_status             * MASK_OUT_LED_STATUS); 
//         outLed an den Led's ausgeben 
        ledWriteAll(outLed);    
    }
}

