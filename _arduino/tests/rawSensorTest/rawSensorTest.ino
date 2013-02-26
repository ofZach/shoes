#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include "CommunicationUtils.h"
#include "FreeSixIMU.h"
#include <Wtv020sd16p.h>
#include <Wire.h>

//*** sensors ***//
FreeSixIMU shoeIMU = FreeSixIMU();
int printOutput = 0;
int fsrPin = A0;
int fsrVal = 0;
float angles[3];  //gyroscope
int raw[3];       //accelerometer


void setup(){
  
  Serial.begin(115200);
  //Serial.println("--- SETUP BEGIN ---");
  Wire.begin();
  delay(5);
  shoeIMU.init(); //IMU setup
  delay(5);
  setupBlueToothConnection();
}
void loop(){
  
if(Serial.available()>0){
  char serData = Serial.read();
  if(serData == 'a'){
    printOutput=1;
  }
  if(serData == 's'){
    printOutput=0;
//    delay(2000); // This delay is required.
//    Serial.print("\r\n+INQ=1\r\n");
//    delay(2000); // This delay is required.
  }
}
  if(printOutput==1){
    fsrVal = analogRead(fsrPin);
    delay(5); //analog debounce
    shoeIMU.getEuler(angles);
    delay(5); 
    shoeIMU.getRawValues(raw);
    delay(5);
    serialPrintCompact();
    //output=1;
  }
}
void serialPrintCompact(){
   // feel's awkward that this is text. 
//   char message[100];
//   sprintf(message, "$%i,%i,%i,%i,%i,%i,%i#", fsrVal, raw[0], raw[1], raw[2], (int)(angles[0]*1000.0), (int)(angles[1]*1000.0), (int)(angles[2]*1000.0));
//   String messageStr(message);
//   Serial.println(message);

  Serial.print("$");
  Serial.print(fsrVal);
  Serial.print(",");
  Serial.print(raw[0]);
  Serial.print(",");
  Serial.print(raw[1]);
  Serial.print(",");  
  Serial.print(raw[2]);
  Serial.print(",");
  Serial.print(angles[0]);
  Serial.print(",");  
  Serial.print(angles[1]);
  Serial.print(",");
  Serial.print(angles[2]); 
   Serial.println("#");

}
void setupBlueToothConnection()
{
    //Serial.print("Setting up Bluetooth link");       //For debugging, Comment this line if not required    
    //blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
//    delay(1000);
//    sendBlueToothCommand("\r\n+STWMOD=0\r\n");
//    sendBlueToothCommand("\r\n+STNA=TalkingShoe\r\n");
//    sendBlueToothCommand("\r\n+STAUTO=0\r\n");
//    sendBlueToothCommand("\r\n+STOAUT=1\r\n");
//    sendBlueToothCommand("\r\n+STPIN=0000\r\n");
    delay(2000); // This delay is required.
    Serial.print("\r\n+INQ=1\r\n");
    delay(2000); // This delay is required.
    //Serial.print("Setup complete");
 
}
 
void sendBlueToothCommand(char command[])
{
    //char a;
    Serial.print(command);
    //Serial.print(command);                          //For debugging, Comment this line if not required    
    delay(3000);
}
