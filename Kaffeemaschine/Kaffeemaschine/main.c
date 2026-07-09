/*
 * Kaffeemaschine.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 

#include "ucBoardDriver.h"




//Maske_IN
#define MASKE_IN_SCHALTER_ONOFF             (1<<0)
#define MASKE_IN_TASTER_1_TASSEKLEIN        (1<<0)
#define MASKE_IN_TASTER_2_TASSEGROSS        (1<<1)
#define MASKE_IN_TASTER_4_REINIGUNGBEENDET  (1<<6)    

//Maske_OUT

//Maske_Divers
#define AUFHEIZ_ZEIT_ms                     (5000)
#define REF_ZEIT_ms                         (4000)
#define REF_LED_BLOCK                       (8)
#define MAX_WERT_FULLSENSOR                 (1023)
#define WASSERSCHUTZ_FULLSTAND_PROZENT      (20)
#define ANZAHL_DURCHGANGE_BEVOR_REINIGUNG   (5)
#define MAX_HELLIGKEIT_RGB                  (1023)
#define MAX_GROESSE_BALKEN                  (16)

#define TASSE_KLEIN (1)
#define TASSE_GROSS (2)

#define PERIODE_BLINKEN_ms      (1000)
#define PERIODE_LAUFLICHT_ms    (REF_ZEIT_ms/REF_LED_BLOCK)

#define AUSATMEN    (1)
#define EINATMEN    (0)

int main(void)
{
    
    /*Hardware Initialisieren*/
    initBoard(0);
    
    /*Alle Variablen Initialisieren und auf 0 setzten*/
    //Einlese/Ausgabe Variablen
    uint8_t inSchalter          = 0;
    uint8_t inTaster            = 0;
    uint8_t inTaster_alt        = 0;
    uint8_t inPosFlanke_Taster  = 0;
    
    uint32_t inZeit_ms              = 0;
    uint32_t inTimer                = 0;
    uint32_t inZeit_blinken_ms      = 0;
    uint32_t inZeit_lauflicht_ms    = 0;
    uint32_t inZeit_Atmen_ms        = 0;
    
    uint8_t inSchalter_OnOff                = 0;
    uint8_t inPosTaster_1_TasseKlein        = 0;
    uint8_t inPosTaster_2_TasseGross        = 0;
    uint8_t inPosTaster_4_ReinigungBeendet  = 0;
    uint16_t inWasserstand                  = 0;
    
    uint16_t outLed             = 0;
    uint16_t outLed_Balken      = 0;
    uint8_t outRGB_R            = 0;
    uint8_t outRGB_G            = 0;
    uint8_t outRGB_B            = 0;
    uint16_t outRGBHelligkeit   = 0;
    
    //Andere Variablen
    uint8_t flagBlinkenRGB         = 0;
    uint8_t flagAtmen           = 0;
    uint8_t flagFortschrittsbalken  = 0;
    uint8_t flagZustandswechsel = 0;
    uint8_t flag_Aus_Einatmen   = 0;
    
    uint8_t bruhmodus           = 0;
    uint8_t anzahlGebruht       = 0;
    uint8_t groesse_LedBalken   = 0; 
    
    /*zustand_AnAus definieren*/
    typedef enum zustand_AnAus_t    {AUS, AN}                                                           zustand_AnAus_t;
    /*zustand_Kochen definieren*/
    typedef enum zustand_Kochen_t   {AUFHEIZEN, BEREIT, WASSERSCHUTZ, BRUHEN, REINIGUNG_ERFORDERLICH}   zustand_Kochen_t;
    
    /*zustand_AnAus = Aus*/
    zustand_AnAus_t     zustand_AnAus   = AUS;
    /*zustand_Kochen = Aufwärmen*/   
    zustand_Kochen_t    zustand_Kochen  = AUFHEIZEN;
    
    /*zustandAlt_AnAus     = -1*/    
    int8_t zustandAlt_AnAus     = -1;
    /*zustandAlt_Kochen    = -1*/
    int8_t zustandAlt_Kochen    = -1;
    
    while (1) 
    {
        //Variablem Always Low/High
        /*Flankenerkennung*/
        
        inTaster_alt        = inTaster;
        inSchalter          = switchReadAll();
        inTaster            = buttonReadAllPL();
        inPosFlanke_Taster  = inTaster & ~inTaster_alt;
        
        /*flagZustandswechsel =
        (zustand_AnAus nicht zustandAlt_AnAus) oder (zustand_Kochen nicht zustandAlt_Kochen)*/
        flagZustandswechsel = ((zustand_AnAus!=zustandAlt_AnAus)||(zustand_Kochen!=zustandAlt_Kochen));
        
        /*zustandAlt_AnAus = zustand_AnAus
        zustandAlt_Kochen = zustand_Kochen*/
        zustandAlt_AnAus    = zustand_AnAus;
        zustandAlt_Kochen   = zustand_Kochen;
        
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        
        /*inSchalter_OnOff aus inSchalter Maskieren
        inPosTaster_1_TasseKlein aus inPosTaster Maskieren
        inPosTaster_2_TasseGross aus inPosTaster Maskieren
        inPosTaster_4_ReinigungBeendet aus inPosTaster Maskieren
        inWasserstand Maskieren*/
        
        inSchalter_OnOff                = (inSchalter           & MASKE_IN_SCHALTER_ONOFF)              > 0;
        inPosTaster_1_TasseKlein        = (inPosFlanke_Taster   & MASKE_IN_TASTER_1_TASSEKLEIN)         > 0;
        inPosTaster_2_TasseGross        = (inPosFlanke_Taster   & MASKE_IN_TASTER_2_TASSEGROSS)         > 0;
        inPosTaster_4_ReinigungBeendet  = (inPosFlanke_Taster   & MASKE_IN_TASTER_4_REINIGUNGBEENDET)   > 0;
        inWasserstand                   = adcRead(ADC_08_POTI_1);
        
        /*Systemzeit in inZeit_ms abspeichern*/
        inZeit_ms = getSystemTimeMs() ;    
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________  
        /*zustand_AnAus*/
        switch (zustand_AnAus)
        {
        /*Aus*/
        case AUS: 
            /*Einstieg?
            (flagZustandswechsel)*/           
            if (flagZustandswechsel)
            {
                /*outLed = 0
                outRGB = 0*/
                
                outLed      = 0;
                outRGB_R    = 0;
                outRGB_G    = 0;
                outRGB_B    = 0;
            }
            /*S1 = 1*/
            if (inSchalter_OnOff)
            {
                /*zustand_AnAus = AN
                zustand_Kochen = Aufheizen*/
                
                zustand_AnAus = AN;
                zustand_Kochen = AUFHEIZEN;
            }
        	break;
        /*An*/
        case AN: 
            /*S1=0*/           
            if (!inSchalter_OnOff)
            {
                /*Zustand_AnAus = Aus*/
                zustand_AnAus = AUS;
            }
            /*Zustand_Kochen*/
            switch (zustand_Kochen)
            {
            /*Aufheizen*/
            case AUFHEIZEN:
                /*enter
                flagZustandwechsel*/
                if (flagZustandswechsel)
                {
                    outRGB_B = 1;
                    flagBlinkenRGB = 1;
                    inTimer = inZeit_ms;
                }
                //do
                /*Timer abgelaufen*/
                if((inZeit_ms - inTimer) >= AUFHEIZ_ZEIT_ms)
                {
                    /*zustand_Kochen = Bereit*/
                    zustand_Kochen = BEREIT;
                }
                /*exit
                (Zustand_Kochen nicht zustandAlt_Kochen)
                oder
                (zustand_AnAus nicht An)*/
                if ((zustand_Kochen!=zustandAlt_Kochen)||(zustand_AnAus!=AN))
                {
                /*  outRGB = 0
                    flagBlinkenRGB = 0*/
                    outRGB_B    = 0;
                    flagBlinkenRGB = 0;
                }
            	break;
            /*Bereit*/
            case BEREIT:
                /*enter
                flagZustandwechsel*/
                if (flagZustandswechsel)
                {   
                    /*outRGB = Grun*/
                    outRGB_G = 1;                 
                }
                //do
                /*inPosTaster_1_TasseKlein = 1
                oder
                inPosTaster_2_TasseGross = 1*/
                
                if (inPosTaster_1_TasseKlein||inPosTaster_2_TasseGross)
                {
                    /*zustand_Kochen = Bruhen*/
                    zustand_Kochen=BRUHEN;
                }
                /*inWasserstand <= 20%*/
                if (inWasserstand <= MAX_WERT_FULLSENSOR * WASSERSCHUTZ_FULLSTAND_PROZENT / 100)
                {
                    /*zustand_Kochen = Wasserschutz*/
                    zustand_Kochen = WASSERSCHUTZ;
                }
                /*exit
                (Zustand_Kochen nicht zustandAlt_Kochen)
                oder
                (zustand_AnAus nicht An)*/
                if ((zustand_Kochen!=zustandAlt_Kochen)||(zustand_AnAus!=AN))
                {
                    /*outRGB = 0*/
                    outRGB_G = 0;
                    /*inPosTaster_2_TasseGross = 1*/
                    if (inPosTaster_2_TasseGross)
                    {
                        /*Bruhmodus = 2*/
                        bruhmodus = TASSE_GROSS;                        
                    }
                    /*inPosTaster_1_TasseKlein*/
                    else if (inPosTaster_1_TasseKlein)
                    {
                        /*Bruhmodus = 1*/
                        bruhmodus = TASSE_KLEIN;
                    }
                }
                break;
                /*Wasserschutz*/
            case WASSERSCHUTZ:
                
                /*enter
                flagZustandwechsel*/
                if (flagZustandswechsel)
                {
                    /*outRGB = Rot*/
                    outRGB_R = 1;
                }
                //do
                /*inWasserstand > 20%*/
                if (inWasserstand > MAX_WERT_FULLSENSOR * WASSERSCHUTZ_FULLSTAND_PROZENT / 100)
                {
                    /*zustand_Kochen = Bereit*/
                    zustand_Kochen = BEREIT;
                }
                /*exit
                (Zustand_Kochen nicht zustandAlt_Kochen)
                oder
                (zustand_AnAus nicht An)*/
                if ((zustand_Kochen!=zustandAlt_Kochen)||(zustand_AnAus!=AN))
                {
                    /*outRGB = 0*/
                    outRGB_R = 0;
                }
                break;
            /*Bruhen*/
            case BRUHEN:
                
                /*enter
                flagZustandwechsel*/
                if (flagZustandswechsel)
                {
                    /*Timer = 4s * Bruhmodus
                    outRGB = Grun
                    flagAtmen = 1
                    anzahlGebruht = anzahlGebruht + 1
                    fortschrittsbalken = 1*/
                    inTimer             = inZeit_ms;
                    outRGB_G            = 1;
                    flagAtmen           = 1;
                    flagFortschrittsbalken  = 1;
                    anzahlGebruht       = anzahlGebruht + 1;
                }
                //do
                /*Timer Abgelaufen*/
                if((inZeit_ms - inTimer) >= (REF_ZEIT_ms * bruhmodus))
                {
                    /*anzahlGebruht >= 5*/
                    if (anzahlGebruht >= ANZAHL_DURCHGANGE_BEVOR_REINIGUNG)
                    {
                        /*zustand_Kochen = Reinigung Erforderlich*/
                        zustand_Kochen = REINIGUNG_ERFORDERLICH;
                    }
                    else
                    {
                        /*zustand_Kochen = Bereit*/
                        zustand_Kochen = BEREIT;
                    }
                }
                /*exit
                (Zustand_Kochen nicht zustandAlt_Kochen)
                oder
                (zustand_AnAus nicht An)*/
                if ((zustand_Kochen!=zustandAlt_Kochen)||(zustand_AnAus!=AN))
                {
                    /*outRGB = 0
                    outLed = 0
                    flagAtmen = 0
                    Fortschrittsbalken = 0*/
                    outRGB_G            = 0;
                    outLed              = 0;
                    outLed_Balken       = 0;
                    flagAtmen           = 0;
                    flagFortschrittsbalken  = 0;
                }
                break;
            /*Reinigung Erforderlich*/
            case REINIGUNG_ERFORDERLICH:
                
                /*enter
                flagZustandwechsel*/
                if (flagZustandswechsel)
                {
                    /*outRGB = Gelb*/
                    outRGB_R    = 1;
                    outRGB_G    = 1;
                    
                }
                //do
                /*inPosTaster_4_ReinigungBeendet = 1*/
                if (inPosTaster_4_ReinigungBeendet)
                {
                    /*zustand_Kochen = Bereit*/
                    zustand_Kochen = BEREIT;
                    /*anzahlGebruht = 0*/
                    anzahlGebruht   = 0;
                }
                /*exit
                (Zustand_Kochen nicht zustandAlt_Kochen)
                oder
                (zustand_AnAus nicht An)*/
                if ((zustand_Kochen!=zustandAlt_Kochen)||(zustand_AnAus!=AN))
                {
                    /*outRGB = 0*/
                    outRGB_R = 0;
                    outRGB_G = 0;
                }
                break;
            }
            break;
        }
             
        //Ausgabe____________________________________________________________________________________________________________________________________________________________  
        /*Logik Blinken*/
        /*flagBlinken = 1*/
        if (flagBlinkenRGB)
        {
            /*Periodenzeit abgelaufen*/
            if (inZeit_ms - inZeit_blinken_ms >= PERIODE_BLINKEN_ms)
            {
                /*inZeit_blinken_ms = inZeit_ms*/
                inZeit_blinken_ms = inZeit_ms;
            }
            /*Halbe Periodenzeit abgelaufen*/
            if (inZeit_ms - inZeit_blinken_ms >= PERIODE_BLINKEN_ms/2)
            {
                /*Rgb Helligkeit = 0*/
                outRGBHelligkeit = 0;
            }
            else
            {
               /*Rgb Helligkeit = Max Helligkeit*/
               outRGBHelligkeit = MAX_HELLIGKEIT_RGB;                 
            }
        }
        else 
        {
            /*inZeit_blinken_ms = inZeit_ms*/
            inZeit_blinken_ms = inZeit_ms;
        }
        
        /*Logik Atmen*/
        /*flagAtmen = 1*/
        if (flagAtmen)
        {
            /*1 ms Vergangen*/
            if (inZeit_ms - inZeit_Atmen_ms >= 1)
            {
                /*flag_Aus_Einatmen = Ausatmen*/
                if (flag_Aus_Einatmen == AUSATMEN)
                {
                    /*RGB Helligkeit -1*/
                    outRGBHelligkeit -= 1;
                }
                else
                {
                    /*RGB Helligkeit +1*/
                    outRGBHelligkeit += 1;                                         
                }
                /*RGB helligkeit >= Max Helligkeit*/
                if (outRGBHelligkeit >= MAX_HELLIGKEIT_RGB)
                {
                    /*Ausatmen in flag_Aus_Einatmen speichern*/
                    flag_Aus_Einatmen = AUSATMEN;
                }
                /*RGB helligkeit <= 0*/
                if (outRGBHelligkeit <= 0)
                {
                    /*Einatmen in flag_Aus_Einatmen speichern*/
                    flag_Aus_Einatmen = EINATMEN;                    
                }
                /*inZeit_Atmen_ms = inZeit_ms*/
                inZeit_Atmen_ms = inZeit_ms;
            }
            
            else
            {
                /*inZeit_Atmen_ms = inZeit_ms*/
                inZeit_Atmen_ms = inZeit_ms;
            }
        }
        
        /*flagAtmen und flagBlinken nicht wahr*/
        if (!(flagAtmen||flagBlinkenRGB))
        {
            /*RGB Helligkeit = Max Helligkeit*/
            outRGBHelligkeit = MAX_HELLIGKEIT_RGB;            
        }
        
        /*Logik Fortschrittbalken*/
        /*flagFortschrittsbalken = 1*/
        if (flagFortschrittsbalken)
        {
            /*Periode Lauflicht vergangen*/
            if (inZeit_ms - inZeit_lauflicht_ms >= PERIODE_LAUFLICHT_ms)
            {
                /*groesse_LedBalken + 1*/
                groesse_LedBalken = groesse_LedBalken + 1;
                /*inZeit_lauflicht_ms = inZeit_ms*/
                inZeit_lauflicht_ms = inZeit_ms;
            }
            /*
            Balken verschoben um die Maximal Grösse des Balkens
            minus groesse des Balkens + 1
            maskiert mit max grösse balken
            in outLed_Balken Gespeichert
            */
            outLed_Balken = (0xFFFF >> (MAX_GROESSE_BALKEN - (groesse_LedBalken+1))) & 0xFFFF;
        }
        else
        {
            /*Grösse led Balken = 0
            inZeit_lauflicht_ms = inZeit_ms*/
            groesse_LedBalken = 0;
            inZeit_lauflicht_ms = inZeit_ms;            
        }     
        outLed = outLed_Balken;
        
        /*outLed an den Led's ausgeben
        outRGB an der RGB Led ausgeben*/
        
        ledWriteAll(outLed);
        rgbWrite(outRGB_R * outRGBHelligkeit, outRGB_G * outRGBHelligkeit, outRGB_B * outRGBHelligkeit);              
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

