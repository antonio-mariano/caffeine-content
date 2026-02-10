#include "Communication.h"
#include "MakeCoffe.h"


void setup() {
  pinMode(BUTTON_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) { }

  setupCoffe();
}

void loop() {

  if(handleSerialInput())  processInput();

  makeCoffe();
    
}





