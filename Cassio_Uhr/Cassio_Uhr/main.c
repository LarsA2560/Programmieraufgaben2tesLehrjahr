/*
 * Cassio_Uhr.c
 *
 * Created: 05/02/2026 11:52:55
 * Author : Lars
 */ 
//#pragma GCC optimize 0

#include "ucBoardDriver.h"


//Maske_IN
#define MASK_IN_TASTER_LIGHT        (1<<0)
#define MASK_IN_TASTER_MODE         (1<<1)
#define MASK_IN_TASTER_FUNCTION     (1<<6)
#define SONNTAG     (6)
#define MONTAG      (0)
#define AlARM       (7)
#define STOPPUHR    (8)

#define MASK_OUT_MAX_BRIGHTNESS_LCD (255)
#define MASK_BRIGHTNESS_ALARM       (1023)
#define ZWANZIG_SEKUNDEN_TIMER      (20000)
#define HUNDERTSTEL_SEKUNDEN_TIMER  (10)
#define MASK_BLINKEN_PERIODENZEIT   (1000)
#define MASK_BLINKEN_ANZEIT         (250)

#define MASK_SCHRITTGROSSE_s  (1)
#define MASK_SCHRITTGROSSE_m  (1)
#define MASK_SCHRITTGROSSE_h  (1)
#define MASK_SCHRITTGROSSE_d  (1)
#define MASK_SCHRITTGROSSE_M  (1)
#define MASK_SCHRITTGROSSE_w  (1)
#define MASK_SCHRITTGROSSE_hs (1)
#define MASK_SCHRITTGESCHWINDIGKEIT_ms (1000)

#define MASK_HS_S   (100)
#define MASK_S_M    (60)
#define MASK_M_H    (60)
#define MASK_H_D    (24)
#define MASK_M_Y    (12)

#define MASK_AM_PM_FILTER               (2)
#define MASK_AM_PM_INDIKATOR_ENTFERNEN  (10)
#define MASK_FUNKTION_AM                (0)
#define MASK_FUNKTION_PM                (1)
#define MASK_FUNKTION_24h               (2)

