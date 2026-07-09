/*
 * Snackautomat der Zustandsautomant.c
 *
 * Created: 14/11/2025 15:29:59
 * Author : Lars
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define PORT_SCHALTER                   (PINC)
#define DDR_SCHALTER                    (DDRC)

#define PORT_LED_LOW_BYTE               (PORTA)
#define DDR_LED_LOW_BYTE                (DDRA)

#define PORT_LED_HIGH_BYTE              (PORTB)
#define DDR_LED_HIGH_BYTE               (DDRB)

#define PORT_TASTER                     (PINL)
#define DDR_TASTER                      (DDRL)

#define ALLE_PINS_IN                    (0x00)
#define ALLE_PINS_OUT                   (0xFF)
#define RGB_OUT                         (111<<3)

#define MASKE_IN_PRODUKTWAHL            (0b111<<0)
#define MASKE_IN_AUSGABEKLAPPE          (1<<7)
#define MASKE_IN_BEZAHLUNG_OK           (1<<7)

#define MASKE_OUT_AUSGABEKLAPPE         (1<<0)
#define MASKE_OUT_PRODUKTWAHLANZEIGE    (0B111<<5)

#define PORT_RGB                        (PORTH)
#define DDR_RGB                         (DDRH)

#define SHIFT_HIGH_BYTE                 (8)
#define SHIFT_RGB                       (3)

#define ROT                             (1)
#define GRUEN                           (1<<1)
#define BLAU                            (1<<2)

#define POSITION_Produktwahl            (5)
#define POSITION_Produkt                (8)

#define AUS                             (0)
#define EIN                             (!AUS)


typedef enum zustand_t {WAHL, SPIRALE, ENTNAHME}
zustand_t;

int main(void)
{
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_LED_LOW_BYTE    = ALLE_PINS_OUT;
    DDR_LED_HIGH_BYTE   = ALLE_PINS_OUT;
    DDR_TASTER          = ALLE_PINS_IN;
    DDR_RGB             = RGB_OUT;
    
    uint8_t inschalter                  = 0;
    uint8_t intaster                    = 0;
    uint16_t outLed                     = 0;
    zustand_t zustand                   = WAHL;

    
    uint8_t inProduktwahl               = 0;
    uint8_t inAusgabeklappe             = 0;
    uint8_t inBezahlung_ok              = 0;
    uint8_t inProduktwahlbeiBezahlung   = 0;
    
    uint8_t outProduktwahl              = 0;
    uint8_t outProdukt                  = 0;
    uint8_t outAusgabeklappe            = 0;
    uint8_t outRGB                      = 0;
    

    while (1)
    {
        //Eingabe
        inschalter          = PORT_SCHALTER;
        intaster            = PORT_TASTER;
        inProduktwahl       = (inschalter   & MASKE_IN_PRODUKTWAHL);
        inAusgabeklappe     = (inschalter   & MASKE_IN_AUSGABEKLAPPE) > 0;
        inBezahlung_ok      = (intaster     & MASKE_IN_BEZAHLUNG_OK)  > 0;
        outRGB              = 0;
        
        //Verarbeitung
        switch (zustand)
        {
        case WAHL:
            outProdukt          = AUS;
            outProduktwahl      = inProduktwahl;
            outAusgabeklappe    = AUS;
            outRGB              = ROT;
            
            if (!inAusgabeklappe && inBezahlung_ok)
            {
                zustand = SPIRALE;
                inProduktwahlbeiBezahlung = inProduktwahl;
                
            }
        	break;
            
        case SPIRALE:
            outProdukt          = (1<<inProduktwahlbeiBezahlung);
            outProduktwahl      = inProduktwahlbeiBezahlung;
            outAusgabeklappe    = AUS;
            outRGB              = BLAU;
 
            if (inAusgabeklappe)
            {
                zustand = ENTNAHME;
            }                    
            break;
            
        case ENTNAHME:
            outProdukt          = AUS;
            outProduktwahl      = inProduktwahlbeiBezahlung;
            outAusgabeklappe    = EIN;
            outRGB              = GRUEN;  

            if (!inAusgabeklappe && !inBezahlung_ok)
            {
                zustand = WAHL;
            }                
            break;

        }

        //Ausgang
        
        
        outLed =    (outAusgabeklappe   * MASKE_OUT_AUSGABEKLAPPE)
                    |(outProduktwahl    << POSITION_Produktwahl)
                    |(outProdukt        << POSITION_Produkt);
        
        
        PORT_LED_LOW_BYTE   =   outLed;
        PORT_LED_HIGH_BYTE  =   (outLed>>SHIFT_HIGH_BYTE);
        PORT_RGB            =   (outRGB<<SHIFT_RGB);
    }
}

