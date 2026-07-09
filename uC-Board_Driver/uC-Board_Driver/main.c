/*
 * uC-Board_Driver.c
 *
 * Created: 26/01/2026 09:51:10
 * Author : Lars
 */ 

#include "ucBoardDriver.h"


int main(void)
{
    initBoard(0);
    uint16_t outLed =1;
    uint16_t inAdcWertJoystickX = 0;
    uint16_t inAdcWertJoystickY = 0;
    uint16_t outPositionLCD_X = 0;
    uint16_t outPositionLCD_Y = 0;
    uint8_t inSchalter        = 0;
    uint16_t outSpannung_X =0;
    uint16_t outSpannung_y =0;
    while (1) 
    {
    inAdcWertJoystickX = adcRead(ADC_10_JOYSTICK_X);
    inAdcWertJoystickY = adcRead(ADC_11_JOYSTICK_Y);
    inSchalter = switchReadAll();
    
    if (inAdcWertJoystickX>=512)
    {
        outLed = outLed<<1;
        outPositionLCD_X = outPositionLCD_X-1;
        
    }
    else if(inAdcWertJoystickX<=430){
        outLed = outLed>>1;
        outPositionLCD_X = outPositionLCD_X+1;
    }
    if (inAdcWertJoystickY>=550)
    {
        outPositionLCD_Y = outPositionLCD_Y-1;
        
        
    }
    else if(inAdcWertJoystickY<=450){
        outPositionLCD_Y = outPositionLCD_Y+1;
    }
    outSpannung_X = ((uint32_t)5000)*inAdcWertJoystickX/1006;
    outSpannung_y = ((uint32_t)5000)*inAdcWertJoystickY/1023;
    
    ledWriteAll(inSchalter);
    lcdLight(inSchalter);
    
    lcdWriteText(0,0,"ADCX:%4u Y:%4u",inAdcWertJoystickX,inAdcWertJoystickY);
    lcdWriteText(1,0,"U  X:%1u.%3uV Y:%1u.%3uV",outSpannung_X/1000,outSpannung_X%1000,outSpannung_y/1000,outSpannung_y%1000);
    
    
    //lcd full Clear
    lcdWriteText(0,0,"                                                             ");
    lcdWriteText(1,0,"                                                             ");
    lcdWriteText(2,0,"                                                             ");
    lcdWriteText(3,0,"                                                             ");
    lcdWriteText(outPositionLCD_Y,outPositionLCD_X,"X:%4u Y:%4u", inAdcWertJoystickX,inAdcWertJoystickY);
    waitForSystemTimeMs(getSystemTimeMs()+100);
    
    }
}

