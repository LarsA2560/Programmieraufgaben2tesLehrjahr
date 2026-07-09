/*
 * 7Segment_Anzeige.c
 *
 * Created: 18/12/2025 13:06:14
 * Author : Lars
 */ 

/* Bit 5 A
   Bit 6 B
   Bit 2 C
   Bit 3 D
   Bit 4 E
   Bit 0 F
   Bit 1 G
   Bit 7 H
*/
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define  SYSTEMTAKT_ms      (10)

#define PORT_OUT_7_SEGMENT     (PORTB)
#define DDR_OUT_7_SEGMENT      (DDRB)

#define PIN_SCHALTER            (PINC)
#define DDR_SCHALTER            (DDRC)

#define ALLE_PINS_IN    (0x00)
#define ALLE_PINS_OUT   (0xFF)

#define MASKE_IN_SCHALTER_EINER (0xFF)

#define MASKE_OUT_DATA_SIGNAL (1<<1)
#define MASKE_OUT_STCP (3)
#define MASKE_OUT_CLOCK (1<<0)
#define MASKE_OUTPUT_ENABLE (1<<2)

#define SEGMENT_A   (1<<15)
#define SEGMENT_B   (1<<14)
#define SEGMENT_C   (1<<13)
#define SEGMENT_D   (1<<12)
#define SEGMENT_E   (1<<11)
#define SEGMENT_F   (1<<10)
#define SEGMENT_N_J (SEGMENT_N|SEGMENT_J)
#define SEGMENT_H   (1<<8)
#define SEGMENT_N   (1<<3)
#define SEGMENT_J   (1<<7)

#define HOECHSTER_ANZEIGBARER_WERT  (99999999)
#define HOECHSTER_SPEICHERWERT      (254)
#define ANZAHLSTELLEN               (8)

int main(void)
{
    DDR_OUT_7_SEGMENT   = ALLE_PINS_OUT;
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDRA                = ALLE_PINS_OUT;
    
    uint8_t outLed      = 0;
    uint8_t inSchalter  = 0;
    
    uint8_t inSchalter_Eingabe = 0;
    
    uint8_t outDs   = 0;
    uint8_t outClock = 0;
    uint8_t outSTCP = 0;
    uint8_t outputEnable = 0;
    
    uint8_t sieben_Segment_PWR          = 0;
    uint8_t sieben_Segment_ausgabe_dez  = 0;
    uint32_t sieben_Segment_ausgabe_bin  = 0;
        
    uint8_t cycler                      = 0;
    uint8_t stellentracker           = 0;
    
   
    const uint32_t ANZEIGE_7_SEGMENT[17] =
    {   
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F,                //0
        SEGMENT_B|SEGMENT_C,                                                        //1
        SEGMENT_A|SEGMENT_B|SEGMENT_D|SEGMENT_E|SEGMENT_N_J,                        //2
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_N_J,                        //3
        SEGMENT_B|SEGMENT_C|SEGMENT_F|SEGMENT_N_J,                                  //4
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_F|SEGMENT_N_J,                        //5
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,              //6
        SEGMENT_A|SEGMENT_B|SEGMENT_C,                                              //7
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,    //8
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_F|SEGMENT_N_J,              //9
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,              //10(A)
        SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                        //11(B)
        SEGMENT_A|SEGMENT_D|SEGMENT_E|SEGMENT_F,                                    //12(C)
        SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_N_J,                        //13(D)
        SEGMENT_A|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                        //14(E)
        SEGMENT_A|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                                  //15(F)
        SEGMENT_N_J                                                                 //-
              
    }; 
   
    while (1) 
    {
        sieben_Segment_PWR          = 1;
        //Eingabe
        inSchalter   = PIN_SCHALTER;       
        inSchalter_Eingabe     = (inSchalter & MASKE_IN_SCHALTER_EINER);
        //Verarbeitung
        
        
        
        //BCD-Code
        if (inSchalter>HOECHSTER_ANZEIGBARER_WERT)
        {
            sieben_Segment_ausgabe_dez    = 16; 
            
        }
        if (inSchalter_Eingabe > HOECHSTER_SPEICHERWERT){
            sieben_Segment_PWR = 0;
        }
        sieben_Segment_ausgabe_bin = ANZEIGE_7_SEGMENT[sieben_Segment_ausgabe_dez];
        
        //Ausgabe
        
        for (stellentracker=0;stellentracker<ANZAHLSTELLEN;stellentracker+=1) //Stellt die Stelle der Zahl fest und gibt bei sieben_Segment_ausgabe_bin die Zahl der derzeitigen stelle aus
        {
        sieben_Segment_ausgabe_dez      = inSchalter_Eingabe;
        sieben_Segment_ausgabe_dez      = (sieben_Segment_ausgabe_dez % 10);
        sieben_Segment_ausgabe_bin      = ~ANZEIGE_7_SEGMENT[sieben_Segment_ausgabe_dez];
        
        inSchalter_Eingabe /= 10;
        
        
        
            for (cycler=0;cycler<16;cycler=cycler+1) //Shift Logik 
            {   

                outDs = (sieben_Segment_ausgabe_bin & (1<<0));
                
                sieben_Segment_ausgabe_bin=(sieben_Segment_ausgabe_bin>>1);             
                outLed  =   ((outDs*MASKE_OUT_DATA_SIGNAL*sieben_Segment_PWR)
                            |(outClock*MASKE_OUT_CLOCK)
                            |(~stellentracker<<MASKE_OUT_STCP)
                            |(MASKE_OUTPUT_ENABLE));
                            

                    
                PORT_OUT_7_SEGMENT = outLed;
                outClock = 1;
                outLed |= (outClock*MASKE_OUT_CLOCK);
                PORT_OUT_7_SEGMENT = outLed;
                outClock = 0;
               
            }
      
        /*outputEnable= 0;
        outLed &=~ (outputEnable*MASKE_OUTPUT_ENABLE);*/
        PORT_OUT_7_SEGMENT &=~ MASKE_OUTPUT_ENABLE;
        _delay_ms(1);         
//          outputEnable = 1;
//         outLed |= (outputEnable*MASKE_OUTPUT_ENABLE);
        PORT_OUT_7_SEGMENT |= MASKE_OUTPUT_ENABLE;  
        }      
        stellentracker = 0;
         
        /*
        //Logik zur übertragung des Shift Registers in das Ausgaberegister
        outLed |= (outSTCP*MASKE_OUT_STCP);
        PORT_OUT_7_SEGMENT = outLed;
        outSTCP = 0;
        outLed |= (outSTCP*MASKE_OUT_STCP);
        PORT_OUT_7_SEGMENT = outLed;
        */
    }
}

