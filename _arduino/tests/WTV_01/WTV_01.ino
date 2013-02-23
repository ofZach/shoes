

#include <Wtv020sd16p.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

int file = 0; //which file to play.

void setup() {
  //Initializes the module.
  wtv020sd16p.reset();
  delay(500);
  Serial.begin(115200);
}

void loop(){
  
  wtv020sd16p.playVoice(file);
  file++;
  if(file > 10) file = 0;
}

