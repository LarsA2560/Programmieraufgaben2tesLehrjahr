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
    
    while (1) 
    {
        out[Typ]_[Name]= 0; //0 Normally Low/1 Normally High
        
        //Eingabe
        inTaster_alt = inTaster_neu;                            //Flankenerkennung
        inTaster_neu        = PIN_TASTER;
        inFlanken           = (inTaster_alt ^ inTaster_neu);
        inPosFlanke         = inTaster_neu & inFlanken;
        inNegFlanke         = inTaster_alt & inFlanken;
        
        
        
        in[Typ]_[Place]    = Pin_[Funktion]; //liest alle Eingänge der Ports mit gleichen Namen ein
        
        in[Typ]_[Name]     = (in[Typ]_[place] & MASKE_IN_[Typ]_[Name]) > 0; // >0 -> Normalisierung macht die variabel Entweder 0/1
                                                                            // Wird Weggelassen wenn die Variabel nicht Binär sein soll. 
                                                                            // Der wert sollte aber so Verschoben werden sodass das LSB auf Bit 0 gesetzt ist
                                                                                           
        //Verarbeitung
        
        //Einstellen eines Delay ohne Code zu pausieren
        if((systemZeit_ms - [Startzeit]) >= [Wartezeit]){ 
            Zaehler += 1;
            [Startzeit] = systemZeit_ms;
        }        
        
        //Ausgabe
        out[Typ]_[Name] =   ((out[Typ]_[Name]   * MASKE_OUT_[Typ]_[Name])   |
                            ( out[Typ]_[Name]   * MASKE_OUT_[Typ]_[Name])   );
                        
        Port_[Funktion] = out[Typ]_[Name]
        
        //Warten
        _delay_ms(SYSTEMTAKT_ms);
        systemZeit_ms += SYSTEMTAKT_ms;
    }
}
//Word Lerndokumentation beispielcode
uint8_t inSchalter=0;
uint8_t inSchalter_3=0;
#define MASKE_IN_SCHALTER_3 ()


inSchalter_3  = (inSchalter & MASKE_IN_SCHALTER_3)     > 0;
    /*Ausgabe aus der Klammer wäre 8 wenn Schalter_3 == 1
    aber da 8>0 = Wahr ist macht das inSchalter_3 == 1*/

_delay_ms()