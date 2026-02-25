#include "Communication.h"
#include "MakeCoffe.h"
#include "Display.h"
#include "Buttons.h"

void setup() {
  pinMode(SOLENOID_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial && millis() < 2000) { }

  setupCoffe();
  setupDisplay();
  setupButtons();
  
}

void loop() {

  loopSerialInput();
  loopCoffe();
  loopDisplay();
  loopButtons();
    
}





