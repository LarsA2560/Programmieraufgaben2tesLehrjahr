/*********************************************************************************\
*
* MMMMMMMMMMMM   SSSSSSSSSSSS   WW   WW   WW   MECHATRONIK
* MM   MM   MM   SS             WW   WW   WW   SCHULE
* MM   MM   MM   SSSSSSSSSSSS   WW   WW   WW   WINTERTHUR
* MM   MM   MM             SS   WW   WW   WW   
* MM   MM   MM   SSSSSSSSSSSS   WWWWWWWWWWWW   www.msw.ch
*
*
* Dateiname: main.c
*
* Projekt  : Muster
* Hardware : uC-Board, ATmega2560v von Atmel
*
* Copyright: MSW, E2
*
* Beschreibung:
* =============
* Siehe Prüfungsaufgabe
*
* Portbelegung:
* =============
* Port C:   Schalter
* Port A:   LEDs
* für genauere Beschreibung siehe Hardwarestruktur
*
* Verlauf:
* ========
* Datum:      Autor:        Version     Grund der Änderung:
* 11.06.2026  L.Märki       V1.0        Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"

//Konstanten
#define MASK_IN_SWITCH_DOOR_CLOSED (1<<0)
#define  MASK_IN_SWITCH_KEYSWITCH_MODE (1<<1)
#define MASK_IN_SWITCH_keyboard_code (0b1111<<4)
#define MASK_IN_BUTTON_KEYBOARD_ENTER (1<<0)

#define MASK_OUT_LED_MODE (1<<0)
#define MASK_OUT_LED_UNLOCK (1<<1)
#define MASK_OUT_LED_HUNDREDS (1<<2)
#define MASK_OUT_LED_TENS (1<<3)
#define MASK_OUT_LED_ONES (1<<4)
#define MASK_OUT_LED_SILENT_ALARM (1<<5)
#define MASK_OUT_LED_CODE_OK (1<<6)

#define MASK_CODE_RESTART (999)

//Hauptprogramm
int main(void)
{ 
    //Hardware initialisieren
    initBoard(0);
    //Alle Variablen Initialisieren und auf 0 setzen
    uint8_t inSchalter = 0;
    uint8_t inButton = 0;
    uint8_t inButton_old = 0;
    uint8_t inPosFlanke = 0;
    uint8_t inSwitch_door_closed = 0;
    uint8_t inSwitch_keyswitch_mode = 0;
    uint8_t inSwitch_keyboard_code = 0;
    uint8_t inButton_keyboard_enter = 0;
    
    uint8_t outLed = 0;
    uint8_t outLed_mode = 0;
    uint8_t outLed_unlock = 0;
    uint8_t outLed_hundreds = 0;
    uint8_t outled_tens = 0;
    uint8_t outLed_ones = 0;
    uint8_t outLed_silent_alarm = 0;
    uint8_t outLed_code_ok = 0;
    
    uint16_t systemZeit_ms = 0;
    uint16_t code_gespeichert = 0;
    uint8_t eingabe_Einer = 0;
    uint8_t eingabe_Zehner = 0;
    uint8_t eingabe_Hunderter = 0;
    uint8_t mode_alt = 0;
    uint8_t flag_Alarm = 0;
    uint16_t blinktimer = 0;
    
    //zustand_Safe definieren
    typedef enum zustand_Safe_t {BETRIEB,SILENT_ALARM,EINSTELLEN,TURE_GEOFFNET} zustand_Safe_t;
    //zustand_Safe = BETRIEB
    zustand_Safe_t zustand_Safe = BETRIEB;
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        //inButton in inButton_old speichern
        inButton_old = inButton;
        //Schalter einlesen und in inSchalter Speichern
        inSchalter = switchReadAll();
        //Taster einlesen und in inButton spaichern
        inButton = buttonReadAllPL();
        //Pos Flanke Taster mit Bitlogik bestimmen
        inPosFlanke = inButton & ~inButton_old;
        
        //inSwitch_door_closed aus inSchalter maskieren
        inSwitch_door_closed = (inSchalter & MASK_IN_SWITCH_DOOR_CLOSED)>0;
        //inSwitch_keyswitch_mode aus inSchalter maskieren
        inSwitch_keyswitch_mode = (inSchalter & MASK_IN_SWITCH_KEYSWITCH_MODE)>0;
        //inSwitch_keyboard_code aus inSchalter maskieren
        inSwitch_keyboard_code = (inSchalter & MASK_IN_SWITCH_keyboard_code)>>4;
        //inButton_keyboard_enter aus inPosFlanke maskieren
        inButton_keyboard_enter = (inPosFlanke & MASK_IN_BUTTON_KEYBOARD_ENTER)>0;
        //systemzeit in systemZeit_ms speichern
        systemZeit_ms = getSystemTimeMs();
        //code_gespeichert = 999
        code_gespeichert = MASK_CODE_RESTART;
    
        //Verarbeitung-------------------------------------------------------------
        //mode_alt nicht = inSwitch_keyswitch_mode
        if (mode_alt == !inSwitch_keyswitch_mode)
        {
//             outled_hundreds = 0
//             outled_tens = 0
//             outLed_ones = 0
//             flag_Alarm = 0
            outLed_hundreds = 0;
            outled_tens = 0;
            outLed_ones = 0;
            flag_Alarm = 0;
        }
        //mode_alt = inSwitch_keyswitch_mode
        mode_alt = inSwitch_keyswitch_mode;
//         outLed_hundreds
//         und outLed_tens
//         und outLed_ones
//         und outled_code_ok
//         und outLed_silent_alarm
//         alle nicht an
        if (~(outLed_hundreds|outled_tens|outLed_ones|outLed_code_ok|outLed_silent_alarm))
        {
            //outled_hundreds = 1
            outLed_hundreds = 1;
        }
        
        //zustand_Safe
        switch (zustand_Safe)
        {
        //Betrieb
        case BETRIEB:
//         outLed_mode = 1
//         outLed_unlock = 0
//         outLed_silent_alarm = 1
//         outLed_code_ok = 0

        outLed_mode = 1;
        outLed_unlock = 0;
        outLed_silent_alarm = 1;
        outLed_code_ok = 0;
        
        //inSwitch_keyswitch_mode = 1
        if (inSwitch_keyswitch_mode)
        {
            //zustand_Safe = EINSTELLEN
            zustand_Safe = EINSTELLEN;
        }
        
        	break;
        //SILENT_ALARM
        case SILENT_ALARM:
//         outLed_mode = 1
//         outLed_unlock = 0
//         outLed_hundreds = 0
//         outled_tens = 0
//         outLed_ones = 0
//         outLed_silent_alarm = 0
//         outLed_code_ok = 0
        outLed_mode = 1;
        outLed_unlock = 0;
        outLed_hundreds = 0;
        outled_tens = 0;
        outLed_ones = 0;
        outLed_silent_alarm = 0;
        outLed_code_ok = 0;
            break;
        //EINSTELLEN
        case EINSTELLEN:
//         outLed_mode = 0
//         outLed_unlock = 0
//         outLed_silent_alarm = 1
//         outLed_code_ok = 0
        outLed_unlock = 0;
        outLed_silent_alarm = 1;
        outLed_code_ok = 0;
        
        //inSwitch_keyswitch_mode = 0
        if (!inSwitch_keyswitch_mode)
        {
            //zustand_Safe = BETRIEB
            zustand_Safe = BETRIEB;
        }
            break;
        //TURE_GEOFFNET
        case TURE_GEOFFNET:
        outLed_mode = 1;
        outLed_unlock = 1;
        outLed_silent_alarm = 1;
            break;
        }
                
        //inButton_keyboard_enter = 1
        if (inButton_keyboard_enter)
        {
            //inswitch_keyboard_code >= 10
            if (inSwitch_keyboard_code >= 10)
            {
//                 outLed_hundreds = 1
//                 outled_tens = 0
//                 outLed_ones = 0
                outLed_hundreds = 1;
                outled_tens = 0;
                outLed_ones = 0;
            } 
            else{           
                //outLed_hundreds = 1
                if (outLed_hundreds)
                {
                    //outled_tens = 1
                    outled_tens = 1;
                    //outLed_hundreds = 0
                    outLed_hundreds = 0;
                    //eingabe_Hunderter = inSwitch_keyboard_code
                    eingabe_Hunderter = inSwitch_keyboard_code;
                }
                //outLed_tens = 1
                else if (outled_tens)
                {
                    //outLed_ones = 1
                    outLed_ones = 1;
                    //outLed_tens = 0
                    outled_tens = 0;
                    //eingabe_Zehner = inSwitch_keyboard_code
                    eingabe_Zehner = inSwitch_keyboard_code;
                }
                //outLed_ones = 1
                else if (outLed_ones)
                {
                    //outLed_ones
                    outLed_ones = 0;
                    //eingabe_Einer = inSwitch_keyboard_code
                    eingabe_Einer = inSwitch_keyboard_code;
                    //inSwitch_keyswitch_mode = 1
                    if (zustand_Safe == BETRIEB)
                    {
                        //code_gespeichert =
                        //eingabe_Hunderter * 100 + eingabe_Zehner * 10 + eingabe_Einer * 1
                        if (code_gespeichert == ((eingabe_Hunderter * 100)|(eingabe_Zehner * 10)|(eingabe_Einer * 1)))
                        {
                            //zustand_Safe = TURE_GEOFFNET
                            zustand_Safe = TURE_GEOFFNET;
                        }
                        else{
                            //flag_Alarm = flag_Alarm + 1
                            flag_Alarm = flag_Alarm + 1;
                            
                        }
                    }
                    else
                    {
//                         eingabe_Hunderter * 100 + eingabe_Zehner * 10 + eingabe_Einer * 1
//                         in code_gespeichert speichern
                        code_gespeichert = ((eingabe_Hunderter * 100)|(eingabe_Zehner * 10)|(eingabe_Einer * 1));
                    }
                }
            }            
                        
        }
        //flag_Alarm > 1
        if (flag_Alarm > 1)
        {
            //zustand_Safe = SILENT_ALARM
            zustand_Safe = SILENT_ALARM;
        }
        //inSwitch_keyswitch_mode = 0
        if (inSwitch_keyswitch_mode)
        {
            //blinktimer 500 ms vergangen
            if ((systemZeit_ms-blinktimer)>= 500)
            {
                //outLed_mode nicht gleich outLed_mode
                outLed_mode = !outLed_mode;
                //blinktimer =systemZeit_ms
                blinktimer =systemZeit_ms;
            }
        }
        else
        {
            //blinktimer =systemZeit_ms
            blinktimer =systemZeit_ms;
        }
        
        //Ausgabe------------------------------------------------------------------
        outLed = ((outLed_mode*MASK_OUT_LED_MODE)|(outLed_unlock*MASK_OUT_LED_UNLOCK)|(outLed_hundreds*MASK_OUT_LED_HUNDREDS)|(outled_tens*MASK_OUT_LED_TENS)|(outLed_ones*MASK_OUT_LED_ONES)|(outLed_silent_alarm*MASK_OUT_LED_SILENT_ALARM)|(outLed_code_ok*MASK_OUT_LED_CODE_OK));
        ledWriteAll(outLed);
    }
}

