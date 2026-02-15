#include <Arduino.h>
#include "Buttons.h"
#include "MakeCoffe.h"
#include "Display.h"

#define HOLD 2 // Buttons have 3 states: LOW, HIGH and HOLD

const int possibleValuePin = 11;

// -------------------------------
// Button configuration
// -------------------------------
enum { MINUS = 0, PLUS = 1, OK = 2 };
const int buttonPins[3] = {21, 20, 19};   // MINUS, PLUS, OK

// Button timers
const unsigned long debounceTime = 30;     // Debounce delay
const unsigned long holdTime = 500;        // Time to entering in hold state
const unsigned long repeatInterval = 50;   // Time between ticks when the button is held

// -------------------------------
// Button state tracking
// -------------------------------
int  buttonState[3] = {LOW, LOW, LOW};          // Debounced state
bool lastReading[3] = {LOW, LOW, LOW};          // Last raw reading
unsigned long lastChangeTime[3] = {0, 0, 0};    // Last change time

int value = 0;  // Caffeine value
bool display_and_return = false; // When the value is not present, the first click in PLUS or MINUS should display the value but not dec/increment

// Initial configuration
void setupButtons(){
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }
  pinMode(possibleValuePin, OUTPUT);
}

// Reset function (called on OK long press/hold)
void resetFunction() {
  value = 0;
  Serial.println("RESET");
}

// Called when Button <id> is pressed
void onButtonPress(int id){

  if(display_and_return){
    display_and_return = false;
    showNumber(value);
    return;
  }

  switch(id){
    case PLUS:
      if(value < 99) value++;
      //Serial.println(value);
      showNumber(value);
      break;
    
    case MINUS:
      if(value > 0) value--;
      //Serial.println(value);
      showNumber(value);
      break;
  }
}

// Called when Button <id> is released
void onButtonRelease(int id){

  switch(id){
    case OK:
      Serial.println("OK");
      newCoffe(String(value));
      break;
  }
}



// Called when Button <id> is long pressed
void onButtonHold(int id)
{
  switch(id){
    case PLUS:
      if(value < 99) value++;
      showNumber(value);
      //Serial.println(value);
      break;
    
    case MINUS:
      if(value > 0) value--;
      showNumber(value);
      //Serial.println(value);
      break;
    
    case OK: //Reset
      // The next button press will change from "--" to the previous value, but not inc or decrement
      display_and_return = true; 
      showLetters("--");
      newCapsule();
      break;
  }
}

// Button state logic, calling onButtonPress(), onButtonRelease() and onButtonHold() 
void loopButtons()
{
  digitalWrite(possibleValuePin, value < minimumCfi() || value > maximumCfi());

  static int id = 0;
  if(id++ >= 3) id = 0;

  bool reading = digitalRead(buttonPins[id]);

  // After hold mode, state returns low as soon as the button is released
  if(reading == LOW && buttonState[id] == HOLD){
    buttonState[id] = LOW;
  }

  if(reading != lastReading[id]){
    lastChangeTime[id] = millis(); // começou uma mudança
  }

  //Apenas considera depois de estabilizar no mesmo valor por 30 ms
  if(millis() - lastChangeTime[id] > debounceTime){ 
    if(reading + buttonState[id] == LOW + HIGH) { // reading and state are low and high or vice versa
      buttonState[id] = reading;
      if(buttonState[id] == HIGH)
          onButtonPress(id);
      else if(buttonState[id] == LOW)
          onButtonRelease(id);
    }
  }

  // Botão está premido e estável por mais de holdTime segundos => Inicia o estado de hold
  if(buttonState[id] == HIGH && millis() - lastChangeTime[id] > holdTime) {
    buttonState[id] = HOLD;
  }

  // Botão está em hold e passaram repeatInterval segundos desde o último tick => novo tick
  if(buttonState[id] == HOLD && millis() - lastChangeTime[id] > repeatInterval) {
    onButtonHold(id);
    lastChangeTime[id] = millis();
  }

  lastReading[id] = reading;
}






