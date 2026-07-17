/*
 * WS2812_example.c
 *
 * Created: 30.09.2019 14:56:06
 * Author : Dario Dündar
 */ 

#include <avr/io.h>
/*Enthält LED-Treiber und delay-Funktionen*/
#include "ws2812.h"


int main()
{
    rgb_color leds[LED_COUNT];
    uint8_t c_temp = 0;
    uint8_t farbspeicher_R = 50;
    uint8_t farbspeicher_G = 0;
    uint8_t farbspeicher_B = 0;
    uint8_t farbspeicher_Temp = 0;
    uint8_t zahler = 0;
   
    while(1)
    {
        /*
        for (uint16_t i = 0; i<=255; i+=8)
        {
        
        leds[i] = (rgb_color){255,255,255};
            c_temp = i+1;
        leds[c_temp] = (rgb_color){255,0,0};
            c_temp = i+2;
        leds[c_temp] = (rgb_color){255,255,0};
            c_temp = i+3;
        leds[c_temp] = (rgb_color){0,255,0};
            c_temp = i+4;
        leds[c_temp] = (rgb_color){0,255,255};
            c_temp = i+5;
        leds[c_temp] = (rgb_color){0,0,255};
            c_temp = i+6;
        leds[c_temp] = (rgb_color){255,0,255};
            c_temp = i+7;
        leds[c_temp] = (rgb_color){255,255,255};
        }
        
        
        led_strip_write(leds);
        _delay_ms(500);
        for (uint16_t i = 0; i<=255; i+=8)
        {
            
            leds[i] = (rgb_color){37, 150, 190};
            c_temp = i+1;
            leds[c_temp] = (rgb_color){242, 78, 141};
            c_temp = i+2;
            leds[c_temp] = (rgb_color){242, 78, 141};
            c_temp = i+3;
            leds[c_temp] = (rgb_color){255,255,255};
            c_temp = i+4;
            leds[c_temp] = (rgb_color){255,255,255};
            c_temp = i+5;
            leds[c_temp] = (rgb_color){242, 78, 141};
            c_temp = i+6;
            leds[c_temp] = (rgb_color){242, 78, 141};
            c_temp = i+7;
            leds[c_temp] = (rgb_color){37, 150, 190};
        }
        led_strip_write(leds);
        _delay_ms(100);
        
        for (uint16_t i = 0; i<=255; i+=8)
        {
            
            leds[i] = (rgb_color){0, 0, 0};
            c_temp = i+1;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+2;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+3;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+4;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+5;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+6;
            leds[c_temp] = (rgb_color){0, 0, 0};
            c_temp = i+7;
            leds[c_temp] = (rgb_color){0, 0, 0};
        }
        
        led_strip_write(leds);
        _delay_ms(500);*/
        
        
        for (uint16_t i=0; i<255; i+=1)
        {
            zahler += 1;
            if (zahler == 8)
            {   
                farbspeicher_Temp = farbspeicher_B;
                farbspeicher_B = farbspeicher_G;
                farbspeicher_G = farbspeicher_R;
                farbspeicher_R =farbspeicher_Temp;
                zahler = 0;
            }
            
            leds[i] = (rgb_color){farbspeicher_R,farbspeicher_G,farbspeicher_B};
                 
        }
        farbspeicher_Temp = farbspeicher_B;
        farbspeicher_B = farbspeicher_G;
        farbspeicher_G = farbspeicher_R;
        farbspeicher_R =farbspeicher_Temp;
        
        
        _delay_ms(500);
        led_strip_write(leds);
    }
}

