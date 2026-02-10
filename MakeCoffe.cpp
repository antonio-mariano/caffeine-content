#include <Arduino.h>
#include <math.h>
#include "MakeCoffe.h"

#define BUTTON_PRESS_TIME 250

long dead_time, over_time; //ms
long flow_time;
float used_cfi, used_vol;  //mg , mL
float total_cfi;           //mg

enum State {
  state_waiting,
  state_start,
  state_dead_time,
  state_active_time,
  state_over_time
};
State current_state = state_waiting;


/*float vol2cfi(float vol){
  return total_cfi * (1 - exp(-vol/LAMBDA));
}*/

float cfi2vol(float cfi){
  return -LAMBDA * log(1 - cfi/total_cfi);
}

void setupCoffe()
{
  dead_time = 1000*DEAD_TIME;
  over_time = 1000*OVER_TIME;

  total_cfi = CAFFEINE_30 / (1 - exp(-30/LAMBDA)); //TODO pode ficar num setup
  used_cfi = 0.0f;
  used_vol = 0.0f;

}


void commandCafe(const String& args) {

  if (args.length() == 0) {
    Serial.println("Usage: Cafe <cafeine mg>");
    return;
  }

  float desired_cfi = args.toFloat(); 
  float desired_vol = cfi2vol(used_cfi + desired_cfi) - used_vol;
  flow_time = 1000.0 * desired_vol / FLOW_RATE; //ms

  Serial.printf("Caffeine quantity: %.1f mg\nNext vol: %.3f mL\nFlow time: %d ms\n\n", desired_cfi, desired_vol, flow_time);

  Serial.print("Starting the process...\n");
  current_state = state_start;

  // Update vars for the next coffe, as if the current coffe making process has already ended
  used_cfi += desired_cfi;
  used_vol += desired_vol;
}


void makeCoffe() {

  if(current_state == state_waiting) return;
  static unsigned long time = 0, button = 0;
  float active_time;

  digitalWrite(BUTTON_PIN, millis() - button < BUTTON_PRESS_TIME ? HIGH : LOW);
  
  switch (current_state)
  {
    case state_start:
      time = millis();
      Serial.print("Start! Press button #1 and go to dead_time\n");
      button = millis(); //Button is pressed

      current_state = state_dead_time;
      break;

    case state_dead_time:
      if(millis() - time > dead_time){
        time = millis();
        current_state = state_active_time;
        Serial.print("Changing to state state_active_time\n");

        active_time = flow_time - over_time;
      }
      break;
      
    case state_active_time:
      if(millis() - time > active_time){
        time = millis();
        Serial.print("Press button #2\n");
        button = millis(); //Button is pressed

        Serial.print("Changing to state state_over_time\n");

        current_state = state_over_time;
      }
      break;

    case state_over_time:  
      if(millis() - time > over_time){
        Serial.print("End over-time => Coffe is ready\n"); //PRESS BUTTON
        
        current_state = state_waiting;
      }
      break;
  }

}

/*
void commandSetLed(const String& args) {
  if (args.length() == 0) {
    Serial.println("Usage: setled <0|1>");
    return;
  }

  int state = args.toInt();
  digitalWrite(ledPin, state ? HIGH : LOW);

  Serial.print("LED set to: ");
  Serial.println(state ? "ON" : "OFF");
}
*/