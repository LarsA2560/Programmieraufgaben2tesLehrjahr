/*
 * Iron_Loop.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"

//Pin Configuration

//Maske DDR

//Maske_IN
#define MASKE_IN_SCHALTER_ZUG_IM_BAHNHOF    (1<<0)
#define MASKE_IN_TASTER_NOTAUS              (1<<1)
#define MASKE_IN_TASTER_WEITER              (1<<0)
//Maske_OUT
#define MASKE_OUT_RGB_HELLIKEIT     (1023)
#define MASKE_OUT_LED_ALL_AN        (0xFFFF)
//Maske_Divers
#define BLINKGESCHWINDIGKEIT        (1000)

#define  ZUGLAENGE                  (8)
#define ZUG_NICHT_IN_DER_STATION    (0)
#define ZUG_IN_DER_STATION          (ZUG_NICHT_IN_DER_STATION + ZUGLAENGE)

int main(void)
{
    initBoard(0);
    //DDR Aktivieren
    
    //Pull Ups Aktivieren
    
    //Einlese/Ausgabe Variablen
    uint8_t inTaster                = 0;
    uint8_t inSchalter              = 0;
    uint16_t inADCWertPotentiometer = 0;
    uint16_t outLed                 = 0;
    uint32_t inZeit_ms              = 0;
    
    uint8_t inTaster_alt = 0;
    uint8_t inTaster_neu = 0;
    
    uint8_t inPosFlanke         = 0;
    uint8_t inZugimBahnhof      = 0;
    uint8_t inNotaus            = 0;
    uint8_t inPosFlankeWeiter   = 0;    
    
    uint8_t outLed_Rot  = 0;
    uint8_t outLed_Grun = 0;
    uint8_t outLed_Blau = 0;
    uint8_t outLedAll   = 0;
    uint16_t outLedLauflicht = 0;
    
    uint16_t    outRGBHelligkeit    = 0;
    uint8_t     outRGBAus           = 0;
    
    uint32_t blinkRGBstart          = 0;
    uint32_t blinkLedstartEmergency = 0;
    uint32_t animationstartEinfahrt = 0;
    uint32_t animationstartDispatch = 0;
    uint32_t lauflichtstart         = 0;

    //Andere Variablen
    
    uint8_t flagRGBBlinken = 0;
    uint8_t flagLedBlinkenEmergency = 0;
    uint8_t flagLauflicht = 0;
    
    uint8_t zugLaenge = 0;
    uint8_t lauflicht_Position = 0;
    
    typedef enum zustandModus_t         {NORMALBETRIEB, EMERGENCY}              zustandModus_t;
    typedef enum zustandZugposition_t   {IDLE, EINFAHRT, LADEN,DISPATCH}        zustandZugposition_t;
    typedef enum zustandLaden_t         {LOADING, LOCKING, SAFETY_CHECK, READY} zustandLaden_t;
        
    zustandModus_t          zustand_Modus       = NORMALBETRIEB;
    zustandZugposition_t    zustand_Zugposition = IDLE;
    zustandLaden_t          zustand_Laden       = LOADING;
    
    char zuganzeige[17] = {0};
    uint8_t zugverschiebung = 0;
    
    lcdWriteText(1,0,"Zuganzeige");
    
    while (1) 
    {
        //Variablem Always Low/High        
        //Flankenerkennung 
        inTaster        = buttonReadAllPL();
        inTaster_alt    = inTaster_neu;
        inTaster_neu    = inTaster;
        
        inPosFlanke  = inTaster_neu &~ inTaster_alt;
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inSchalter              = switchReadAll();
        inADCWertPotentiometer  = adcRead(ADC_08_POTI_1);
        
        inZugimBahnhof      = (inSchalter   & MASKE_IN_SCHALTER_ZUG_IM_BAHNHOF)>0;
        inNotaus            = (inTaster     & MASKE_IN_TASTER_NOTAUS)>0;
        inPosFlankeWeiter   = (inPosFlanke  & MASKE_IN_TASTER_WEITER)>0;
        
        inZeit_ms = getSystemTimeMs();
            
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        
        switch (zustand_Modus)
        {
        case NORMALBETRIEB:
            switch (zustand_Zugposition)
            {
            case IDLE:
                outLed_Rot  = 0;
                outLed_Grun = 0;
                outLed_Blau = 1;
                flagRGBBlinken = 0;
                zugLaenge = ZUG_NICHT_IN_DER_STATION;
                outLedAll = 0;
                flagLauflicht = 0;
                lcdWriteText(0,0,"Warte auf Zug                                       ");
                
                if (inZugimBahnhof)
                {
                    zustand_Zugposition = EINFAHRT;
                }
                                
            	break;
            case EINFAHRT:
                outLed_Rot  = 0;
                outLed_Grun = 0;
                outLed_Blau = 1;
                flagRGBBlinken = 0;
                outLedAll = 0;
                flagLauflicht = 1;
                lcdWriteText(0,0,"Zug fahrt ein                                       ");                              
                if (inZeit_ms-animationstartEinfahrt >= 5000)
                {
                    zustand_Zugposition = LADEN;
                }
                //Animation LCD Einfahrt
                if (inZeit_ms-animationstartEinfahrt >= (uint16_t)zugLaenge*5000/ZUG_IN_DER_STATION)
                {
                    zugLaenge += 1;
                }
                 
                break;
            case LADEN:
                zustand_Laden = READY;
                switch (zustand_Laden)
                {
                case LOADING:
                    outLed_Rot  = 1;
                    outLed_Grun = 1;
                    outLed_Blau = 1;
                    flagRGBBlinken = 0;
                    zugLaenge = ZUG_IN_DER_STATION;
                    outLedAll = 0;
                    flagLauflicht = 0;
                    lcdWriteText(0,0,"Bereit zum Schliessen                                       ");
                    if (inPosFlankeWeiter)
                    {
                        zustand_Laden = LOCKING;
                    }
                    break;                
                	
                case LOCKING:
                    outLed_Rot  = 1;
                    outLed_Grun = 1;
                    outLed_Blau = 1;
                    flagRGBBlinken = 1;
                    zugLaenge = ZUG_IN_DER_STATION;
                    outLedAll = 0;
                    flagLauflicht = 0;
                    lcdWriteText(0,0,"Bereit zum Schliessen                                       ");
                    if (inPosFlankeWeiter)
                    {
                        zustand_Laden = SAFETY_CHECK;
                    }
                    break;
                case SAFETY_CHECK:
                    outLed_Rot  = 1;
                    outLed_Grun = 1;
                    outLed_Blau = 0;
                    flagRGBBlinken = 0;
                    zugLaenge = ZUG_IN_DER_STATION;
                    outLedAll = 0;
                    flagLauflicht = 0;
                    lcdWriteText(0,0,"Sicherheit Prüfen                                       ");
                    if (inPosFlankeWeiter)
                    {
                        zustand_Laden = READY;
                    }
                    break;
                case READY:
                    outLed_Rot  = 0;
                    outLed_Grun = 1;
                    outLed_Blau = 0;
                    flagRGBBlinken = 1;
                    zugLaenge = ZUG_IN_DER_STATION;
                    outLedAll = 0;
                    flagLauflicht = 0;
                    lcdWriteText(0,0,"Bereit zur Abfahrt                                       ");
                    break;                                                       
                }
                            
                if (inPosFlankeWeiter && (zustand_Laden == READY))
                {
                    zustand_Zugposition = DISPATCH;
                }
                if (!inZugimBahnhof)
                {
                    zustand_Modus = EMERGENCY;
                }
                break;
            case DISPATCH:
                outLed_Rot  = 0;
                outLed_Grun = 1;
                outLed_Blau = 0;
                flagRGBBlinken = 0;
                outLedAll = 0;
                flagLauflicht = 1;
                lcdWriteText(0,0,"Zug fahrt aus                                       ");
                
                //Animation LCD Ausfahrt
                if ((inZeit_ms-animationstartDispatch >= (uint16_t)zugverschiebung*5000/ZUG_IN_DER_STATION)&&zugLaenge)
                {
                    zugLaenge -= 1;
                    zugverschiebung += 1;
                }
                
                if (!inZugimBahnhof && (inZeit_ms-animationstartDispatch >= 5000))
                {
                    zustand_Zugposition = IDLE;
                    zugverschiebung = 0;
                }
                break;
            }
            
            if (inNotaus)
            {
                zustand_Modus = EMERGENCY;
            }
        	break;
        case EMERGENCY:
            outLed_Rot  = 1;
            outLed_Grun = 0;
            outLed_Blau = 0;
            flagRGBBlinken = 1;
            flagLedBlinkenEmergency = 1;
            zugLaenge = ZUG_NICHT_IN_DER_STATION;
            flagLauflicht = 0;
            lcdWriteText(0,0,"Emergency                                           ");

            break;
        }
        if (zustand_Zugposition != EINFAHRT)
        {
            animationstartEinfahrt = inZeit_ms;
        }
        if (zustand_Zugposition != DISPATCH)
        {
            animationstartDispatch = inZeit_ms;
        }    

        //Ausgabe____________________________________________________________________________________________________________________________________________________________ 
        if (flagRGBBlinken)
        {
             if (inZeit_ms - blinkRGBstart >= BLINKGESCHWINDIGKEIT)
             {
                 blinkRGBstart = inZeit_ms;
             }
             if (inZeit_ms - blinkRGBstart <= BLINKGESCHWINDIGKEIT/2)
             {
                 outRGBAus = 0;
             }
             else{
                 outRGBAus = 1;
             }           
        }
        else
        {
            blinkRGBstart=inZeit_ms;
            outRGBAus = 0;
        }
        
        if (flagLedBlinkenEmergency)
        {
            if (inZeit_ms - blinkLedstartEmergency >= BLINKGESCHWINDIGKEIT)
            {
                blinkLedstartEmergency = inZeit_ms;
            }
            if (inZeit_ms - blinkLedstartEmergency <= BLINKGESCHWINDIGKEIT/2)
            {
                outLedAll = 0;
            }
            else{
                outLedAll = 1;
            }
        }
        else
        {
            blinkLedstartEmergency=inZeit_ms;
        }
        
        if (flagLauflicht)
        {
            if (!outLedLauflicht)
            {
                outLedLauflicht = 0x8000;
            }
            
            if (inZeit_ms - lauflichtstart >= 100)
            {
                outLedLauflicht = (outLedLauflicht>>1);
                lauflichtstart = inZeit_ms;
            }
        }
        else{lauflichtstart=inZeit_ms;outLedLauflicht = 0;}
        
        
        outLed = ((outLedAll * MASKE_OUT_LED_ALL_AN)|outLedLauflicht);
        outRGBHelligkeit = !outRGBAus * MASKE_OUT_RGB_HELLIKEIT;
         
        for (uint8_t i = 0; i<17; i++)
        {
            if (i<zugLaenge)
            {
                zuganzeige[i] ='#';
            }
            else{zuganzeige[i] = ' ';}
        }
        zuganzeige[16] = 0;
        
        lcdWriteText(2,0,zuganzeige);
        lcdWriteText(3,0,zuganzeige);
        
        ledWriteAll(outLed);
        rgbWrite(outLed_Rot * outRGBHelligkeit, outLed_Grun * outRGBHelligkeit, outLed_Blau * outRGBHelligkeit);
             
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

