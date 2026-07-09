/*
 * Temperatursensor.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN

//Maske_OUT

//Maske_Divers

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint16_t outLed =1;
    uint16_t inAdcWertTempSensor = 0;
    uint16_t outSpannung_Temp = 0;    
    int16_t outTemp = 0;
    uint32_t outSpannung_Poti1 = 0;
    uint32_t outSpannung_Poti2 = 0;
    
    uint16_t inPotentiometer_1 = 0;
    uint16_t inPotentiometer_2 = 0;
    int32_t outMinTemp = 0;
    int32_t outMaxTemp = 0;
    uint8_t outRGBLedR = 0;
    uint8_t outRGBLedG = 0;
    uint8_t outRGBLedB = 0;
    
    //Andere Variablen
       
    while (1) 
    {
        //Variablem Always Low/High
        
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
       inAdcWertTempSensor = adcRead(ADC_13_TEMPERATUR);
       inPotentiometer_1 = adcRead(ADC_08_POTI_1);
       inPotentiometer_2 = adcRead(ADC_09_POTI_2);
             
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        outSpannung_Temp = ((uint32_t)5000)*inAdcWertTempSensor/1023;
        outSpannung_Poti1 = ((uint32_t)5000)*inPotentiometer_1/1023;
        outSpannung_Poti2 = ((uint32_t)5000)*inPotentiometer_2/1023;
        
        outTemp = ((int64_t)outSpannung_Temp * 1000 - 400000) * 1000 / 19500;
        outMinTemp = ((int64_t)outSpannung_Poti1 * 1000- 400000) * 1000 / 19500; 
        outMaxTemp = ((int64_t)outSpannung_Poti2 * 1000- 400000) * 1000 / 19500;  
        
        if (outTemp<outMinTemp)
        {
            outRGBLedR = 0;
            outRGBLedG = 0;
            outRGBLedB = 1;
        } 
        else if(outTemp>outMaxTemp)
        {
            outRGBLedR = 1;
            outRGBLedG = 0;
            outRGBLedB = 0;
        }
        else
        {
            outRGBLedR = 0;
            outRGBLedG = 1;
            outRGBLedB = 0;
        }        
          
        
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        lcdWriteText(0,0,"TempSens:%4u",inAdcWertTempSensor); 
        lcdWriteText(1,0,"TempSens:%4umV",outSpannung_Temp);
        outSpannung_Temp = ((uint32_t)5000)*inAdcWertTempSensor/1023;
        outSpannung_Poti1 = ((uint32_t)5000)*inPotentiometer_1/1023;
        outSpannung_Poti2 = ((uint32_t)5000)*inPotentiometer_2/1023;
        
        outTemp = ((int64_t)outSpannung_Temp * 1000 - 400000) * 1000 / 19500;
        outMinTemp = ((int64_t)outSpannung_Poti1 * 1000- 400000) * 1000 / 19500;
        outMaxTemp = ((int64_t)outSpannung_Poti2 * 1000- 400000) * 1000 / 19500;
        
        if (outTemp<outMinTemp)
        {
            outRGBLedR = 0;
            outRGBLedG = 0;
            outRGBLedB = 1;
        }
        else if(outTemp>outMaxTemp)
        {
            outRGBLedR = 1;
            outRGBLedG = 0;
            outRGBLedB = 0;
        }
        else
        {
            outRGBLedR = 0;
            outRGBLedG = 1;
            outRGBLedB = 0;
        }
        
        
        //Ausgabe____________________________________________________________________________________________________________________________________________________________
        lcdWriteText(0,0,"TempSens:%4u",inAdcWertTempSensor);
        lcdWriteText(1,0,"TempSens:%4umV",outSpannung_Temp);
        lcdWriteText(2,0,"TempSens:%3i.%1iC",outTemp/1000,outTemp/100%10);
        if(outTemp>= 0){
            lcdWriteText(2,0,"TempSens:%3i.%1iC",outTemp/1000,outTemp/100%10);
        }
        else{
            lcdWriteText(2,0,"TempSens:-%3i.%1iC",outTemp/1000*(-1),outTemp/100*(-1)%10);
        }
        
        rgbWrite(outRGBLedR*1023,outRGBLedG*1023,outRGBLedB*1023);
        lcdWriteText(2,0,"TempSens:%3i.%1iC",outTemp/1000,outTemp/100%10);
        if(outMinTemp>= 0){
            lcdWriteText(3,0,"Min%3li.%1liC",outMinTemp/1000,outMinTemp/100%10);
        }
        else{
            lcdWriteText(3,0,"Min-%3li.%1liC",outMinTemp/1000*(-1),outMinTemp/100*(-1)%10);
        }
        if(outMaxTemp>= 0){
            lcdWriteText(3,11,"Max%3lu.%1luC",outMaxTemp/1000,outMaxTemp/100%10);
        }
        else{
            lcdWriteText(3,11,"Max-%3lu.%1luC",outMaxTemp/1000*(-1),outMaxTemp/100*(-1)%10);
        }
          
        rgbWrite(outRGBLedR*1023,outRGBLedG*1023,outRGBLedB*1023);   
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

