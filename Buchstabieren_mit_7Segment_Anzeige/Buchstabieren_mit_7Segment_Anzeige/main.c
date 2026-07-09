/*
 * Buchstabieren_mit_7Segment_Anzeige.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

#define PORT_OUT_7_SEGMENT     (PORTB)
#define DDR_OUT_7_SEGMENT      (DDRB)

#define PIN_SCHALTER            (PINC)
#define DDR_SCHALTER            (DDRC)

#define PIN_TASTER              (PINL)
#define DDR_TASTER              (DDRL)
//Maske DDR
#define ALLE_PINS_IN            (0x00)
#define ALLE_PINS_OUT           (0xFF)
//Maske_IN
#define MASKE_IN_SCHALTER_EINER (0xFF)
//Maske_OUT
#define MASKE_OUT_DATA_SIGNAL (1<<1)
#define MASKE_OUT_STCP (3)
#define MASKE_OUT_CLOCK (1<<0)
#define MASKE_OUTPUT_ENABLE (1<<2)
//Maske_Divers
#define SEGMENT_A       (1<<15)
#define SEGMENT_B       (1<<14)
#define SEGMENT_C       (1<<13)
#define SEGMENT_D       (1<<12)
#define SEGMENT_E       (1<<11)
#define SEGMENT_F       (1<<10)
#define SEGMENT_N_J     (SEGMENT_N|SEGMENT_J)
#define SEGMENT_G       (1<<9)
#define SEGMENT_H       (1<<8)
#define SEGMENT_J       (1<<7)
#define SEGMENT_K       (1<<6)
#define SEGMENT_L       (1<<5)
#define SEGMENT_M       (1<<4)
#define SEGMENT_N       (1<<3)
#define SEGMENT_P       (1<<2)
#define SEGMENT_PUNKT   (1<<1)

#define ANZAHLSTELLEN               (8)

#define MASKE_STELLE_1  (((stellentracker==7)>0))
#define MASKE_STELLE_2  (((stellentracker==6)>0))
#define MASKE_STELLE_3  (((stellentracker==5)>0))
#define MASKE_STELLE_4  (((stellentracker==4)>0))
#define MASKE_STELLE_5  (((stellentracker==3)>0))
#define MASKE_STELLE_6  (((stellentracker==2)>0))
#define MASKE_STELLE_7  (((stellentracker==1)>0))
#define MASKE_STELLE_8  (((stellentracker==0)>0))

#define SYSTEMTAKT_ms  (1)
int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    DDR_OUT_7_SEGMENT   = ALLE_PINS_OUT;
    DDR_SCHALTER        = ALLE_PINS_IN;
    DDR_TASTER          = ALLE_PINS_IN;
    DDRA                = ALLE_PINS_OUT;
   
    //Einlese/Ausgabe Variablen
    const char text[] = "Nipun ist super Cool        ";
    
    uint8_t outLed                      = 0;
    uint8_t inSchalter                  = 0;
    uint8_t inTaster                    = 0;
    
    uint8_t inSchalter_Eingabe          = 0;
    
    uint8_t outDs                       = 0;
    uint8_t outClock                    = 0;
    uint8_t outSTCP                     = 0;
    uint8_t outputEnable                = 0;    
    //Andere Variablen
    uint8_t sieben_Segment_PWR          = 1;
    uint8_t sieben_Segment_ausgabe_dez  = 0;
    uint32_t sieben_Segment_ausgabe_bin = 0;

    uint8_t cycler                      = 0;
    uint8_t stellentracker              = 0;
    
    uint8_t textposition                = 0;
    
    uint16_t systemZeit_ms               = 0;
    uint16_t startzeit                   = 0;
    
    const uint32_t ANZEIGE_7_SEGMENT[37] =
    {
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_H|SEGMENT_M,    //0
        SEGMENT_B|SEGMENT_C,                                                                //1
        SEGMENT_A|SEGMENT_B|SEGMENT_D|SEGMENT_E|SEGMENT_N_J,                                //2
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_N_J,                                //3
        SEGMENT_B|SEGMENT_C|SEGMENT_F|SEGMENT_N_J,                                          //4
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_F|SEGMENT_N_J,                                //5
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                      //6
        SEGMENT_A|SEGMENT_B|SEGMENT_C,                                                      //7
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,            //8
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_F|SEGMENT_N_J,                      //9
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                      //10(A)
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_J|SEGMENT_G|SEGMENT_L,              //11(B)
        SEGMENT_A|SEGMENT_D|SEGMENT_E|SEGMENT_F,                                            //12(C)
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_G|SEGMENT_L,                        //13(D)
        SEGMENT_A|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                                //14(E)
        SEGMENT_A|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                                          //15(F)
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_J,                        //16(G)
        SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                                //17(H)
        SEGMENT_G|SEGMENT_L,                                                                //18(I)
        SEGMENT_A|SEGMENT_G|SEGMENT_M,                                                      //19(J)
        SEGMENT_E|SEGMENT_F|SEGMENT_H|SEGMENT_K|SEGMENT_N,                                  //20(K)
        SEGMENT_D|SEGMENT_E|SEGMENT_F,                                                      //21(L)
        SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_H|SEGMENT_P,                        //22(M)
        SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_K|SEGMENT_P,                        //23(N)
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F,                        //24(O)
        SEGMENT_A|SEGMENT_B|SEGMENT_E|SEGMENT_F|SEGMENT_N_J,                                //25(P)
        SEGMENT_A|SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F|SEGMENT_K,              //26(Q)
        SEGMENT_A|SEGMENT_B|SEGMENT_E|SEGMENT_F|SEGMENT_N_J|SEGMENT_K,                      //27(R)
        SEGMENT_A|SEGMENT_C|SEGMENT_D|SEGMENT_F|SEGMENT_N_J,                                //28(S)
        SEGMENT_A|SEGMENT_G|SEGMENT_L,                                                      //29(T)
        SEGMENT_B|SEGMENT_C|SEGMENT_D|SEGMENT_E|SEGMENT_F,                                  //30(U)
        SEGMENT_E|SEGMENT_F|SEGMENT_H|SEGMENT_M,                                            //31(V)
        SEGMENT_B|SEGMENT_C|SEGMENT_E|SEGMENT_F|SEGMENT_K|SEGMENT_M,                        //32(W)
        SEGMENT_H|SEGMENT_K|SEGMENT_M|SEGMENT_P,                                            //33(X)
        SEGMENT_H|SEGMENT_L|SEGMENT_P,                                                      //34(Y)
        SEGMENT_A|SEGMENT_D|SEGMENT_H|SEGMENT_M,                                            //35(Z)                                                                            
        SEGMENT_D                                                                           //_     
        
    };    
    while (1) 
    {
        //Variablem Always Low/High
        textposition = 7;
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter   = PIN_SCHALTER;
        inTaster    = PIN_TASTER;
        inSchalter_Eingabe     = inSchalter;
        
                     
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        lcdWriteText(0,0,text);
        while (text[textposition])
        {
        
        if((systemZeit_ms - startzeit) >= 100){
            textposition += 1;
            startzeit = systemZeit_ms;
            }
        for (stellentracker=0;stellentracker<ANZAHLSTELLEN;stellentracker+=1)
            {
            
            sieben_Segment_ausgabe_dez = text[textposition-stellentracker];
            if ((sieben_Segment_ausgabe_dez>64)&(sieben_Segment_ausgabe_dez<91))
                {sieben_Segment_ausgabe_dez -= 55;}
            else if ((sieben_Segment_ausgabe_dez>96)&(sieben_Segment_ausgabe_dez<123))
                {sieben_Segment_ausgabe_dez -= 0x20; sieben_Segment_ausgabe_dez -= 55;} 
            else if ((sieben_Segment_ausgabe_dez>47)&(sieben_Segment_ausgabe_dez<58))
                {sieben_Segment_ausgabe_dez -= 48;}
            else
                {
                    sieben_Segment_ausgabe_dez = 36;
                }
            sieben_Segment_ausgabe_bin      = ~ANZEIGE_7_SEGMENT[sieben_Segment_ausgabe_dez];

            for (cycler=0;cycler<16;cycler=cycler+1) // 
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
      

                PORT_OUT_7_SEGMENT &=~ MASKE_OUTPUT_ENABLE;
                _delay_ms(1);         
                PORT_OUT_7_SEGMENT |= MASKE_OUTPUT_ENABLE;  
            }      
            stellentracker = 0;
        //Warten_____________________________________________________________________________________________________________________________________________________________
        _delay_ms(SYSTEMTAKT_ms);
        systemZeit_ms += SYSTEMTAKT_ms;        
        }        
    }
}

