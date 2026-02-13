#include "Communication.h"
#include "MakeCoffe.h"
#include "Display.h"
#include "Buttons.h"

void setup() {
  pinMode(BUTTON_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) { }

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





