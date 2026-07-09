/**********************************************************************************\
* Dateiname: Timer.h
*
* Projekt  : Weihnachtsstern
* Hardware : Mocca-Board, ATmega2560v von Atmel
*
* Copyright: MSW, AE2
*
* Beschreibung:
* Stellt die Timerfunktionen zur Verfügung
*
*
* Datum:      Autor:         Version,  Grund der Änderung:
* 24.11.2017  Oliver Schneider  V1.0   Neuerstellung
*
\**********************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

/**********************************************************************************
* Funktion: wait50us
*
* Beschreibung:
* Diese Funktion wartet 50us
*
* Rückgabewert:
* -
*
* Parameter:
* Anzahl mal 50us (16bit)
*
***********************************************************************************/
void wait50us(unsigned int u16Anz50us);

/**********************************************************************************
* Funktion: wait1ms
*
* Beschreibung:
* Diese Funktion wartet 1ms
*
* Rückgabewert:
* -
*
* Parameter:
* Anzahl mal 1ms (16bit)
*
***********************************************************************************/
void wait1ms(unsigned int u16Anz1ms);

#endif /* TIMER_H_ */