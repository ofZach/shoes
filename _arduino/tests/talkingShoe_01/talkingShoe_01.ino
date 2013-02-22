#include <stdio.h>
#include <stdlib.h>

/*
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>

SoftwareSerial btSerial(10, 11); // RX, TX
char incomingVal;

int accelZ = A0;
int accelY = A1;
int accelX = A2;

int fsrBack = A4;
int fsrFront = A5;

void setup() {
  Serial.begin(9600);
  delay(1000);
  // set the data rate for the SoftwareSerial port
  btSerial.begin(115200);
  btSerial.println("bluetooth connected");
}

void loop() {
  if (btSerial.available()){
    //    incomingVal = btSerial.read(); 
    //    Serial.print("received: ");
    //    Serial.println(incomingVal);

    //    if (incomingVal == 'Y'){
    String msg;
    msg += readAccel('x');
    msg += ",";
    msg += readAccel('y');
    msg += ",";
    msg += readAccel('z');
    msg += ",";
    msg += readFsr('f');
    msg += ",";
    msg += readFsr('b');
    msg += ".";

    btSerial.print(msg);

    //      btSerial.print( readAccel('x') );
    //      btSerial.print(",");
    //      btSerial.print( readAccel('y') );
    //      btSerial.print(",");
    //      btSerial.print( readAccel('z') );
    //      btSerial.print(",");
    //      btSerial.print( readFsr('f') );
    //      btSerial.print(".");
    //    }

    //    if(incomingVal == 'N'){

    //    }
  }
}

int readFsr(byte which){
  int fsrVal;

  switch(which){
  case 'f':
    fsrVal = analogRead(fsrFront);
    delay(5); //debounce
    break;

  case 'b':
    fsrVal = analogRead(fsrBack);
    delay(5); //debounce
    break;
  }
  return fsrVal;  
}

int readAccel(byte axis){
  int accelVal;

  switch (axis) {
  case 'z':
    accelVal = analogRead(accelZ);
    delay(5); //debounce
    break;

  case 'y':
    accelVal = analogRead(accelY);
    delay(5);    
    break;

  case 'x':
    accelVal = analogRead(accelX);
    delay(5);
    break;
  }
  return accelVal;
}

void sendPacked(){

}




