#include <Arduino.h>
#include <math.h>
#include "MakeCoffe.h"
#include "Display.h"

#define BUTTON_PRESS_TIME 250
#define BUTTON_INTERRUPT_MIN_TIME 250

long dead_time, over_time; //ms
long flow_time;
float used_cfi, used_vol;  //mg , mL
float total_cfi;           //mg

float desired_cfi, desired_vol;

enum State {
  state_waiting,
  state_start,
  state_dead_time,
  state_active_time,
  state_over_time
};
State current_state = state_waiting;


float vol2cfi(float vol){
  return total_cfi * (1 - exp(-vol/LAMBDA));
}

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


void newCoffe(const String& args) {

  if (args.length() == 0) {
    Serial.println("Usage: coffe <cafeine mg>");
    return;
  }

  desired_cfi = args.toFloat(); 
  desired_vol = cfi2vol(used_cfi + desired_cfi) - used_vol;
  flow_time = 1000.0 * desired_vol / FLOW_RATE; //ms

  Serial.printf("Caffeine quantity: %.1f mg\nNext vol: %.3f mL\nFlow time: %d ms\n\n", desired_cfi, desired_vol, flow_time);
  current_state = state_start; // Starts the process
}

void newCapsule(){
  //Or call setupCoffe() just for these 2 resets
  used_cfi = 0.0f;
  used_vol = 0.0f;

  Serial.println("New capsule");
}

float extractedCfi(long initial_time){
  float extracted_vol = FLOW_RATE * (millis() - initial_time) / 1000;
  float extracted_cfi = vol2cfi(used_vol + extracted_vol) - used_cfi;
  return extracted_cfi;
}
void loopCoffe() {

  if(current_state == state_waiting) return;
  static long time = 0, button = 0;
  static long active_time;
  static long extracted_time;


  // Presses the button if the counter is active
  digitalWrite(BUTTON_PIN, millis() - button < BUTTON_PRESS_TIME ? HIGH : LOW);
  
  if(current_state == state_start){
    if(flow_time < over_time)
      active_time = 0;
    else
      active_time = flow_time - over_time;

    if(dead_time + active_time < BUTTON_PRESS_TIME + BUTTON_INTERRUPT_MIN_TIME){
      Serial.println("Can't produce a coffe so small");
      current_state = state_waiting;
      return;
    }

    time = millis();
    Serial.print("Start! go to dead_time\n");
    button = millis(); // Starts the button counter, pressing it for 250 ms
    current_state = state_dead_time;
  }

  if(current_state == state_dead_time){

    // Divides dead_time in five equal partes to write "COFFEE"
    int frac = (5*(millis() - time)) / dead_time;
    switch(frac){
      case 0: showLetters("CO"); break;
      case 1: showLetters("OF"); break;
      case 2: showLetters("FF"); break;
      case 3: showLetters("FE"); break;
      case 4: showLetters("EE"); break;
    }

    if(millis() - time > dead_time){ // change to state_active_time
      time = millis();
      Serial.printf("Changing to state_active_time\n");
      current_state = state_active_time;
      extracted_time = millis();
    }
  }
    
  if(current_state == state_active_time){

    showNumber((int) extractedCfi(extracted_time));

    if(millis() - time > active_time){ // change to state_over_time
      time = millis();
      button = millis(); // Starts the button counter, pressing it for 250 ms
      Serial.print("Changing to state state_over_time\n");
      current_state = state_over_time;
    }
  }

  if(current_state == state_over_time){

    showNumber((int) extractedCfi(extracted_time));

    if(millis() - time > over_time){
      Serial.print("End over-time => Coffe is ready\n");

      // Update vars for the next coffe, as if the current coffe making process has already ended
      used_cfi += desired_cfi;
      used_vol += desired_vol;
      current_state = state_waiting;
    }
  }

}
