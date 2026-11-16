
// gamut inc´s Solenoid Library
// v11.00 29.07.2020
// www.gamutinc.org

#include <Solenoid.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define  WIRE 1
#define  MID  2
#define CONNECTION MID 

// ÄNDERUNGEN AB HIER 
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define MIDI_CHANNEL 1 // auf welchen Kanal empfangen wir? 

#define SOLENOIDS 2 // wieviele Magnete? 
#define STD_MIN_LENGTH 3 // standartmäßige minimalzeit Bestromung in millisekunden 
#define STD_MAX_LENGTH 20 // standartmäßige maximalzeit Bestromung in millisekunden 

                            // BEATER or DAMPER  MIDI_triggerNote  PIN   Bestromung ms bei Velo 1           Bestromung ms bei Velo 127  delay, falls verzögerte Auslösung gewünscht (zB um es mit anderen Instrumenten zu synchronisieren)
Solenoid EinMagnet (          BEATER,            60,               1,    STD_MIN_LENGTH,                     STD_MAX_LENGTH,             0      ); // c3
Solenoid EinAndererMagnet  (  BEATER,            61,               2,    STD_MIN_LENGTH,                     STD_MAX_LENGTH,             0      ); // c#3


// hier 
Solenoid solenoids[SOLENOIDS] = {EinMagnet, EinAndererMagnet};

// ÄNDERUGNEN BIS HIER 
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void setup() {
  
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
