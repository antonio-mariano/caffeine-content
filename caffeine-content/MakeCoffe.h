
#pragma once

#define BUTTON_PIN LED_BUILTIN

//Capsule parameters
#define CAFFEINE_30 63     // mg of caffeine in 30 mL of coffe

// TODO O usar deve meter a cfi a 15 e o lambda é calcualdo internamente no setupCoffe
#define LAMBDA 10.82    // "Time constant" for volume instead of time

// Machine parameters
#define DEAD_TIME 3   // time after button press to start running coffe
#define OVER_TIME 1  // seconds after button press that stills run coffe
#define FLOW_RATE 2.0        // mL (or grams) per second

void setupCoffe();

void newCoffe(const String& args);

void newCapsule();

void runCoffe();

//void commandSetLed(const String& args);