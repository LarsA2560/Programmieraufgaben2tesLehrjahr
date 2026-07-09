/*
 * Nils_idee.c
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
    initBoard(1);

//     uint16_t systemzeit_ms = 0;
//     uint16_t wechselzeit = 0;
//     uint8_t address_RS = 1;
//     uint8_t address_fire =2;
//     uint8_t address_rschrag = 3;
//     uint8_t address_fschrag = 4;
//     uint8_t address_rL = 5;
//     uint8_t address_fL = 6;
//     
//     typedef enum zustand_RL_t {Stationary,Abheben,Schrag,leaving,idle} zustand_RL_t;
//     zustand_RL_t zustand_RL = Stationary;
//     uint8_t zustand_RL_alt = 0;
//     const uint8_t Rocket_stationary[] = {
//         0x04,
//         0x0A,
//         0x0A,
//         0x0A,
//         0x0A,
//         0x15,
//         0x1B,
//         0x11
//     };
//     const uint8_t fire[] = {
//         0x04,
//         0x0A,
//         0x04,
//         0x04,
//         0x00,
//         0x00,
//         0x00,
//         0x00
//     };
//     const uint8_t rackete_schrag[] = {
//         0x03,
//         0x05,
//         0x1A,
//         0x14,
//         0x0C,
//         0x00,
//         0x00,
//         0x00
//     };
//     const uint8_t fire_schrag[] = {
//         0x00,
//         0x00,
//         0x00,
//         0x00,
//         0x00,
//         0x03,
//         0x03,
//         0x04
//         
//     };
//     const uint8_t rackete_leaving[] = {
//         0x05,
//         0x1A,
//         0x14,
//         0x0C,
//         0x00,
//         0x00,
//         0x00,
//         0x00
//     };
//     const uint8_t fire_leaving[] = {
//         0x00,
//         0x00,
//         0x00,
//         0x00,
//         0x03,
//         0x03,
//         0x04,
//         0x00
//         
//     };
//     lcdCreateCustomChar(address_RS,Rocket_stationary);
//     lcdCreateCustomChar(address_fire,fire);
//     lcdCreateCustomChar(address_rschrag,rackete_schrag);  
//     lcdCreateCustomChar(address_fschrag,fire_schrag);
//     lcdCreateCustomChar(address_rL,rackete_leaving);
//     lcdCreateCustomChar(address_fL,fire_leaving);
//     
//     while (1) 
//     {
//         //Variablem Always Low/High
//         
//         //Flankenerkennung
//         systemzeit_ms = getSystemTimeMs();      
//         if (zustand_RL_alt != zustand_RL)
//         {
//             lcdWriteText(0,0,"                                    ");
//             lcdWriteText(1,0,"                                    ");
//             lcdWriteText(2,0,"                                    ");
//             lcdWriteText(3,0,"                                    ");
//         }
//         zustand_RL_alt = zustand_RL;
//         switch (zustand_RL)
//         {
//         case Stationary:
//             
//             lcdWriteText(3,0,"%c",address_RS);
//             if ((systemzeit_ms-wechselzeit)>= 500)
//             {
//                 zustand_RL = Abheben;
//                 wechselzeit = systemzeit_ms;
//             }
//         	break;
//         case Abheben:
//             lcdWriteText(2,0,"%c",address_RS);
//             lcdWriteText(3,0,"%c",address_fire);
//             if ((systemzeit_ms-wechselzeit)>= 500)
//             {
//                 zustand_RL = Schrag;
//                 wechselzeit = systemzeit_ms;
//             }
//             break;
//         case Schrag:
//             lcdWriteText(1,1,"%c",address_rschrag);
//             lcdWriteText(1,0,"%c",address_fschrag);
//             if ((systemzeit_ms-wechselzeit)>= 500)
//             {
//                 zustand_RL = leaving;
//                 wechselzeit = systemzeit_ms;
//             }
//             break;
//         case leaving:
//             lcdWriteText(0,2,"%c",address_rL);
//             lcdWriteText(0,1,"%c",address_fL);
//             if ((systemzeit_ms-wechselzeit)>= 500)
//             {
//                 zustand_RL = idle;
//                 wechselzeit = systemzeit_ms;
//             }
//             break;
//         case idle:
//             break;
//         }
        
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
         
         
        //Warten_____________________________________________________________________________________________________________________________________________________________
//    }
}