int main(void)
{
    initBoard(0);
    //Einlese/Ausgabe Variablen
    char    * MMM[3] =                      { "AM ","PM ","24h"};
    char    * DD[9] =                       { "MO","TU","WE","TH","FR","SA","SO","AL","ST"};
    const   uint8_t zwolfSunden_UHR[24] =   { 120,10,20,30,40,50,60,70,80,90,100,110,121,11,21,31,41,51,61,71,81,91,101,111};
    typedef enum zustand_mode_t             {NORMALZEITANZEIGE, TAGLICHER_ALARM, STOPPUHRFUNKTION, ZEIT_KALENDER_EINSTELLFUNKTION}                                                      zustand_mode_t;
    typedef enum alarmzustand_t             {AlARMZUSTAND_ALARM_ZEIT,AlARMZUSTAND_AUS,AlARMZUSTAND_ALARM,AlARMZUSTAND_ZEIT}                                                             alarmzustand_t;
    typedef enum zustand_alarm_einstellen_t {ALARM_EINSTELLEN,STUNDENEINSTELLUNG,MINUTENEINSTELLUNG}                                                                                    zustand_alarm_einstellen_t;
    typedef enum zustand_zeiteinstellung_t  {SEKUNDENEINSTELLUNG,STUNDENEINSTELLUNG_z,MINUTENEINSTELLUNG_z,MONATSEINSTELLUNG,DATUMSEINSTELLUNG,WOCHENTAGSEINSTELLUNG}                   zustand_zeiteinstellung_t;
    zustand_mode_t              zustand_mode                = NORMALZEITANZEIGE;
    alarmzustand_t              alarmzustand                = AlARMZUSTAND_ALARM_ZEIT;
    zustand_alarm_einstellen_t  zustand_alarm_einstellen    = ALARM_EINSTELLEN;
    zustand_zeiteinstellung_t  zustand_zeiteinstellung      = SEKUNDENEINSTELLUNG;
        
    uint8_t dd = 1;
    
    uint8_t hh          = 0;
    uint8_t hh_24       = 0;
    uint8_t mm          = 0;
    uint8_t mmalt       = 0;
    uint8_t ss          = 0;
    uint8_t month       = 1;
    uint8_t hh_st       = 0;
    uint8_t mm_st       = 0;
    uint8_t ss_st       = 0;
    uint8_t hh_al       = 0;
    uint8_t mm_al       = 0;
    uint8_t hh_anzeige  = 0;
    uint8_t mm_anzeige  = 0;
    uint8_t ss_anzeige  = 0;
    
    uint8_t flagAlarmEin                = 0;
    uint8_t flagBeepEin                 = 0;
    uint8_t flag_function               = 0;
    uint8_t flag_Stoppuhr_lauft         = 0;
    uint8_t flag_Zwischenzeit           = 0;
    uint8_t flag_alarmStummschalten     = 0;
    uint8_t flag_Monatanzeige           = 0;
    uint8_t Wochentag                   = 0;
    uint8_t Wochentag_Ausgabe           = 0;
    
    uint8_t inTaster            = 0;
    uint8_t inTaster_alt        = 0;
    uint8_t inPosFlanke_Taster  = 0;
    uint8_t inTaster_Light      = 0;
    uint8_t inTaster_Mode       = 0;
    uint8_t inTaster_Function   = 0;
    uint8_t outSummer           = 0;
    
    uint8_t address_alarm   = 1;
    uint8_t address_zeit    = 2;
    
    uint8_t outLCDbrightness = 0;
    
    uint64_t systemZeit_ms          = 0;
    uint64_t sekunde                = 0;
    uint64_t startTimerStoppuhr     = 0;
    uint64_t startAlarm             = 0;
    uint64_t blinken_toggle         = 0;
    
    uint8_t flag_blinken_Sekunde    = 0;
    uint8_t blinken_Sekunde         = 0;
    uint8_t flag_blinken_Minute     = 0;
    uint8_t blinken_Minute          = 0;
    uint8_t flag_blinken_Stunde     = 0;
    uint8_t blinken_Stunde          = 0;
    uint8_t flag_blinken_Tag        = 0;
    uint8_t blinken_Tag             = 0;
    uint8_t flag_blinken_Wochentag  = 0;
    uint8_t blinken_Wochentag       = 0;
    uint8_t flag_blinken_Monat      = 0;
    uint8_t blinken_Monat           = 0;
    
    uint8_t flag_stoppuhr_load = 0;
    
    const   uint8_t alarmzeichen[]  = {
        0x00,
        0x00,
        0x15,
        0x15,
        0x15,
        0x15,
        0x15,
        0x15
    };
    const   uint8_t zeitsignal[]    ={
        0x00,
        0x04,
        0x0E,
        0x0E,
        0x0E,
        0x1F,
        0x1F,
        0x04
    };
    lcdCreateCustomChar(address_alarm,  alarmzeichen);
    lcdCreateCustomChar(address_zeit,   zeitsignal);
    const uint8_t anzahl_Tage_im_Monat[13] =
    {
        0,31,28,31,30,31,30,31,31,30,31,30,31
    };        
       
    while (1) 
    {
        //Variablem Always Low/High

        //Flankenerkennung
        inTaster_alt        = inTaster;
        inTaster            = buttonReadAllPL();
        inPosFlanke_Taster  = inTaster & ~inTaster_alt;
        //Eingabe____________________________________________________________________________________________________________________________________________________________  
        inTaster_Light      = (inPosFlanke_Taster   & MASK_IN_TASTER_LIGHT)     >0;
        inTaster_Mode       = (inPosFlanke_Taster   & MASK_IN_TASTER_MODE)      >0;
        inTaster_Function   = (inPosFlanke_Taster   & MASK_IN_TASTER_FUNCTION)  >0;
        systemZeit_ms = getSystemTimeMs();     
        //Verarbeitung_______________________________________________________________________________________________________________________________________________________          
        mmalt               = mm;
        Wochentag_Ausgabe   = Wochentag;
        flag_blinken_Sekunde     = 0;
        flag_blinken_Minute      = 0;
        flag_blinken_Stunde      = 0;
        flag_blinken_Tag         = 0;
        flag_blinken_Wochentag   = 0;
        flag_blinken_Monat       = 0;
        
        if (inTaster_Mode)
        {
            zustand_alarm_einstellen    = ALARM_EINSTELLEN;
            zustand_zeiteinstellung     = SEKUNDENEINSTELLUNG;
            zustand_mode                += 1;
            if (zustand_mode > ZEIT_KALENDER_EINSTELLFUNKTION)
            {
                zustand_mode = NORMALZEITANZEIGE;
            }
        }
        if((systemZeit_ms - sekunde) >= MASK_SCHRITTGESCHWINDIGKEIT_ms){
            ss      += MASK_SCHRITTGROSSE_s;
            sekunde = systemZeit_ms;
        }
        if (ss>= MASK_S_M)
        {
            ss = 0;
            mm += MASK_SCHRITTGROSSE_m;
        }
        if (mm>=MASK_M_H)
        {
            mm = 0;
            hh_24 += MASK_SCHRITTGROSSE_h;
        }
        if (hh_24 >= MASK_H_D)
        {
            hh_24       = 0;
            dd          += MASK_SCHRITTGROSSE_d;
            Wochentag   += MASK_SCHRITTGROSSE_w;
        }
        if (Wochentag > SONNTAG)
        {
            Wochentag = MONTAG;
            
        }
        if (dd > anzahl_Tage_im_Monat[month])
        {
            dd = 1;
            month += MASK_SCHRITTGROSSE_M;
        }
        if (month > MASK_M_Y)
        {
            month = 1;
        }
        switch (zustand_mode)
        {
        case NORMALZEITANZEIGE:
            if (inTaster_Light)
            {
                outLCDbrightness = !outLCDbrightness;
            }
            if (inTaster_Function)
            {
                if (flag_function == MASK_FUNKTION_24h)
                {
                    flag_function = MASK_FUNKTION_AM;
                }
                else
                {
                    flag_function = MASK_FUNKTION_24h;
                }
            }
            if (flag_function<2)
            {
                hh              = zwolfSunden_UHR[hh_24];
                flag_function   = (hh % MASK_AM_PM_FILTER);
                hh              = hh/MASK_AM_PM_INDIKATOR_ENTFERNEN;
            
            }
            else{
                hh = hh_24;
            }
            
            hh_anzeige = hh;
            mm_anzeige = mm;
            ss_anzeige = ss;
            Wochentag_Ausgabe = Wochentag;
        	break;
        case TAGLICHER_ALARM:
            Wochentag_Ausgabe = AlARM;
            if (inTaster_Function&&!zustand_alarm_einstellen)
            {
                alarmzustand += 1;
                if (alarmzustand > AlARMZUSTAND_ZEIT)
                {
                    alarmzustand = AlARMZUSTAND_ALARM_ZEIT;
                }
            }
            if (inTaster_Light)
            {
                zustand_alarm_einstellen += 1;
                if (zustand_alarm_einstellen > MINUTENEINSTELLUNG)
                {
                    zustand_alarm_einstellen = ALARM_EINSTELLEN;
                }
            }
            switch (zustand_alarm_einstellen)
            {
            case ALARM_EINSTELLEN:
                break;
            case STUNDENEINSTELLUNG:
                flag_blinken_Stunde = 1;
                if (inTaster_Function)
                {
                    hh_al += MASK_SCHRITTGROSSE_h;
                    if (hh_al >= MASK_H_D)
                    {
                        hh_al = 0;
                    }
                }
                break;
            case MINUTENEINSTELLUNG:
                flag_blinken_Minute = 1;
                if (inTaster_Function)
                {
                    mm_al += MASK_SCHRITTGROSSE_m;
                    if (mm_al >= MASK_M_H)
                    {
                        mm_al = 0;
                    }
                }
                break;
             }
             if (flag_function<2)
             {
                 hh              = zwolfSunden_UHR[hh_al];
                 flag_function   = (hh % MASK_AM_PM_FILTER);
                 hh              = hh/MASK_AM_PM_INDIKATOR_ENTFERNEN;
                 
             }
             else{
                 hh = hh_al;
             }
            hh_anzeige = hh;
            mm_anzeige = mm_al;
            ss_anzeige = 0;
            break;
        case STOPPUHRFUNKTION:
            Wochentag_Ausgabe = STOPPUHR;
            if (inTaster_Function)
            {
                flag_Stoppuhr_lauft = !flag_Stoppuhr_lauft;
            }
            if (flag_Stoppuhr_lauft)
            {
                if((systemZeit_ms - startTimerStoppuhr) >= HUNDERTSTEL_SEKUNDEN_TIMER){
                    ss_st += 1;
                    startTimerStoppuhr = systemZeit_ms;
                }
                if (ss_st >= MASK_HS_S)
                {
                    mm_st += 1;
                    ss_st = 0;
                    startTimerStoppuhr = systemZeit_ms;
                }
                if (mm_st >= MASK_S_M)
                {
                    hh_st += 1;
                    mm_st = 0;
                }
                if (hh_st >= MASK_M_H)
                {
                    hh_st = 0;
                }
                if (inTaster_Light)
                {
                    flag_Zwischenzeit = !flag_Zwischenzeit;
                }
            }
            else{
                
                
                if (inTaster_Light)
                {
                    if (flag_Zwischenzeit)
                    {
                        flag_Zwischenzeit = 0;
                    }
                    else{
                        ss_st = 0;
                        mm_st = 0;
                        hh_st = 0;

                    }
                }
            }
            if ((!flag_Zwischenzeit)||flag_stoppuhr_load)
            {
                hh_anzeige = hh_st;
                mm_anzeige = mm_st;
                ss_anzeige = ss_st;
            }
            
            break;
        case ZEIT_KALENDER_EINSTELLFUNKTION:
            if (inTaster_Light)
            {
                zustand_zeiteinstellung += 1;
                if (zustand_zeiteinstellung > WOCHENTAGSEINSTELLUNG)
                {
                    zustand_zeiteinstellung = SEKUNDENEINSTELLUNG;
                }
            }
            switch (zustand_zeiteinstellung)
            {
            case SEKUNDENEINSTELLUNG:
                flag_Monatanzeige = 0;
                flag_blinken_Sekunde = 1;
                if (inTaster_Function)
                {
                    ss += MASK_SCHRITTGROSSE_s;
                    if (ss >= MASK_S_M)
                    {
                        ss = 0;
                    }                    
                }
                break;
            case  STUNDENEINSTELLUNG_z:
                flag_Monatanzeige = 0;
                flag_blinken_Stunde = 1;
                if (inTaster_Function)
                {
                    hh_24 += MASK_SCHRITTGROSSE_h;
                    if (hh_24 >= MASK_H_D)
                    {
                        hh_24 = 0;
                    }
                }
                break;
            case MINUTENEINSTELLUNG_z:
                flag_Monatanzeige = 0;
                flag_blinken_Minute = 1;
                if (inTaster_Function)
                {
                    mm += MASK_SCHRITTGROSSE_m;
                    if (mm >= MASK_M_H)
                    {
                        mm = 0;
                    }
                }
                break;
            case MONATSEINSTELLUNG:
                flag_Monatanzeige = 1;
                flag_blinken_Monat = 1;
                if (inTaster_Function)
                {
                    month += MASK_SCHRITTGROSSE_M;
                    if (month > MASK_M_Y)
                    {
                        month = 1;
                    }
                }
                break;
            case DATUMSEINSTELLUNG:
                flag_Monatanzeige = 1;
                flag_blinken_Tag = 1;
                if (inTaster_Function)
                {
                    dd += MASK_SCHRITTGROSSE_d;
                    if (dd>anzahl_Tage_im_Monat[month])
                    {
                        dd=1;
                    }
                }
                break;            
            case WOCHENTAGSEINSTELLUNG:
                flag_Monatanzeige = 1;
                flag_blinken_Wochentag = 1;
                if (inTaster_Function)
                {
                    Wochentag += MASK_SCHRITTGROSSE_w;
                    if (Wochentag > SONNTAG)
                    {
                        Wochentag = MONTAG;
                    }                    
                }
                break;
            }
            if (!flag_Monatanzeige)
            {
                if (flag_function<2)
                {
                    hh              = zwolfSunden_UHR[hh_24];
                    flag_function   = (hh % MASK_AM_PM_FILTER);
                    hh              = hh/MASK_AM_PM_INDIKATOR_ENTFERNEN;
                    
                }
                else{
                    hh = hh_24;
                }
                hh_anzeige = hh;
                mm_anzeige = mm;
                ss_anzeige = ss;
                Wochentag_Ausgabe = Wochentag;
            }
            
            break;
    	}
        switch (alarmzustand)
        {
        case AlARMZUSTAND_ALARM_ZEIT:
            lcdWriteText(2,0,"%c %c",address_alarm,address_zeit);
            flagAlarmEin    = 1;
            flagBeepEin     = 1;
        	break;
        case AlARMZUSTAND_AUS:
            lcdWriteText(2,0,"        ");
            flagAlarmEin    = 0;
            flagBeepEin     = 0;
            break;
        case AlARMZUSTAND_ALARM:
            lcdWriteText(2,0,"%c   ",address_alarm);
            flagAlarmEin    = 1;
            flagBeepEin     = 0;
            break;
        case AlARMZUSTAND_ZEIT:
            lcdWriteText(2,0,"  %c",address_zeit);
            flagAlarmEin    = 0;
            flagBeepEin     = 1;
            break; 
        }
        if (flagAlarmEin)
        {
            if ((hh_24==hh_al)&&(mm==mm_al)&&(!flag_alarmStummschalten))
            {
                outSummer = 1;
            }
        }
        if (flagBeepEin)
        {
            if ((mm==00)&&(ss==00)&&(!flag_alarmStummschalten))
            {
                outSummer = 1;
            }
        }
        
        if (outSummer)
        {
            if (inTaster_Light)
            {
                outSummer               = 0;
                flag_alarmStummschalten = 1;
            }
            if((systemZeit_ms - startAlarm) >= ZWANZIG_SEKUNDEN_TIMER){
                outSummer               = 0;
                flag_alarmStummschalten = 1;
            }
        }
        else{startAlarm = systemZeit_ms;}
        
        if (mm == !mmalt)
        {
            flag_alarmStummschalten = 0;
        }
            
        if (!flagAlarmEin&&!flagBeepEin)
        {
            outSummer = 0;
        }
        if (zustand_mode == STOPPUHRFUNKTION)
        {
            flag_stoppuhr_load = 0;
        }
        else{flag_stoppuhr_load = 1;}
        //Ausgabe____________________________________________________________________________________________________________________________________________________________
        lcdLight    (outLCDbrightness   * MASK_OUT_MAX_BRIGHTNESS_LCD   );
        rgbRot      (outSummer          * MASK_BRIGHTNESS_ALARM         );
        if ((zustand_mode == TAGLICHER_ALARM)&&(!blinken_Minute)&&(!blinken_Stunde))
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            lcdWriteText(1,0,"%2u:%02u       ",hh_anzeige,mm_anzeige );
        }
        if (!flag_blinken_Sekunde&&!flag_blinken_Minute&&!flag_blinken_Stunde&&!flag_blinken_Tag&&!flag_blinken_Wochentag&&!flag_blinken_Monat&&!(zustand_mode==TAGLICHER_ALARM))
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            lcdWriteText(1,0,"%2u:%02u:%02u",hh_anzeige,mm_anzeige,ss_anzeige);
            
        }
        
        if((systemZeit_ms - blinken_toggle) >= MASK_BLINKEN_PERIODENZEIT)
        {
            blinken_toggle = systemZeit_ms;
        }   
        if((systemZeit_ms - blinken_toggle) <= MASK_BLINKEN_ANZEIT){
            blinken_Sekunde     = 1;
            blinken_Minute      = 1;
            blinken_Stunde      = 1;
            blinken_Tag         = 1;
            blinken_Wochentag   = 1;
            blinken_Monat       = 1;
        }
        else {
            blinken_Sekunde     = 0;
            blinken_Minute      = 0;
            blinken_Stunde      = 0;
            blinken_Tag         = 0;
            blinken_Wochentag   = 0;
            blinken_Monat       = 0;
        }
        
        if (flag_blinken_Sekunde)
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            if (blinken_Sekunde)
            {
                lcdWriteText(1,0,"%2u:%02u:  ",hh_anzeige,mm_anzeige);
            }
            else
            {
                lcdWriteText(1,0,"%2u:%02u:%02u",hh_anzeige,mm_anzeige,ss_anzeige);
            }
        }
        if (flag_blinken_Minute)
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            if (zustand_mode == TAGLICHER_ALARM)
            {
                if (blinken_Minute)
                {
                    lcdWriteText(1,0,"%2u:         ",hh_anzeige );
                }
                else
                {
                    lcdWriteText(1,0,"%2u:%02u       ",hh_anzeige,mm_anzeige );
                }
            }
            else{
                if (blinken_Minute)
                {
                    lcdWriteText(1,0,"%2u:  :%02u",hh_anzeige,ss_anzeige);
                }
                else
                {
                    lcdWriteText(1,0,"%2u:%02u:%02u",hh_anzeige,mm_anzeige,ss_anzeige);
                } 
            }                      
        }
        if (flag_blinken_Stunde)
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            if (zustand_mode == TAGLICHER_ALARM)
            {
                if (blinken_Stunde)
                {
                    lcdWriteText(1,0,"  :%^02u       ",mm_anzeige);
                }
                else
                {
                    lcdWriteText(1,0,"%2u:%02u       ",hh_anzeige,mm_anzeige);
                }
            }
            else{
                if (blinken_Stunde)
                {
                    lcdWriteText(1,0,"  :%02u:%02u",mm_anzeige,ss_anzeige);
                }
                else
                {
                    lcdWriteText(1,0,"%2u:%02u:%02u",hh_anzeige,mm_anzeige,ss_anzeige);
                }
            }
        }
        if (flag_blinken_Tag)
        {
            lcdWriteText(1,0,"%2u               ",month);
            if (blinken_Tag)
            {
                lcdWriteText(0,0,"%s %s   ",MMM[flag_function],DD[Wochentag_Ausgabe]);
            }
            else{
                lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            }
        }
        if (flag_blinken_Wochentag)
        {
            lcdWriteText(1,0,"%2u               ",month);
            if (blinken_Wochentag)
            {
                lcdWriteText(0,0,"%s    %2u",MMM[flag_function],dd);
            }
            else{
                lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            }
        }
        if (flag_blinken_Monat)
        {
            lcdWriteText(0,0,"%s %s %2u",MMM[flag_function],DD[Wochentag_Ausgabe],dd);
            if (blinken_Monat)
            {
                lcdWriteText(1,0,"                 ");
            }
            else{
                lcdWriteText(1,0,"%2u               ",month);
            }
        }
        //Warten_____________________________________________________________________________________________________________________________________________________________
    }
}

