#include <Arduino.h>
#include "Communication.h"
#include "MakeCoffe.h"

String inputBuffer = "";

bool handleSerialInput() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        return true;
      }
    } else {
      inputBuffer += c;
    }
  }
  return false;
}

void processInput() {
  
  String trimmed = inputBuffer;
  trimmed.trim();
  inputBuffer = "";

  int firstSpace = trimmed.indexOf(' ');
  String command, args;

  if (firstSpace == -1) {
    command = trimmed;
    args = "";
  } else {
    command = trimmed.substring(0, firstSpace);
    args = trimmed.substring(firstSpace + 1);
  }

  if (command.equalsIgnoreCase("cafe")) {
    commandCafe(args);
  }

}

