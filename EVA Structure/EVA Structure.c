/*
 * EVA Structure.c
 *
 * Created: 10/12/2025 14:24:16
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL //Nur wenn ein Delay benutzt
#include <util/delay.h>
#define  SYSTEMTAKT_ms      ([Zeit])

//Pin Configuration
#define PORT_[Funktion]     (PORT[Name])    //DDR: Eingang(Tri state/Pxn = source current if pull-up[1]) Ausgang(Output Low[0]/Output High[1])
#define DDR_[Funktion]      (DDR[Name])     //Eingang[0]/Ausgang[1]
#define Pin_[Funktion]      (PIN[Name])     //Activate Pull-up

//Maske DDR
#define ALLE_PINS_IN                (0x00)
#define ALLE_PINS_OUT               (0xFF)

#define MASKE_IN_[Typ]_[Name] //Typ Led/Schalter/RGB etc.

#define MASKE_OUT_[Typ]_[Name]

int main(void)
{
	//Board initialisieren (Ersetzt DDR wenn Treiber installiert)
	initBoard (0);
	
    //Ausgang/Eingang aktivieren
    DDR_[Function]    = [Maske DDR];
    //Pull-ups Aktivieren    
    PORT_[Function] |= [Maske Pin];
    
    //Einlese/Ausgabe Variablen
    uint8_t in[Typ]_[Place]     = 0; //Variabel um  alle Eingänge der Ports mit dem gleichen Namen einzulesen
    uint16_t out[Typ]_[Place]   = 0; //Variabel um  alle Ausgänge mit einem oder mehreren Namen anzusteuern
        
    //Eingangsvariabeln
    uint8_t in[Typ]_[Name]  = 0;
    //AusgangsVariablen
    uint8_t out[Typ]_[Name] = 0;
    
    //Andere Variablen
    uint8_t [Name]          = 0;
    
    uint8_t systemZeit_ms   = 0;
	
	//Array
	const uint8_t [Name][[anzahl Variablen]] =
	{ [Wert 0],[Wert 1],[Wert 2], ...}
	
	//zustände definieren
	typedef enum [Name]_t        {[Zustand], [Zustand] ...}             [Name]_t;
	
	[Name Zustand]_t [Name Zustand] = [Sartzustand]
    
    while (1) 
    {
        out[Typ]_[Name]= 0; //0 Normally Low/1 Normally High
        
        //Eingabe
        in[Typ]_[Place]_alt = in[Typ]_[place]_neu;                            //Flankenerkennung
        in[Typ]_[Place]_neu	= PIN_[Funktion];
        inFlanken_[Typ]_[Place]           = (in[Typ]_[Place]_alt ^ in[Typ]_[place]_neu);
        inPosFlanke_[Typ]_[Place]         = in[Typ]_[Place]_neu & ~in[Typ]_[place]_alt;
        inNegFlanke_[Typ]_[Place]         = in[Typ]_[Place]_alt & ~in[Typ]_[place]_neu;
               
        in[Typ]_[Place]    = Pin_[Funktion]; //liest alle Eingänge der Ports mit gleichen Namen ein
		
        //Schalter einlesen mit Treiber
		in[Typ]_[Place] = switchReadAll/buttonReadAllPL;
		
        in[Typ]_[Name]     = (in[Typ]_[place] & MASKE_IN_[Typ]_[Name]) > 0; // >0 -> Normalisierung macht die variabel Entweder 0/1
                                                                            // Wird Weggelassen wenn die Variabel nicht Binär sein soll. 
                                                                            // Der wert sollte aber so Verschoben werden sodass das LSB auf Bit 0 gesetzt ist
                                                                                         
        //Verarbeitung
        
		//Machen Eines Balkens
		out[Typ]_[Name] = ([Maske Balkengrösse] >> ([Maximumgrösse Balken] - [anzahl angezeigte Leds in Balken])) & [Maske Balkengrösse];
		
		//Blinken
		if (flagBlinkenRGB)
        {
            if ((systemZeit_ms - [Startzeit]) >= [Periodenzeit])
            {
                [Startzeit] = systemZeit_ms;
            }
            if ((systemZeit_ms - [Startzeit]) >= [EinZeit])
            {
                out[Typ]_[Name] = 0;
            }
            else
            {
               out[Typ]_[Name] = 1;                 
            }
        }
        else 
        {
            [Startzeit] = systemZeit_ms;
        }
		
        //Einstellen eines Delay ohne Code zu pausieren
        if((systemZeit_ms - [Startzeit]) >= [Wartezeit]){ 
            Zaehler += 1;
            [Startzeit] = systemZeit_ms;
        }        
        
        //Ausgabe
        out[Typ]_[Name] =   ((out[Typ]_[Name]   * MASKE_OUT_[Typ]_[Name])   |
                            ( out[Typ]_[Name]   * MASKE_OUT_[Typ]_[Name])   );
                        
        Port_[Funktion] = out[Typ]_[Name];
		
		//Treiber Ausgabe
		ledWriteAll(out[Typ]_[Name]);
		rgbWrite(R,G,B);
		lcdWriteText ([Zeile],[Spalte],[Text],[Zahl]); //um die Zahl hinzuzufügen muss %[Stellen]lu im Text stehen
        
        //Warten
        _delay_ms(SYSTEMTAKT_ms);
        systemZeit_ms += SYSTEMTAKT_ms;
    }
}
