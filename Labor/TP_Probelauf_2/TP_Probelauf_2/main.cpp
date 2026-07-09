/*
 * TP_Probelauf_2.cpp
 *
 * Created: 03/06/2026 13:07:42
 * Author : Lars
 */ 

 #include "ucBoardDriver.h"
 #define AUS (0)
 #define EIN (1)


int main(void)
{
    initBoard(0);
    
    uint8_t onOff_modes = 0;
    uint8_t schalter = 0;
    uint8_t power_switch = 0;
    
    while (1) {
//Eingabe
       schalter = PINC;
       power_switch = (schalter & 0b0001)>0; 
//Verarbeitung
        switch (onOff_modes){
            case AUS:
            
                if(power_switch){
                    onOff_modes = EIN;
                }
            break;
            case EIN:
                if(!power_switch){
                    onOff_modes = AUS;
                }
            break;
        }
//Ausgabe

lcdWriteZahl(0,0,onOff_modes,1,0);
    }
}

