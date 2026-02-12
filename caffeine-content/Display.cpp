#include <Arduino.h>
#include "Display.h"

// Segment pin order: A, B, C, D, E, F, G
const int segmentPins[7] = {14, 15, 18, 17, 16, 13, 12};

const int anode10 = 10, anode1 = 11;

// Digit-to-segment map for numbers 0–9
// 1 = segment ON, 0 = segment OFF
// For common-anode displays: ON = LOW, OFF = HIGH
const bool digitMap[10][7] = {
  //A B C D E F G
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setupDisplay(){
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], HIGH); // OFF for common-anode
  }

  pinMode(anode10, OUTPUT);
  digitalWrite(anode10, LOW);

  pinMode(anode1, OUTPUT);
  digitalWrite(anode1, LOW);
}


// Displays a single digit (0–9) on the 7-segment display
void showDigit(int digit) {

  if (digit < 0 || digit > 9){
    for (int i = 0; i < 7; i++)  digitalWrite(segmentPins[i], HIGH);  
    return;
  }

   // For common-anode: LOW = ON, HIGH = OFF
  for (int i = 0; i < 7; i++)  digitalWrite(segmentPins[i], digitMap[digit][i] ? LOW : HIGH);    
}

int disp_tens = 0, disp_units = 0;

void showNumber(int number){

  if (number < 0 || number > 99){
    disp_tens = -1;
    disp_units = -1;
    return;
  }

  disp_tens = number/10;
  disp_units = number - disp_tens*10;
}

void runDisplay(){

  static unsigned long time = 0;
  static bool units_active = true;

  if(millis() - time >= 5){
    time = millis();
    units_active = !units_active;
  }

  if(units_active){
    showDigit(disp_units);
    digitalWrite(anode10, HIGH);
    digitalWrite(anode1, LOW);
  }
  else{
    showDigit(disp_tens);
    
    digitalWrite(anode10, LOW);
    digitalWrite(anode1, HIGH);
  }

}

/************** BUTTONS ****************/
/*
bool buttonPlusState = LOW;

const int buttonOkPin = 19;
const int buttonPlusPin = 20;
const int buttonMinusPin = 21;
*/
/**********/

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
//unsigned long pressStartTime[3] = {0, 0, 0};    // When button was pressed
//unsigned long lastRepeatTime[3] = {0, 0, 0};    // Last auto-repeat event

// -------------------------------
// Application variable
// -------------------------------
int value = 0;
#define HOLD 2
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


void runButtons(){

  int id = 0;

  bool reading = digitalRead(buttonPins[id]);

  // After hold mode, state returns low as soon as the button is released
  if(reading == LOW && buttonState[id] == HOLD){
    buttonState[id] = LOW;
  }

  if(reading != lastReading[id]){
    lastChangeTime[id] = millis(); // começou uma mudança
  }

  //Apenas considera depois de estabilizar no mesmo valor por 30 ms
  if (millis() - lastChangeTime[id] > debounceTime) { 
    if (reading + buttonState[id] == 1) { // reading and state are low and high or vice versa
      buttonState[id] = reading;
      if (buttonState[id] == HIGH) {
          value++;
          Serial.println(value);
      }
    }
  }

  if (millis() - lastChangeTime[id] > holdTime && buttonState[id] == HIGH) {
    buttonState[id] = HOLD;
  }

  if (millis() - lastChangeTime[id] > repeatInterval && buttonState[id] == HOLD) {
    value++;
    Serial.println(value);
    lastChangeTime[id] = millis(); // começou uma mudança
  }

  lastReading[0] = reading;

}









