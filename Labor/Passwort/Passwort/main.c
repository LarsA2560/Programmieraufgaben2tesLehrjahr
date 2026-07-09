/*
 * Passwort.c
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
    const char passwort[] = "123456/890";
    uint8_t flagBenutzernameOk = 1;
    //Andere Variablen
    for(uint8_t i = 0; passwort[i]; i += 1)
    {
        if (((passwort[i]>=0x30)&&(passwort[i]<=0x39))|((passwort[i]>=0x41)&&(passwort[i]<=0x5A))|((passwort[i]>=0x61)&&(passwort[i]<=0x7A)))
        {
            
        }
        else
        {
            flagBenutzernameOk = 0;
        }
    }   
    rgbWrite(1023*(!flagBenutzernameOk),1023 * flagBenutzernameOk,0);
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
              
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

