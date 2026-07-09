/**********************************************************************************\
* Dateiname: LED_Treiber.c
*
* Projekt  : Weihnachtsstern
* Hardware : Mocca-Board, ATmega2560v von Atmel
*
* Copyright: MSW, AE2
*
* Beschreibung:
* Stellt die Treiberfunktionen für das LED zur verfügung
*
*
* Datum:      Autor:         Version,  Grund der Änderung:
* 24.11.2017  Oliver Schneider  V1.0   Neuerstellung
*
\**********************************************************************************/

#include <avr/io.h>						// ATmega2560v I/O-Definitionen
#include "LED_Treiber.h"

//unsigned char au8SternRGB[ANZ_RGB_LED][3];	// Master LED-Array welches mit der Funktion RunLEDStripe() geschrieben wird.

struct sRGBLED au8RGBLEDs[ANZ_RGB_LED];

/**********************************************************************************
* Funktion: GetLEDArray
*
* Beschreibung:
* Diese Funktion gibt einen Pointer auf das Array zurück welches angezeigt wird.
*
* Rückgabewert:
* Pointer auf das Array
* Form au8SternRGB[NR der LED 0..45][Farbe rot=0, Grün=1, Blau=3]
*
* Parameter:
* -
*
***********************************************************************************/
struct sRGBLED* GetLEDArray(void)
{
    return(&au8RGBLEDs[0]);
}


/**********************************************************************************
* Funktion: Send8bit
*
* Beschreibung:
* Diese Funktion Sendet 8Bit seriell an das LED-Array
*
* Rückgabewert:
* -
*
* Parameter:
* u8Daten = Daten welche gesendet werden
*
***********************************************************************************/
void Send8bit(unsigned char u8Daten)
{
    USIDR = u8Daten;
    
    for (char i = 0; i < 8; i++)
    {
        
        USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(0<<USICLK);
        USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    }
}

/**********************************************************************************
* Funktion: SendLEDDaten
*
* Beschreibung:
* Mit dieser Funktion kann für jede Fabe den PWM Wert eingestellt werden.
* Insgesammt werden 24 Bit an das das Array gesendet, je 8Bit für jede Farbe
*
* Rückgabewert:
* -
*
* Parameter:
* u8Rot   = PWM Wert für Rot 0..255
* u8Gruen = PWM Wert für Grün 0..255
* u8Blau  = PWM Wert für Blau 0..255
*
***********************************************************************************/
void SendLEDDaten(unsigned char u8Rot, unsigned char u8Gruen, unsigned char u8Blau)
{
    Send8bit(u8Rot);
    Send8bit(u8Blau);
    Send8bit(u8Gruen);
}


void SetLED(unsigned char u8LEDNr, unsigned char u8Rot, unsigned char u8Gruen, unsigned char u8Blau)
{
    if (u8LEDNr < ANZ_RGB_LED)
    {
        au8RGBLEDs[u8LEDNr].u8Rot=u8Rot;
        au8RGBLEDs[u8LEDNr].u8Gruen=u8Gruen;
        au8RGBLEDs[u8LEDNr].u8Blau=u8Blau;
    }
}

void SetLedHwb(unsigned char u8LEDNr, float h, float w, float b)
{
    if (u8LEDNr < ANZ_RGB_LED)
    {
        // Normalisierung: Falls Weiss + Schwarz > 100% (1.0), skaliere beide proportional
        float ratio = w + b;
        if (ratio > 1.0f) {
            w /= ratio;
            b /= ratio;
        }

        // HWB zu HSV Konvertierung:
        // Value = 1 - Blackness
        // Saturation = 1 - (Whiteness / Value)
        float v = 1.0f - b;
        float s = (v > 0.0f) ? 1.0f - (w / v) : 0.0f;

        // HSV zu RGB Standard-Algorithmus
        float red=0, green=0, blue=0;
        if (s == 0.0f) {
            red = green = blue = v; // Graustufe
            } else {
            float hue = h / 60.0f;
            int i = (int)hue;
            float f = hue - i;
            float p = v * (1.0f - s);
            float q = v * (1.0f - s * f);
            float t = v * (1.0f - s * (1.0f - f));

            switch (i % 6) {
                case 0: red = v; green = t; blue = p; break;
                case 1: red = q; green = v; blue = p; break;
                case 2: red = p; green = v; blue = t; break;
                case 3: red = p; green = q; blue = v; break;
                case 4: red = t; green = p; blue = v; break;
                case 5: red = v; green = p; blue = q; break;
            }
        }
        au8RGBLEDs[u8LEDNr].u8Rot=red;
        au8RGBLEDs[u8LEDNr].u8Gruen=green;
        au8RGBLEDs[u8LEDNr].u8Blau=blue;
    }
}


void RunLEDStripe ()
{
    
    for (unsigned char u8LEDNr=0; u8LEDNr < ANZ_RGB_LED; u8LEDNr++)
    {
        SendLEDDaten(au8RGBLEDs[u8LEDNr].u8Rot, au8RGBLEDs[u8LEDNr].u8Gruen, au8RGBLEDs[u8LEDNr].u8Blau);
    }
    
    wait50us(6);
}


void initLED_Treiber(void)
{
    uint8_t i;
    DDRB |= (1<<PB4);//IR-LEDs auf Ausgang
    DDRB |= ((1<<PB1)|(1<<PB2)); // Cl und SD Pin auf Ausgang schalten
    for(i = 0; i < ANZ_RGB_LED; i++){
        SetLED(i,0,0,0);
    }
    RunLEDStripe();
}



void irLedOn (void){
    PORTB |= (1<<PB4);
}


void irLedOff (void){
    PORTB &= ~(1<<PB4);
}


uint8_t readIR (void){
    return !(PINB & (1<<PB3));
}



void waitForIr (void){
    while(!readIR());
    irLedOn();
    wait1ms(1);
    irLedOff();
}