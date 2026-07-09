/*
 * C_String_Benutzen.c
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
//     const char meinName1[]={0x4C,0x61,0x72,0x73,0x20,0x4D,0x61,0x65,0x72,0x6B,0x69,0};
//     const char meinName2[]={'L','a','r','s',' ','M','a','e','r','k','i',0};
     char meinName3[] = "New Phone who dis";
     char meinName4[] = "New Phone who dis";
     int8_t warnflag = 0;
     uint8_t anzahlBuchstaben = 0;
    //Andere Variablen
    
    
    /*Niels Code
    char name1 [] = {"NielsNiels"};
    char names [] = {"NielsNiels"};
    char * binames [] = {"Neo hat einen", "kleinen schwanni"};
    uint8_t hallo = 0;
    hallo	 = names;
    hallo = (~hallo)-5;
    lcdWriteText(0,0,name1[0]);
    lcdWriteText(0,0,binames[0]);
    lcdWriteText(1,0,binames[1]);
    lcdWriteText(3,0,names);
    lcdWriteZahl(2,0,hallo,2,0);
    */
    while (1) 
    {
        //Variablem Always Low/High
        
        //Flankenerkennung
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
              
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
         for(anzahlBuchstaben = 0; meinName3[anzahlBuchstaben]; anzahlBuchstaben += 1)
         {
         if(strcmp(meinName3,meinName4)){warnflag = 1;}   
         
         if ((meinName3[anzahlBuchstaben]>='a')&& (meinName3[anzahlBuchstaben]<='z'))
         {
               meinName3[anzahlBuchstaben] = meinName3[anzahlBuchstaben] - 0x20;
         }
         }
         
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
         lcdWriteText(0,0,"%s",meinName3);
         lcdWriteText(1,0,"%u",warnflag);
         lcdWriteText(2,0,"%u",anzahlBuchstaben);
//         lcdWriteText(1,0,"%s",meinName2);
//         lcdWriteText(2,0,"%s",meinName3);
//         lcdWriteText(3,0,"%s","Lars Maerki");      
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

