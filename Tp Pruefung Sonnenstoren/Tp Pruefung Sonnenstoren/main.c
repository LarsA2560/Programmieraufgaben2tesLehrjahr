/*
 * Tp_Pruefung_Sonnenstoren.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN
#define MASKE_IN_SCHALTER_POWER (1<<0)
#define MASKE_IN_SCHALTER_AUTOMATIC_MANUAL (1<<2)
#define MASKE_IN_SCHALTER_SENSOR_LIMIT_IN (1<<4)
#define MASKE_IN_SCHALTER_SENSOR_LIMIT_OUT (1<<5)
#define MASKE_IN_SCHALTER_SENSOR_SUNSHINE (1<<7)

#define MASKE_IN_TASTER_MOVE_OUT (1<<0)
#define MASKE_IN_TASTER_MOVE_IN (1<<1)

//Maske_OUT
#define MASKE_OUT_LED_POWER (1<<0)
#define MASKE_OUT_LED_AUTOMATIC (1<<2)
#define MASKE_OUT_LED_MANUAL (1<<3)
#define MASKE_OUT_LED_MOTOR_MOVE_IN (1<<6)
#define MASKE_OUT_LED_MOTOR_MOVE_OUT (1<<7)

//Maske_Divers

int main(void)
{
    //Hardware initialisieren
    initBoard(0);
    //alle variablen initialisieren und auf 0 setzen
    uint8_t inSchalter = 0;
    uint8_t inTaster = 0;
    uint8_t inTaster_alt = 0;
    uint8_t inPosTaster = 0;
    uint8_t outLed = 0;
    uint16_t systemzeit_ms = 0;
    
    uint8_t inSchalter_power = 0;
    uint8_t inSchalter_Automatic_Manual = 0;
    uint8_t inSchalter_sensor_limit_Out = 0;
    uint8_t inSchalter_sensor_limit_In = 0;
    uint8_t inSchalter_sensor_Sunshine = 0;
    
    uint8_t inTaster_move_Out = 0;
    uint8_t inTaster_move_In = 0;
    
    uint8_t outLed_Power = 0; 
    uint8_t outLed_automatic = 0;
    uint8_t outLed_manual = 0;
    uint8_t outLed_Motor_move_In = 0;
    uint8_t outLed_Motor_move_Out = 0;
    
    uint8_t flag_Blinken_Manual = 0;
    uint16_t timerBlinken = 0;
    
    //zustand_Sonnenstore definieren
    typedef enum zustand_Sonnenstore_t {AUS, AUTOMATIC, MANUAL, BEWEGEN, AUSSCHALTEN} zustand_Sonnenstore_t;
    //zustand_Sonnenstore = Aus
    zustand_Sonnenstore_t zustand_Sonnenstore = AUS;
    while (1) 
    {        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  

        //inTaster_alt = inTaster
        inTaster_alt = inTaster;
        //Schalter einlesen und inSchalter speichern
        inSchalter= switchReadAll();
        //Taster einlesen und inTaster speichern
        inTaster = buttonReadAllPL();
        //Flanken mit Bitlogik bestimmen
        inPosTaster = inTaster & ~inTaster_alt;
        
        //inSchalter_power aus inSchalter maskieren
        inSchalter_power = (inSchalter & MASKE_IN_SCHALTER_POWER)>0;
        //inSchalter_Automatic_Manual aus inSchalter maskieren
        inSchalter_Automatic_Manual = (inSchalter & MASKE_IN_SCHALTER_AUTOMATIC_MANUAL)>0;
        //inSchalter_sensor_limit_Out aus inSchalter maskieren
        inSchalter_sensor_limit_Out = (inSchalter & MASKE_IN_SCHALTER_SENSOR_LIMIT_OUT)>0;
        //inSchalter_sensor_limit_In aus inSchalter maskieren
        inSchalter_sensor_limit_In = (inSchalter & MASKE_IN_SCHALTER_SENSOR_LIMIT_IN)>0;
        //inSchalter_sensor_Sunshine aus inSchalter maskieren
        inSchalter_sensor_Sunshine = (inSchalter & MASKE_IN_SCHALTER_SENSOR_SUNSHINE)>0;
        
        //inTaster_move_In aus inPosTaster maskieren
        inTaster_move_In = (inPosTaster & MASKE_IN_TASTER_MOVE_IN)>0;
        //inTaster_move_Out aus inPosTaster maskieren
        inTaster_move_Out = (inPosTaster & MASKE_IN_TASTER_MOVE_OUT)>0;
        
        //Systemzeit in systemZeit_ms speichern
        systemzeit_ms = getSystemTimeMs();
              
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        //Powerschalter = 0 und zustand_Storen nicht Gleich aus
        if (((inSchalter_power == 0)&&(zustand_Sonnenstore!=AUS))|(zustand_Sonnenstore==AUSSCHALTEN))
        {
            //Sensor limit in = 1
            if (inSchalter_sensor_limit_In)
            {
              //zustand_Sonnenstore = AUS
              zustand_Sonnenstore = AUS;  
            }            
            else
            {
              //zustand_Sonnenstore = AUSSCHALTEN
              zustand_Sonnenstore = AUSSCHALTEN;  
            }
        }
        else {}
        //zustand_Sonnenstore
        switch (zustand_Sonnenstore)
        {
        //AUS
        case AUS:
            //outLed_Power = 0
            outLed_Power = 0;
            //outLed_automatic = 0
            outLed_automatic = 0;
            //outLed_manual = 0
            outLed_manual = 0;
            //outLed_Motor_move_In = 0
            outLed_Motor_move_In = 0;
            //outLed_Motor_move_Out = 0
            outLed_Motor_move_Out = 0;
            //flag_Blinken_Manual = 0
            flag_Blinken_Manual = 0;
            if (inSchalter_power)
            {
                zustand_Sonnenstore = AUTOMATIC;
            }
        	break;
        //AUTOMATIC
        case AUTOMATIC:
            //outLed_Power = 1
            outLed_Power = 1;
            //outLed_automatic = 1
            outLed_automatic = 1;
            //outLed_Manual = 0
            outLed_manual = 0;
            //outLed_Motor_move_In = 0
            outLed_Motor_move_In = 0;
            //outLed_Motor_move_Out = 0
            outLed_Motor_move_Out = 0;
            //flag_Blinken_Manual = 0
            flag_Blinken_Manual = 0;
            
            //inSchalter_Automatic_Manual = 0
            if (!inSchalter_Automatic_Manual)
            {
                //zustand_Sonnenstore = Manual
                zustand_Sonnenstore = MANUAL;
            }
            /*inschalter_sensor_limit_In = 1 und inSchalter_sensor_Sunshine = 1
            oder
            inschalter_sensor_limit_Out = 1 und inSchalter_sensor_Sunshine = 0*/
            if (((inSchalter_sensor_limit_In)&&(inSchalter_sensor_Sunshine))|((inSchalter_sensor_limit_Out)&&(!inSchalter_sensor_Sunshine)))
            {
                //zustand_Sonnenstore = BEWEGEN
                zustand_Sonnenstore = BEWEGEN;
            }
            break;
        //MANUAL
        case MANUAL:
            //outLed_Power = 1
            outLed_Power = 1;
            //outLed_automatic = 0
            outLed_automatic = 0;
            //outLed_Motor_move_In = 0
            outLed_Motor_move_In = 0;
            //outLed_Motor_move_Out = 0
            outLed_Motor_move_Out = 0;
            //flag_Blinken_Manual = 1
            flag_Blinken_Manual = 1;
            //inSchalter_Automatic_Manual = 1
            if (inSchalter_Automatic_Manual)
            {
                //zustand_Sonnenstore = Automatic
                zustand_Sonnenstore = AUTOMATIC;
            }
            /*inschalter_sensor_limit_In = 1 und inTaster_move_Out = 1
            oder 
            inschalter_sensor_limit_Out = 1 und inTaster_move_In = 1*/
            if ((inTaster_move_Out && inSchalter_sensor_limit_In)|(inTaster_move_In && inSchalter_sensor_limit_Out))
            {
                //zustand_Sonnenstore = BEWEGEN
                zustand_Sonnenstore = BEWEGEN;
            }
            break;
        //BEWEGEN
        case BEWEGEN:
            //outLed_Power = 1
            outLed_Power = 1;
            //outLed_Motor_move_In = 0 und outLed_Motor_move_Out = 0
            if ((!outLed_Motor_move_In)&&(!outLed_Motor_move_Out))
            {
            
                //inSchalter_sensor_limit_In = 1
                if (inSchalter_sensor_limit_In)
                {
                    //outLed_Motor_move_Out = 1
                    outLed_Motor_move_Out = 1;
                }
                //inSchalter_sensor_limit_Out = 1
                else if(inSchalter_sensor_limit_Out)
                {
                    //outLed_Motor_move_In = 1
                    outLed_Motor_move_In = 1;
                }
            }
            else
            {
                /*inSchalter_sensor_limit_In = 1 und outLed_Motor_move_In = 1
                oder
                inSchalter_sensor_limit_Out = 1 und outLed_Motor_move_Out = 1*/
                if ((inSchalter_sensor_limit_In&&outLed_Motor_move_In)|(inSchalter_sensor_limit_Out&&outLed_Motor_move_Out))
                {
                    //zustand_Sonnenstore = AUTOMATIC
                    zustand_Sonnenstore = AUTOMATIC;
                }
            }            
            break;
        //AUSSCHALTEN
        case AUSSCHALTEN:
            //outLed_Power = 1
            outLed_Power = 1;
            //outLed_automatic = 0
            outLed_automatic = 0;
            //outLed_Motor_move_In = 1
            outLed_Motor_move_In = 1;
            //outLed_Motor_move_Out = 0
            outLed_Motor_move_Out = 0;
            //flag_Blinken_Manual = 0
            flag_Blinken_Manual = 0;     
            break;
        }
        
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        //flag_Blinken_Manual
        if (flag_Blinken_Manual)
        {
            //Blinktimer 200ms vergangen
            if ((systemzeit_ms - timerBlinken) >= 200)
            {
                //timerBlinken = systemZeit_ms
                timerBlinken = systemzeit_ms;
                //outLed_Manual = 1
                outLed_manual = 1;
            }
            //Blinktimer 100ms vergangen
            if ((systemzeit_ms - timerBlinken) >= 100)
            {
                //outLed_Manual = 0
                outLed_manual = 0;
            }
        }
        else{
            //timerBlinken = systemZeit_ms
            timerBlinken = systemzeit_ms;
        }
        
        //ausgabevariablen maskieren und in outLed speichern
        outLed = ((outLed_Power * MASKE_OUT_LED_POWER)|(outLed_automatic * MASKE_OUT_LED_AUTOMATIC)|(outLed_manual * MASKE_OUT_LED_MANUAL)|(outLed_Motor_move_In * MASKE_OUT_LED_MOTOR_MOVE_IN)|(outLed_Motor_move_Out * MASKE_OUT_LED_MOTOR_MOVE_OUT));
        //outLed an den Led's ausgeben
        ledWriteAll(outLed);
        //lcdWriteText(0,0,"%u",zustand_Sonnenstore);      
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

