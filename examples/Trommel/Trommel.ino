
// gamut inc´s Trommel
// v11.00 29.07.2020
// www.gamutinc.org

#include <Solenoid.h>
//#include <SoftPWM.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define  WIRE 1
#define  MID  2
#define CONNECTION MID  

#define MIDI_CHANNEL 1

#define SOLENOIDS 15
#define STD_MIN_LENGTH 3
#define STD_MAX_LENGTH 20

                  // BEATER or DAMPER,  MIDI_trigger, PIN,  min_length,        max_length,       delay
Solenoid L_aussen (  BEATER,            48,           35,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // c2
Solenoid L_innen  (  BEATER,            49,           34,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // c#2
Solenoid mitte    (  CONTINOUS,         50,           05,   1,                 255,              0      ); // d2
Solenoid R_innen  (  BEATER,            51,           36,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // d#2
Solenoid R_aussen (  BEATER,            52,           03,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // e2

Solenoid g1_bottom(  BEATER,            53,           29,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // f2
Solenoid g1_mid   (  BEATER,            54,           32,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // f#2
Solenoid g1_top   (  BEATER,            55,           37,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // g2

Solenoid g2_bottom(  BEATER,            56,           33,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // g#2
Solenoid g2_mid   (  BEATER,            57,           31,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // a2
Solenoid g2_top   (  BEATER,            58,           30,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // a#2

Solenoid g3_bottom(  BEATER,            59,           28,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // b2
Solenoid g3_mid   (  BEATER,            60,           27,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // c3
Solenoid g3_top   (  BEATER,            61,           26,   STD_MIN_LENGTH,    STD_MAX_LENGTH,   0      ); // c#

Solenoid LED      (  CONTINOUS,         72,           02,   1,                 255,              0      ); // d4

// THE SOLENOID ARRAY
Solenoid solenoids[SOLENOIDS] = {L_aussen, L_innen, mitte, R_innen, R_aussen, g1_top, g1_mid, g1_bottom, g2_top, g2_mid, g2_bottom, g3_top, g3_mid, g3_bottom, LED};

void setup() {
  
//  SoftPWMBegin();  
//  SoftPWMSet(6, 0);
//  SoftPWMSetFadeTime(6, 1000, 1000);
  
  MIDI.begin(MIDI_CHANNEL);
  
  switch(CONNECTION)
  {
    case WIRE: Serial.begin(115200);
    break;
    case MID: Serial.begin(31250);
    break;
  }
}

void loop() {
// CONTROL
 
  if (MIDI.read() > 0) for(int i = 0; i< SOLENOIDS; i++) solenoids[i].control(MIDI.getType(), MIDI.getData1(), MIDI.getData2());  
  
// PLAY 
 
  for(int i = 0; i< SOLENOIDS; i++) solenoids[i].process();
}
