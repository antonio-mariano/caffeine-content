#include <Arduino.h>
#include "Buttons.h"

#define HOLD 2

// -------------------------------
// Button configuration
// -------------------------------
const int buttonPins[3] = {20, 21, 19};   // PLUS, MINUS, OK
enum { PLUS = 0, MINUS = 1, OK = 2 };

const unsigned long debounceTime = 30;     // Debounce delay
const unsigned long holdTime = 500;        // Time before auto-repeat
const unsigned long repeatInterval = 75;  // Auto-repeat speed

// -------------------------------
// Button state tracking
// -------------------------------
int  buttonState[3] = {LOW, LOW, LOW};          // Debounced state
bool lastReading[3] = {LOW, LOW, LOW};          // Last raw reading
unsigned long lastChangeTime[3] = {0, 0, 0};      // Last change time


// -------------------------------
// Application variable
// -------------------------------
int value = 0;

// -------------------------------
// Reset function (called on OK long press)
// -------------------------------
void resetFunction() {
  value = 0;
  Serial.println("RESET triggered");
}

///////////

void setupButtons()
{
  // Configure all buttons as input with pull-down
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
  }
}

void onButtonPress(int id){

  switch(id){
    case PLUS:
      value++;
      Serial.println(value);
      break;
    
    case MINUS:
      value--;
      Serial.println(value);
      break;
  }
}

void onButtonRelease(int id){

  switch(id){
    case OK:
      Serial.println("OK");
      break;
  }
}

void onButtonHold(int id)
{
  switch(id){
    case PLUS:
      value++;
      Serial.println(value);
      break;
    
    case MINUS:
      value--;
      Serial.println(value);
      break;
    
    case OK:
      value = 0;
      Serial.println("Reset");
      buttonState[OK] = HIGH;
      break;
  }
}

void loopButtons()
{
  for(int id = 0; id < 3; id++){
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
      lastChangeTime[id] = millis(); // começou uma mudança
    }

    lastReading[id] = reading;
  }

}






