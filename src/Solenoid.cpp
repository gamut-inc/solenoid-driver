#include "Solenoid.h"
#include <Arduino.h>

#define TIMESCALE 1000  

static solenoid_t solenoids[MAX_SOLENOIDS];					 // static array for solenoid values
int SolenoidCount = 0;							    	           // number of Solenoids connected

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // INIT // 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Solenoid::Solenoid(int type, int trigger, int pin, double min_length, double max_length, double delaytime)
{
    this->SolenoidIndex = SolenoidCount++;			    // assign an solenoid index to this instance
    pinMode(pin, OUTPUT);				    	    // set pin
    digitalWrite(pin, LOW);
    
    solenoids[this->SolenoidIndex].type = type;
    solenoids[this->SolenoidIndex].mode = STANDART;    
    solenoids[this->SolenoidIndex].trigger = trigger;	
    solenoids[this->SolenoidIndex].pin = pin;	    
    solenoids[this->SolenoidIndex].min_length = min_length;	
    solenoids[this->SolenoidIndex].max_length = max_length;	
    solenoids[this->SolenoidIndex].delaytime = delaytime;
    solenoids[this->SolenoidIndex].starttime = 0;
    solenoids[this->SolenoidIndex].stoptime = 1;
    solenoids[this->SolenoidIndex].active = LOW;	   
    solenoids[this->SolenoidIndex].damp_length = max_length;         
}    

Solenoid::Solenoid(int type, int trigger, int pin, double min_length, double max_length, double damper_length, double delaytime)
{
    this->SolenoidIndex = SolenoidCount++;			    // assign an solenoid index to this instance
    pinMode(pin, OUTPUT);				    	    // set pin
    digitalWrite(pin, LOW);
    
    solenoids[this->SolenoidIndex].type = type;
    solenoids[this->SolenoidIndex].damper_length = damper_length;
    solenoids[this->SolenoidIndex].mode = STANDART;    
    solenoids[this->SolenoidIndex].trigger = trigger;	
    solenoids[this->SolenoidIndex].pin = pin;	    
    solenoids[this->SolenoidIndex].min_length = min_length;	
    solenoids[this->SolenoidIndex].max_length = max_length;	
    solenoids[this->SolenoidIndex].delaytime = delaytime;
    solenoids[this->SolenoidIndex].starttime = 0;
    solenoids[this->SolenoidIndex].stoptime = 1;	
    solenoids[this->SolenoidIndex].active = LOW;
    solenoids[this->SolenoidIndex].damp_length = max_length;      
}    

Solenoid::Solenoid(int type, int trigger, int pin, double max_length, double delaytime)
{
    this->SolenoidIndex = SolenoidCount++;			    // assign an solenoid index to this instance
    pinMode(pin, OUTPUT);				    	    // set pin
    digitalWrite(pin, LOW);
    
    solenoids[this->SolenoidIndex].type = type;
    solenoids[this->SolenoidIndex].mode = STANDART; 
    solenoids[this->SolenoidIndex].trigger = trigger;	
    solenoids[this->SolenoidIndex].pin = pin;	    	
    solenoids[this->SolenoidIndex].max_length = max_length;	
    solenoids[this->SolenoidIndex].delaytime = delaytime;
    solenoids[this->SolenoidIndex].starttime = 0;
    solenoids[this->SolenoidIndex].stoptime = 1;	
    solenoids[this->SolenoidIndex].active = LOW;	
    solenoids[this->SolenoidIndex].damp_length = max_length;      	    
}    
  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL // CONTROL 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Solenoid::control(int MIDItype, int trigger, int velocity){

// MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES // MODES 
  
          if(trigger == 0 && MIDItype == 144) solenoids[this->SolenoidIndex].mode = STANDART; // CHANGE DAMPER MODE ON MIDI NOTE C0, C#0, D0 
          else if(trigger == 1 && MIDItype == 144) solenoids[this->SolenoidIndex].mode = FLOATING; 
          else if(trigger == 2 && MIDItype == 144) solenoids[this->SolenoidIndex].mode = DAMPED; 
          else if(trigger==123) analogWrite(solenoids[this->SolenoidIndex].pin, LOW); // security when live is turned off;
          
          else if(trigger == solenoids[this->SolenoidIndex].trigger) // COMPLEMENTARY MIDI TRIGGER?
          
          { 
            
            switch(solenoids[this->SolenoidIndex].type) // BEATER OR DAMPER?
            {
              case BEATER:
              {
                
                if(solenoids[this->SolenoidIndex].mode == DAMPED)
                {
                  
                solenoids[this->SolenoidIndex].delaytime = solenoids[this->SolenoidIndex].damp_length;
                if(MIDItype == 144 && velocity > 0) solenoid_on(velocity);
                else if(MIDItype == 128 || velocity == 0) solenoid_off();
                }
                
                else{
                solenoids[this->SolenoidIndex].delaytime = 0;
                if(MIDItype == 144 && velocity > 0) solenoid_on(velocity);
                else if(MIDItype == 128 || velocity == 0) solenoid_off();
                }
                
              }break;
              
// DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER // DAMPER //               
              
              case DAMPER:
              {
                switch(solenoids[this->SolenoidIndex].mode){ // WHICH DAMPER MODE?
                  
                case STANDART:
                {
                  if(MIDItype == 144) {
                    if (velocity == 0) solenoid_on(solenoids[this->SolenoidIndex].max_length*TIMESCALE);
                    else solenoid_off();
                  }
                  else if(MIDItype == 128) solenoid_on(velocity);
                }break;
  
                case FLOATING:
                {
             //     if(MIDItype == 144 && velocity > 0) solenoid_off();
                }break;
                
                case DAMPED:
                {
                  if(MIDItype == 144 && velocity > 0) solenoid_on(solenoids[this->SolenoidIndex].damp_length*2);
                  else if(MIDItype == 128 || velocity == 0) solenoid_off();
                }break;
                } 
              }break;     

 // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS  // CONTINOUS 
            
              case CONTINOUS:
              {
                if(MIDItype == 144 || solenoids[this->SolenoidIndex].active == LOW){  
                  analogWrite(solenoids[this->SolenoidIndex].pin, map(velocity, 2, 128, solenoids[this->SolenoidIndex].min_length, solenoids[this->SolenoidIndex].max_length));
                  solenoids[this->SolenoidIndex].active = HIGH;
                } 
                if (MIDItype == 176 || solenoids[this->SolenoidIndex].active == HIGH) {
                   analogWrite(solenoids[this->SolenoidIndex].pin, map(velocity, 2, 128, solenoids[this->SolenoidIndex].min_length, solenoids[this->SolenoidIndex].max_length));
                }
                if (MIDItype == 128 || velocity == 0){
                  analogWrite(solenoids[this->SolenoidIndex].pin, LOW);
                  solenoids[this->SolenoidIndex].active = LOW; 
                }
                
            } break;
            }
          } 
         
          else if(trigger-24 == solenoids[this->SolenoidIndex].trigger) // COMPLEMENTATY MIDI TRIGGER FOR DAMPER?
          { 
            if(solenoids[this->SolenoidIndex].type == DAMPER)
            {
                if(MIDItype == 144 && velocity > 0) solenoid_on(velocity);
                else if(MIDItype == 128 || velocity == 0) solenoid_off();
            }
          }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER // HELPER //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Solenoid::solenoid_on(int velocity)
{
  solenoids[this->SolenoidIndex].starttime = micros() + (solenoids[this->SolenoidIndex].delaytime*TIMESCALE);
  solenoids[this->SolenoidIndex].stoptime = solenoids[this->SolenoidIndex].starttime + (map(velocity, 1, 127, (solenoids[this->SolenoidIndex].min_length*TIMESCALE), (solenoids[this->SolenoidIndex].max_length*TIMESCALE)));  
  solenoids[this->SolenoidIndex].active = HIGH;
}

void Solenoid::solenoid_off(void)
{
  solenoids[this->SolenoidIndex].stoptime = micros();
}

void Solenoid::damper_on(int velocity)
{
  solenoids[this->SolenoidIndex].starttime = micros() + (solenoids[this->SolenoidIndex].delaytime*TIMESCALE);
  solenoids[this->SolenoidIndex].stoptime = solenoids[this->SolenoidIndex].starttime + solenoids[this->SolenoidIndex].damper_length*TIMESCALE;  
  solenoids[this->SolenoidIndex].active = HIGH;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS // PROCESS 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Solenoid::process()
{
  if(solenoids[this->SolenoidIndex].type != CONTINOUS && solenoids[this->SolenoidIndex].active == HIGH){ // only play before rollover
    
    if(micros() >= solenoids[this->SolenoidIndex].starttime && micros() <= solenoids[this->SolenoidIndex].stoptime) 
    {
      digitalWrite(solenoids[this->SolenoidIndex].pin, HIGH);
    } else{
      digitalWrite(solenoids[this->SolenoidIndex].pin, LOW); 
      solenoids[this->SolenoidIndex].active = LOW;  // avoid another stike on rollover
    }
  };
}
