/*
 * Weihnachtsstern.c
 *
 * Created: 06.12.2017 13:40:40
 * Author : scol
 */ 

#include <avr/io.h>

#include "LED_Treiber.h"



int main(void)
{
    initLED_Treiber();
     uint8_t out_Led_Position    = 0;
     float out_HWB               = 0;
     uint8_t out_RGB_Rot         = 0;
     uint8_t out_RGB_Gruen       = 0;
     uint8_t out_RGB_Blau        = 255;
     
     uint8_t random_zahl         = rand()%3;
     
     while (1)
     {
         // Farbwechsler
         
         for (out_Led_Position=0;out_Led_Position<ANZ_RGB_LED;out_Led_Position+=1)
         {
             SetLED(out_Led_Position,out_RGB_Rot,out_RGB_Gruen,out_RGB_Blau);
         }
         if ((out_RGB_Rot<255) && out_RGB_Blau && !out_RGB_Gruen)
         {out_RGB_Rot += 1;}
         if ((out_RGB_Rot==255) && out_RGB_Blau && !out_RGB_Gruen)
         {out_RGB_Blau -= 1;}
         if ((out_RGB_Gruen<255) && out_RGB_Rot && !out_RGB_Blau)
         {out_RGB_Gruen += 1;}
         if ((out_RGB_Gruen==255) && out_RGB_Rot && !out_RGB_Blau)
         {out_RGB_Rot -= 1;}
         if ((out_RGB_Blau<255) && out_RGB_Gruen && !out_RGB_Rot)
         {out_RGB_Blau += 1;}
         if ((out_RGB_Blau==255) && out_RGB_Gruen && !out_RGB_Rot)
         {out_RGB_Gruen -= 1;}
         
         /*
         for (out_Led_Position=0;out_Led_Position<ANZ_RGB_LED;out_Led_Position+=1)
         {
             SetLedHwb(out_Led_Position,out_HWB,0,0);
         }
         out_HWB += 0.5;
         if (out_HWB < 360)
         {out_HWB = 0; }    
         */    
         random_zahl         = rand()%3;
         RunLEDStripe();
         wait1ms(10);
         
    }
}

