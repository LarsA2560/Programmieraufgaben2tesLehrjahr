/**********************************************************************************\
* Dateiname: LED_Treiber.h
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

#include "Timer.h"


#ifndef LEDTREIBER_H_
#define LEDTREIBER_H_

#define ANZ_RGB_LED 45

struct  sRGBLED
{
	unsigned char u8Rot;
	unsigned char u8Gruen;
	unsigned char u8Blau;
};


 /**********************************************************************************
 * Funktion: initLED_Treiber
 *
 * Beschreibung:
 * Diese Funktion schaltet die nötigen Ausgänge um das LED-Array anzusteuern
 *
 * Rückgabewert:
 * -
 *
 * Parameter:
 * -
 *
 ***********************************************************************************/
void initLED_Treiber(void);

 /**********************************************************************************
 * Funktion: SetLED
 *
 * Beschreibung:
 * Mit dieser Funktion kann jedes einzelne LED im Array vorbestimmt werden.
 *
 * Rückgabewert:
 * -
 *
 * Parameter:
 * u8Nr    = LED Nummer im Array
 * u8Rot   = PWM Wert für Rot 0..255
 * u8Gruen = PWM Wert für Grün 0..255
 * u8Blau  = PWM Wert für Blau 0..255
 *
 ***********************************************************************************/ 
void SetLED(unsigned char u8LEDNr, unsigned char u8Rot, unsigned char u8Gruen, unsigned char u8Blau);

/**
 * \brief setzt eine LED auf eine Farbe aus dem HWB-Farbraum
 * 
 * \param u8LEDNr
 * \param h Hue
 * \param w Whiteness
 * \param b Blackness
 * 
 * \return void
 */
void SetLedHwb(unsigned char u8LEDNr, float h, float w, float b);

  /**********************************************************************************
 * Funktion: RunLEDStripe
 *
 * Beschreibung:
 * Mit  dieser Funktion wird der ganze LED-Stripe (Stern) geschrieben.
 *
 * Rückgabewert:
 * -
 *
 * Parameter:
 * -
 *
 ***********************************************************************************/ 
void RunLEDStripe (void);

/**
 * \brief IR-LED einschalten
 * 
 * \param void
 * 
 * \return void
 */
void irLedOn (void);

/**
 * \brief IR-LED ausschalten
 * 
 * \param void
 * 
 * \return void
 */
void irLedOff (void);

/**
 * \brief IR-Sensor einlesen
 * 
 * \param void
 * 
 * \return uint8_t WAHR wenn ein IR-Signal erkannt wurde
 */
uint8_t readIR (void);

/**
 * \brief   Wartet bis ein IR-Signal empfangen wird und gibt einen IR-Impuls weiter.
 *          Eignet sich um mehrere Sterne zu synchronisieren, indem alle auf ein
 *          IR-Signal warten und dann gleichzeitig starten.
 * 
 * \param void
 * 
 * \return void
 */
void waitForIr (void);

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
struct sRGBLED* GetLEDArray(void);
 
#endif /* LEDTREIBER_H_ */