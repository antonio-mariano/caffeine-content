#include <Arduino.h>
#include "Display.h"

// Segment pin order: A, B, C, D, E, F, G
const int segmentPins[7] = {14, 15, 18, 17, 16, 13, 12};

const int anode10 = 8, anode1 = 9;

// Digit-to-segment map for numbers 0–9
// 1 = segment ON, 0 = segment OFF
// For common-anode displays: ON = LOW, OFF = HIGH
const bool digitMap[][7] = {
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
  {1,1,1,1,0,1,1}, // 9

  {1,0,0,1,1,1,0}, // C  //10
  {1,1,1,1,1,1,0}, // O  //11
  {1,0,0,0,1,1,1}, // F  //12
  {1,0,0,1,1,1,1}, // E  //13
  {0,0,0,0,0,0,1}  // -  //14
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

  if(digit < 0){
    for (int i = 0; i < 7; i++)  digitalWrite(segmentPins[i], HIGH);  //CLEAR
    return;
  }

  // For common-anode: LOW = ON, HIGH = OFF
  for (int i = 0; i < 7; i++)  digitalWrite(segmentPins[i], digitMap[digit][i] ? LOW : HIGH);    
}

int digit_left = 0, digit_right = 0;

// Changes digit_left and digit_right so loopDisplay() shows the <number>
void showNumber(int number){

   // If invalid number: -1 means nothing to display
  if(number < 0 || number > 99){
    digit_left = -1;
    digit_right = -1;
    return;
  }

  // Extract algarisms from number
  digit_left = number/10;
  digit_right = number - digit_left*10;
}

// Changes digit_left and digit_right so loopDisplay() shows the first 2 elements in <letters>
void showLetters(const String& letters) {
  int index;

  for(int n = 0; n < 2; n++){
    switch(letters[n]) {
      case 'C': index = 10; break;
      case 'O': index = 11; break;
      case 'F': index = 12; break;
      case 'E': index = 13; break;
      case '-': index = 14; break;
      default:  index = -1; break; //-1 means nothing to display
    }
    n == 0 ? digit_left = index : digit_right = index;
  }
}


void loopDisplay(){

  static unsigned long time = 0;
  static bool units_active = true;

  if(millis() - time >= 5){
    time = millis();
    units_active = !units_active;
  }

  if(units_active){
    showDigit(digit_right);
    digitalWrite(anode10, HIGH);
    digitalWrite(anode1, LOW);
  }
  else{
    showDigit(digit_left);
    
    digitalWrite(anode10, LOW);
    digitalWrite(anode1, HIGH);
  }
}


