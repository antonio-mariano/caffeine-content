
#pragma once



//Capsule parameters
#define CAFFEINE_30 63     // mg of caffeine in 30 mL of coffe

// TODO O user deve meter a cfi a 15 e o lambda é calcualdo internamente no setupCoffe
#define LAMBDA 13.6    // "Time constant" for volume instead of time

// Machine parameters
#define DEAD_TIME 3.0  // time after button press to start running coffe
#define OVER_TIME 2.6  // seconds after button press that stills run coffe
#define FLOW_RATE 1.75  // mL (or grams) per second

#define SOLENOID_PIN 22 // Actuator for the coffe machine

void setupCoffe();

// Turning off the machine as soon as it enters over_time will produce a volume equal to over_time
// So, the minimum volume is the volume produced during over_time
// This function convers that min_vol to caffeine
int minimumCfi();

int maximumCfi();

void newCoffe(const String& args);

void newCapsule();

void loopCoffe();

//void commandSetLed(const String& args);