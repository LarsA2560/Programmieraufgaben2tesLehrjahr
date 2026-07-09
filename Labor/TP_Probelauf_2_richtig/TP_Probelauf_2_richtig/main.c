/*
 * TP_Probelauf_2.cpp
 *
 * Created: 03/06/2026 13:07:42
 * Author : Lars
 */ 

 #include "ucBoardDriver.h"
 #define AUS (0)
 #define EIN (1)
 #define POWER_SWITCH (0b00000001)
 #define STATION_EINS_SECH_KW (0b00000100)
 #define STATION_EINS_ELF_KW (0b00001000)
  #define STATION_ZWEI_SECH_KW (0b00010000)
  #define STATION_EINS_ELF_KW (0b00100000)
  #define SCHALTER_LOAD_MAX (0b01000000)
  #define SCHALTER_LOAD_REDUCED (0b10000000)
  #define LOAD_MAXIMUM (0)
  #define LOAD_REDUCED (1)
  #define BLOCKIERT (2)
  #define FEHLER (3)
  #define ERROR (4)
  #define DREI_VIERTEL (750)
  #define EIN_VIERTEL (250)
   


int main(void)
{
    initBoard(0);// initialisierung des Boards
    
    //alle variabeln werden eigelesen und auf 0 gesetzt
    uint8_t schalter = 0;
    uint8_t onOff_modes = 0;
    uint8_t power_switch = 0;
    uint8_t station_one_six_kW = 0;
    uint8_t station_one_eleven_kW = 0;
    uint8_t station_two_six_kW = 0;
    uint8_t station_two_eleven_kW = 0;
    uint8_t energy_output = 0;
    uint8_t load_max = 0;
    uint8_t load_reduced = 0;
    uint8_t power_led = 0;
    uint8_t block_flag_six_kW = 0;
    uint8_t block_flag_eleven_kw = 0;
    uint64_t inZeit_ms = 0;
    uint64_t timer = 0;
    uint8_t entry = 0;
    uint8_t blink = 0;
    uint8_t led_station_one_six_kW_ok = 0;
    uint8_t led_station_one_eleven_kW_ok = 0;
    uint8_t led_station_two_six_kW__ok = 0;
    uint8_t led_station_two_eleven_kW_ok = 0;
    uint8_t led_load_max = 0;
    uint8_t led_load_reduced = 0;
    uint8_t led_blockiert = 0;

    
    while (1) {
//Eingabe----------------------------------------------------------------------------------------------------------
       schalter = PINC; //alle schalter werden eingelesen
       power_switch = (schalter & POWER_SWITCH)>0; //power schalter wird eingelesen
       station_one_six_kW = (schalter & STATION_EINS_SECH_KW)>0; //schalter Station 1 6kW wird eingelesen
       station_one_eleven_kW = (schalter & STATION_EINS_ELF_KW)>0; //schalter Station 1 11kW wird eingelesen
       station_two_six_kW = (schalter & STATION_ZWEI_SECH_KW)>0; //schalter Station 2 6kW wird eingelesen
       station_two_eleven_kW = (schalter & STATION_EINS_ELF_KW)>0; //schalter Station 2 11kW wird eingelesen
       load_max = (schalter & SCHALTER_LOAD_MAX)>0; // schalter load maximum wird eingelesen
       load_reduced = (schalter & SCHALTER_LOAD_REDUCED)>0; // schalter load reduced wird eingelesen
       inZeit_ms = getSystemTimeMs(); //saystemzeit wird abgespeichert

//Verarbeitung------------------------------------------------------------------------
        switch (onOff_modes){ //OnOff zustand switch case
            case AUS: // Zustand AUS alles wird deaktiviert
                power_led = 0;
                led_station_one_eleven_kW_ok = 0;
                led_station_two_eleven_kW_ok = 0;
                led_station_one_six_kW_ok = 0;
                led_station_two_six_kW__ok = 0;
                led_load_max = 0;
                led_load_reduced = 0;
                led_blockiert = 0;
                //  wenn der power schalter aktiv ist wird der OnOff zustand auf "EIN" gestellt
                if(power_switch){ 
                    onOff_modes = EIN;
                }
            break;
            case EIN: //Zustand EIn der Normalbetrieb der schaltung
                // schaltet die schaltun wieder aus falls der power switch deaktiviert wurde
                if(!power_switch){
                    onOff_modes = AUS;
                }
                // aktiviert die power led falls der energy_output zustand aktuell nicht im Fehlermodus ist
                if(!(energy_output==FEHLER)){
                    power_led = 1;
                    entry = 0;
                }
                //load maximum und reduced sind beide aktiv und aktivieren den error modus
                if (load_max && load_reduced){
                    energy_output = ERROR;
                }
                //load maximum und reduced sind beide deaktiviert die schaltung wird blockiert
                else if(!load_max && !load_reduced){
                    energy_output = BLOCKIERT;
                }
                //beide schalter bei station 1 oder bei Station 2 oder beides gleichzeitig sind gedrückt und aktivieren den fehlermodus
                else if((station_one_six_kW && station_one_eleven_kW) | (station_two_eleven_kW && station_two_six_kW)){
                    energy_output = FEHLER;
                }
                //nur load maximum ist aktiviert deshalb wird ist der output für die stationen auf dem maximum
                else if(load_max && !load_reduced){
                    energy_output = LOAD_MAXIMUM;
                }
                //nur load reduced ist aktiviert deshalbt können die Stationen nur 6kW ziehen
                else if(load_reduced && !load_max){
                    energy_output = LOAD_REDUCED;
                }
                //logikkontrolle damit die LED aktiviert werden der einzelnen stationen--------------------------------------------------------
                
                // aktiviert station 1 auf 11kW wenn der entsprechende schalter aktiv ist und der load auf dem maximum ist
                led_station_one_eleven_kW_ok = station_one_eleven_kW && !block_flag_eleven_kw;
                 // aktiviert station 2 auf 11kW wenn der entsprechende schalter aktiv ist und der load auf dem maximum ist
                led_station_two_eleven_kW_ok = station_two_eleven_kW && !block_flag_eleven_kw;
                //aktiviert Station 1 auf 6kW wenn der entsprechende schalter aktiv ist und die schaltung nicht blockiert ist oder weil man den schalter für 
                //11kW aktiviert hat und der load aber nur noch reduced ist
                led_station_one_six_kW_ok = (station_one_eleven_kW && block_flag_eleven_kw && !block_flag_six_kW) | (station_one_six_kW && !block_flag_six_kW);
                //aktiviert Station 2 auf 6kW wenn der entsprechende schalter aktiv ist und die schaltung nicht blockiert ist oder weil man den schalter für
                //11kW aktiviert hat und der load aber nur noch reduced ist
                led_station_two_six_kW__ok = (station_two_eleven_kW && block_flag_eleven_kw && !block_flag_six_kW) | (station_two_six_kW && !block_flag_six_kW);
                
                //switch case für die entsprechenden output zustände (maximum und reduced load) und die fehlerzustände
                switch (energy_output){
                    case 0: // load maximum // power led und load max led leuchten und je nach ladegeschwindigkeit werden die entsprechen led's angezeigt
                        block_flag_eleven_kw = 0;
                        block_flag_six_kW = 0;
                        led_load_max = 1;
                        led_load_reduced = 0;
                        led_blockiert = 0;
                    break;
                    case 1: // load reduziert //led reduzuert und die power led leuchten und je nach auswahl der taster bei den Stationen wird bei Station 1 oder 2 6kW oder gar nichts ausgegeben
                        block_flag_eleven_kw = 1;
                        block_flag_six_kW = 0;  
                        led_load_max = 0;
                        led_load_reduced = 1;
                        led_blockiert = 0;                      
                    break;
                    case 2: // schaltun ist blockiert led blockiert leuchtet sowie die power led sonst nichts
                        block_flag_eleven_kw = 1;
                        block_flag_six_kW = 1;    
                        led_load_max = 0;
                        led_load_reduced = 0;
                        led_blockiert = 1;                
                    break;
                    case 3: //fehlermodus die power led blinkt der rest alles deaktiviert
                        block_flag_eleven_kw = 1;
                        block_flag_six_kW = 1;
                        led_load_max = 0;
                        led_load_reduced = 0;
                        led_blockiert = 0;
                        if(!entry){//aktiviert beim ersten man den timer und schaltet die LED aus für das 1/3 verhältnis
                            power_led = 0;
                            timer = inZeit_ms;
                            entry = 1;
                        }
                        //nach 750ms (0.75s) wird die led eingeschaltet also invertiert
                        if(!blink && ((inZeit_ms-timer)>=DREI_VIERTEL)){
                            power_led = power_led ^ 1;
                            timer = inZeit_ms;
                            blink = 1;
                        }
                        //nach 250ms (0.25s) wird die LEd ausgeschaltet, genaugenommen nochmals invertiert
                        if(blink && ((inZeit_ms-timer)>=EIN_VIERTEL)){
                            power_led = power_led ^ 1;
                            timer = inZeit_ms;
                            blink = 0;
                        }
                    break;
                    case 4://Error modus alles wird deaktiviert aussert die power led
                        block_flag_six_kW = 1;
                        block_flag_eleven_kw = 1;
                        led_load_max = 0;
                        led_load_reduced = 0;
                        led_blockiert = 0;
                    break;
                }              
            break;
        }
//Ausgabe------------------------------------------------------------------------------------
//alle variabeln werden am Port a ausgegeben und sind bei den LED's sichtbar
ledWriteAll(power_led | led_station_one_eleven_kW_ok<<5 | led_station_two_eleven_kW_ok <<7 | led_station_one_six_kW_ok<<4 | led_station_two_six_kW__ok<<6 | led_load_max<<1 | led_load_reduced<<2 | led_blockiert<<3);
    }
}

