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
const unsigned long repeatInterval = 100;  // Auto-repeat speed

// -------------------------------
// Button state tracking
// -------------------------------
bool buttonState[3] = {LOW, LOW, LOW};          // Debounced state
bool lastReading[3] = {LOW, LOW, LOW};          // Last raw reading
unsigned long lastDebounce[3] = {0, 0, 0};      // Last change time
unsigned long pressStartTime[3] = {0, 0, 0};    // When button was pressed
unsigned long lastRepeatTime[3] = {0, 0, 0};    // Last auto-repeat event

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


// -------------------------------
// Read and process one button
// -------------------------------
void handleButton(int id) {
  bool reading = digitalRead(buttonPins[id]);

  // Debounce logic
  if (reading != lastReading[id]) {
    lastDebounce[id] = millis();
    lastReading[id] = reading;
  }

  if (millis() - lastDebounce[id] > debounceTime) {
    if (reading != buttonState[id]) {
      buttonState[id] = reading;

      // Button just pressed
      if (buttonState[id] == HIGH) {
        pressStartTime[id] = millis();
        lastRepeatTime[id] = millis();

        if (id == PLUS) {
          value++;
          Serial.print("Value = ");
          Serial.println(value);
        }
        else if (id == MINUS) {
          value--;
          Serial.print("Value = ");
          Serial.println(value);
        }
        // OK does NOT increment; only long press triggers reset
      }
    }
  }

  // Auto-repeat for PLUS and MINUS
  if (buttonState[id] == HIGH && (id == PLUS || id == MINUS)) {
    if (millis() - pressStartTime[id] > holdTime) {
      if (millis() - lastRepeatTime[id] > repeatInterval) {
        lastRepeatTime[id] = millis();

        if (id == PLUS) {
          value++;
        } else {
          value--;
        }

        Serial.print("Value = ");
        Serial.println(value);
      }
    }
  }

  // Long press for OK button → reset
  if (id == OK && buttonState[id] == HIGH) {
    if (millis() - pressStartTime[id] > holdTime) {
      resetFunction();
      // Prevent multiple resets while holding
      pressStartTime[id] = millis() + 100000; 
    }
  }
}


void runButtons(){
    handleButton(PLUS);
  handleButton(MINUS);
  handleButton(OK);

}









