#ifndef Solenoid_h
#define Solenoid_h

#define SOLENOID_LIBRARY_VERSION "00.11"

#define MAX_SOLENOIDS 16

#define BEATER 0
#define DAMPER 1
#define CONTINOUS 3

#define STANDART 0
#define FLOATING 1
#define DAMPED 2

typedef struct {
    int type;
    int mode;
    int trigger;
    int pin;
    double min_length;
    double max_length;
    double damper_length;
    double delaytime;
    double starttime;
    double stoptime;
    int active;
    int damp_length;
} solenoid_t;



class Solenoid {
    public: 
	  Solenoid(int, int, int, double, double, double); // TYPE, TRIGGER, PIN, MIN_LENGTH, MAX_LENGTH, DELAY
	  Solenoid(int, int, int, double, double, double, double); // TYPE, TRIGGER, PIN, MIN_LENGTH, MAX_LENGTH, DAMPER_LENGTH, DELAY
          Solenoid(int, int, int, double, double); // TYPE, TRIGGER, PIN, MAX_LENGTH, DELAY
	  void control(int, int, int);
	  void process(void);
    private:
	  int SolenoidIndex;
          void solenoid_on(int);
          void solenoid_off(void);
          void damper_on(int);
};

#endif
