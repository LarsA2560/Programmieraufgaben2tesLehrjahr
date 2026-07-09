/*
* Snackautomat.c
*
* Created: 13/11/2025 15:44:45
* Author : Lars
*/

#include <avr/io.h>
#define F_CPU 16000000UL

#define PORT_SCHALTER                   (PINC)
#define DDR_SCHALTER                    (DDRC)

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define SHIFT_HIGH_BYTE                 (8)

#define ALLE_PINS_IN                    (0x00)
#define ALLE_PINS_OUT                   (0xFF)

#define MASKE_IN_PRODUKTWAHL            (0b111<<0)
#define MASKE_IN_AUSGABEKLAPPE          (1<<6)
#define MASKE_IN_BEZAHLUNG_OK           (1<<7)

#define MASKE_OUT_AUSGABEKLAPPE         (1<<0)
#define MASKE_OUT_PRODUKTWAHLANZEIGE    (0B111<<5)

#define POSITION_Produktwahl            (5)
#define POSITION_Produkt                (8)


#define FALSE                           (0)
#define TRUE                            (!FALSE)

int main(void)
{
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    
    uint8_t inschalter                  = 0;
    uint16_t outLed                     = 0;
    uint8_t flagResetMaschinensperrung  = 0;
    uint8_t flagMaschinensperrung       = 0;
    
    uint8_t inProduktwahl               = 0;
    uint8_t inAusgabeklappe             = 0;
    uint8_t inBezahlung_ok              = 0;
    
    uint8_t outProduktwahl              = 0;
    uint8_t outProdukt                  = 0;
    uint8_t outAusgabeklappe            = 0;


    while (1)
    {
        //Eingabe
        inschalter          = PORT_SCHALTER;
        inProduktwahl       = (inschalter & MASKE_IN_PRODUKTWAHL);
        inAusgabeklappe     = (inschalter & MASKE_IN_AUSGABEKLAPPE) > 0;
        inBezahlung_ok      = (inschalter & MASKE_IN_BEZAHLUNG_OK)  > 0;
        
        //Verarbeitung
        outAusgabeklappe    = inAusgabeklappe;
        
        if (inBezahlung_ok > inAusgabeklappe) //Check ob  Bezahlung Betätigt und Ausgabeklappe zu
        {
            if (!flagMaschinensperrung) //Sperre um überschreibung zu Verhindern
            {
                flagMaschinensperrung   = TRUE;                
                outProdukt              = (1<<inProduktwahl);
            }
        }        
        if (!flagMaschinensperrung) //Produktanzeige wenn Maschine aus
        {
          outProduktwahl = inProduktwahl;  
        }
        
        if (inAusgabeklappe) 
        {
            flagResetMaschinensperrung  = TRUE;
            outProdukt                  = FALSE;
        }
        
        if (flagResetMaschinensperrung > inAusgabeklappe) //Auschalten der Maschinensperrung
        {
            flagMaschinensperrung       = FALSE;
            flagResetMaschinensperrung  = FALSE;
        }
        //Ausgang
        
        
        outLed =    (outAusgabeklappe   * MASKE_OUT_AUSGABEKLAPPE)
                    |(outProduktwahl    << POSITION_Produktwahl)
                    |(outProdukt        << POSITION_Produkt);
        
        
        PORT_LED_LOW_BYTE   =   outLed;
        PORT_LED_HIGH_BYTE  =   (outLed>>SHIFT_HIGH_BYTE);
    }
}

