/**********************************************************************************\
* Dateiname: Timer.c
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

#include <avr/io.h>                  // ATmega2560v I/O-Definitionen




void wait50us(unsigned int u16Anz50us)
{
	TCCR0A = (1<<WGM01); // Timer in CTC Modus betreiben
	TCCR0B = (1<<CS00);  // Timer ohne Prescaler = 0.1us
	OCR0A = 40;          // Timerwert 
	TCNT0 = 0;           // Timer zurücksetzen
	
	for (; u16Anz50us >0; u16Anz50us--)
	{
		while((TIFR & (1<<OCF0A)) == 0);
		TIFR = TIFR | (1<<OCF0A);
		TCNT0 = 0;
	}
}



void wait1ms(unsigned int u16Anz1ms)
{
	TCCR0A = (1<<WGM01); // Timer in CTC Modus betreiben
	TCCR0B = (1<<CS01);  // Timer mit Prescaler 2 = 8us
	OCR0A = 122;		 // Timerwert
	TCNT0 = 0;			 // Timer zurücksetzen
	
	for (; u16Anz1ms >0; u16Anz1ms--)
	{
		while((TIFR & (1<<OCF0A)) == 0);
		TIFR = TIFR | (1<<OCF0A);
	}
}

