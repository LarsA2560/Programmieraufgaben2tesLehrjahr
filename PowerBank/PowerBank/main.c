/*
 * PowerBank.c
 *
 * Created: 21/11/2025 13:42:56
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL

//hardwarestruktur
#define PORT_LED            (PORTA)
#define DDR_LED             (DDRA)

#define PORT_SCHALTER       (PINC)
#define DDR_SCHALTER        (DDRC)

#define ALLE_PINS_IN        (0x00)
#define ALLE_PINS_OUT       (0xFF)

#define POSITION_ADC_WERT_EINGABE                       (2)
#define POSITION_LADESTAND_AUSGABE                      (4)

//Masken für die Eingänge
#define MASKE_IN_ON_OFF_SCHALTER                        (1<<0)
#define MASKE_IN_GERAET_ZUM_LADEN                       (1<<1)
#define MASKE_IN_ADC_WERT                               (0b1111 << POSITION_ADC_WERT_EINGABE)
#define MASKE_IN_POWERBANK_AN_DEN_STROM_ANGESCHLOSSEN   (1<<7)

//Masken für die Ausgänge
#define MASKE_OUT_POWER_LED             (1<<0)
#define MASKE_OUT_LADE_GERAET_LED       (1<<1)
#define MASKE_OUT_AKKUANZEIGE           (111)
#define MASKE_OUT_LADE_POWERBANK_LED    (1<<7)

//position Ladestand
#define  LADESTAND_STUFE_1          (0)
#define  LADESTAND_STUFE_2          (5)
#define  LADESTAND_STUFE_3          (10)
#define  LADESTAND_VOLL             (15)

int main(void)
{
        DDR_SCHALTER        = ALLE_PINS_IN;
        DDR_LED             = ALLE_PINS_OUT;
                
        uint8_t inschalter          = 0;
        uint16_t outLed             = 0;
        uint8_t akkuschutz          = 0;        
        
        //Variabeln für die Eingänge
        uint8_t inSchalterOnOff                         = 0;
        uint8_t inSchalterGeraetZumLaden                = 0;
        uint8_t inSchalterADCWert                       = 0;
        uint8_t inSchalterPowerbankAnStromAngeschlossen = 0;
        
        
    while (1) 
    {
        //Variabeln für die Ausgänge
        uint8_t outLedPower         = 0;    //setzt alle LED Ausgänge immer auf 0
        uint8_t outLedLadeGeraet    = 0;
        uint8_t outLedAkkuanzeige   = 0;
        uint8_t outLedLadePowerbank = 0;
        
        //Einlesen
        inschalter  = PORT_SCHALTER;
        inSchalterOnOff                         = (inschalter & MASKE_IN_ON_OFF_SCHALTER) > 0;
        inSchalterGeraetZumLaden                = (inschalter & MASKE_IN_GERAET_ZUM_LADEN) > 0;
        inSchalterADCWert                       = (inschalter & MASKE_IN_ADC_WERT) >> POSITION_ADC_WERT_EINGABE;
        inSchalterPowerbankAnStromAngeschlossen = (inschalter & MASKE_IN_POWERBANK_AN_DEN_STROM_ANGESCHLOSSEN) > 0;        
        
        //Verarbeiten
        if (inSchalterOnOff && !akkuschutz) //On-Off Schalter und Einschaltschutz wenn Akkustand=0 ist
        {
            outLedPower = 1;
            

            outLedLadeGeraet = inSchalterGeraetZumLaden;            
            if (inSchalterADCWert > LADESTAND_STUFE_1) //Linkes Bit des Balken
            {
                outLedAkkuanzeige = (1 << 2);
            }
            if (inSchalterADCWert > LADESTAND_STUFE_2) //Mittleres Bit des Balken
            {
                outLedAkkuanzeige |= (1 << 1);
            }
            if (inSchalterADCWert > LADESTAND_STUFE_3) //Rechtes Bit des Balken
            {
                outLedAkkuanzeige |= (1);
            }            
           
                        
        }
        if(!inSchalterADCWert){
            akkuschutz = 1;  // aktivierung Einschaltschutz da Akkustand=0 ist
        }        
        if (inSchalterPowerbankAnStromAngeschlossen) // deaktivierung des Einschaltschutz da Powerbank auflädt
        {
            akkuschutz = 0;
        }
        
        if (inSchalterPowerbankAnStromAngeschlossen & !(inSchalterADCWert == LADESTAND_VOLL)) //ausschalten des ladens der Powerbank da sie voll geladen ist
        {
            outLedLadePowerbank = 1;
            
        }
        
        //Ausgabe
        outLed      =   ((outLedPower * MASKE_OUT_POWER_LED)                |
                        (outLedLadeGeraet * MASKE_OUT_LADE_GERAET_LED)      |
                        (outLedAkkuanzeige << POSITION_LADESTAND_AUSGABE)   |
                        (outLedLadePowerbank * MASKE_OUT_LADE_POWERBANK_LED));
        

        PORT_LED    = outLed;    
                  
    }
}

