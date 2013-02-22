
#include <SoftwareSerial.h>
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include "CommunicationUtils.h"
#include "FreeSixIMU.h"
#include <Wire.h>

//*** Emic2 TTS module ***// 
#define rxPin 8    // Serial input (connects to Emic 2 SOUT)
#define txPin 9    // Serial output (connects to Emic 2 SIN)
#define ledPin 13
SoftwareSerial emic = SoftwareSerial(rxPin, txPin);

String sentence[] = {
  "you are not moving.",
  "you are walking slowly.",
  "you are walking quickly.",
  "you are running.",
  "jump!",
  //"[:rate 150][:n0][:dv ap 90 pr 0]All your base are belong to us.",
  "eyed pat you on the back if I could.", 
  //"[:phone arpa speak on][:rate 200][:n0][hxae<300,10>piy<300,10>brrrx<600,12>th<100>dey<600,10>tuw<600,15> yu<1200,14>_<300> hxae<300,10>piy<300,10>brrrx<600,12>th<100>dey<600,10> tuw<600,17>yu<1200,15>_<300>",  //"Getting off the ground? Good job.", 
  "Hey internets, whose got two thumbs, speaks limited French, and is on a run? This mwah.",
  "Its hard being a shoe when youre carrying around all this dead wait.",
  //"If my bank account was filled with runs, eyed be Warren Buffet.",
  "That sun would do your pale skin some good.",
  "Yayyy! We did it.",
  "You call that running? Nancy.",
  //[:rate 200][:n0][:dv ap 90 pr 0] Nancy.",
  "Boom! Goes the dynamite."
  //"dun, dun, dun dun, dun, dun, duuuuuuuuun" //(humming the tune to eye of the tiger) 
};

unsigned long bFootRaised;
unsigned long bFootRasiedLastFrame;
unsigned long lastFrameFoot;
unsigned long goodFootLiftThresholdMillis;
unsigned long startFootLiftMillis;
unsigned long endFootLiftMillis;
unsigned long lastGoodFootLiftTime;

int walkingMode;

float stepsPerMinute;
float stepsPerMinuteSmooth;

boolean talkTime;
unsigned long lastTalkFrame;

//*** sensors ***//
FreeSixIMU shoeIMU = FreeSixIMU();
int fsrPin = A0;
int fsrVal = 0;
float angles[3];  //gyroscope
int raw[3];       //accelerometer
int msg = -1;

//long stepTimeStamp = 0;
//long stepTimeInterval = 0;
//boolean stepOn = false; 

void setup() {

  Serial.begin(115200);
  //Serial.println("--- SETUP BEGIN ---");
  Wire.begin();
  delay(5);
  shoeIMU.init(); //IMU setup
  delay(5);

  /*** emic setup ***/
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // turn LED off
  emic.begin(9600);
  delay(500);
  /*** emic notes:
   * When the Emic 2 powers on, it takes about 3 seconds for it to successfully
   * intialize. It then sends a ":" character to indicate it's ready to accept
   * commands. If the Emic 2 is already initialized, a CR will also cause it
   * to send a ":"
   ***/
  emic.print('\n');             // Send a CR in case the system is already up
  while (emic.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emic.flush();                 // Flush the receive buffer

  emic.print("W160\n");          //words per minute ( 75 - 600 ) 
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  delay(500);

  emic.print("V9\n");          //Volume control, -48 to 18db
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  delay(500);

  // Change voice
  emic.print("N6\n");          
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  // Change Parser: P0 = DECtalk  // P1 = Epson
  emic.print("P0\n");          
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  //Serial.println("--- SETUP END ---");
  talkTime = false;
  lastTalkFrame = millis();

  lastFrameFoot = 100;

  bFootRaised = 0;
  bFootRasiedLastFrame = 0;
  goodFootLiftThresholdMillis = 50;
  lastGoodFootLiftTime = millis();
  stepsPerMinute = 0;
  stepsPerMinuteSmooth = 0;
  walkingMode = 0;
}

void loop(){
  fsrVal = analogRead(fsrPin);
  delay(5); 
  shoeIMU.getEuler(angles);
  delay(5);
  shoeIMU.getRawValues(raw);
  delay(5);

  if(Serial.available()>0) {
    char inc = Serial.read();
    if(inc == '0') sendMsg(0);
    if(inc == '1') sendMsg(1);
    if(inc == '2') sendMsg(2);
    if(inc == '3') sendMsg(3);
    if(inc == '4') sendMsg(4);
  }

  if(fsrVal < 50) bFootRaised = 1;
  else bFootRaised = 0;

  //***step detect and timer
  if (bFootRaised && !bFootRasiedLastFrame){
    startFootLiftMillis = millis();
  }

  if (!bFootRaised && bFootRasiedLastFrame){
    endFootLiftMillis = millis();
    if ((endFootLiftMillis - startFootLiftMillis) > goodFootLiftThresholdMillis){
      unsigned long  oldLastGoodFootTime = lastGoodFootLiftTime;
      lastGoodFootLiftTime = (endFootLiftMillis + startFootLiftMillis) / 2;

      float timeBetweenSteps = lastGoodFootLiftTime - oldLastGoodFootTime;
      float stepsPerMinuteRate = 60.0 / (float)(timeBetweenSteps/1000.0);


      stepsPerMinute = stepsPerMinuteRate;        // smooth this somehow?
      //Serial.print("stepsPerMinute: ");
      //Serial.println(stepsPerMinute);    
    }
  }

  if (bFootRaised == 1){
    bFootRasiedLastFrame = 1;
  } 
  else {
    bFootRasiedLastFrame = 0;
  }

  if ( ((long int)millis() - lastGoodFootLiftTime) >  2500){
    stepsPerMinuteSmooth = 0;
    stepsPerMinute = 0;
  }

  int stopTimeDetectMillis = 2000;
  if ( ((long int)millis() - lastGoodFootLiftTime) >  stopTimeDetectMillis){
    stepsPerMinuteSmooth = 0.9 * stepsPerMinuteSmooth + 0.1 * 0.0;
  } 
  else {
    stepsPerMinuteSmooth = 0.99 * stepsPerMinuteSmooth + 0.01 * stepsPerMinute;
  }

  int oldMode = walkingMode;

  if(stepsPerMinuteSmooth < 15) { //not moving
    walkingMode = 0;
  }
  else if(stepsPerMinuteSmooth >= 15 && stepsPerMinuteSmooth <50){ //walking slowly
    walkingMode = 1;
  }
  else if(stepsPerMinuteSmooth >= 50 && stepsPerMinuteSmooth <100){ //walking quickly
    walkingMode = 2;
  }
  else if(stepsPerMinuteSmooth >= 130){ // running
    walkingMode = 3;
  }

  if (oldMode != walkingMode){
    sendMsg(walkingMode); 
  }

  //*** talk timer 
  //  if(millis() - lastTalkFrame > 5000){ //currently will talk every 5 seconds
  //    if(stepsPerMinute < 15) { //not moving
  //      sendMsg(0);
  //    }
  //    else if(stepsPerMinute > 15 && stepsPerMinute <50){ //walking slowly
  //      sendMsg(1);
  //    }
  //    else if(stepsPerMinute > 50 && stepsPerMinute <90){ //walking quickly
  //      sendMsg(2);
  //    }
  //    else if(stepsPerMinute > 90){ // running
  //      sendMsg(3);
  //    }
  //    lastTalkFrame = millis();
  //  }

  //if(fsrVal > 525) sendMsg(4); //jumping???

  //bFootRasiedLastFrame = bFootRaised;

  //next is to average stepTimeInterval and set thresholds for walking, jogging, running, etc

  //printing everything out

  if (fsrVal > 370){
    sendMsg(4);
    //Serial.print("fsr:\t");
    //Serial.println(fsrVal);
  }

  //Serial.print("fsr:\t");
  //Serial.println(fsrVal);
  // 
  //    Serial.print("\taccel:\t");
  //    Serial.print(raw[0]);
  //    Serial.print("\t");  
  //    Serial.print(raw[1]);
  //    Serial.print("\t");  
  //    Serial.print(raw[2]);
  //  
  //    Serial.print("\tgyro:\t");
  //    Serial.print(angles[0]);
  //    Serial.print("\t");  
  //    Serial.print(angles[1]);
  //    Serial.print("\t");
  //    Serial.println(angles[2]); 
}

void sendMsg(int msgNum){
  //Serial.print(">>> SENDING MSG: ");
  Serial.print(sentence[msgNum]);s
  emic.print('S');
  emic.print(sentence[msgNum]);
  emic.print('\n');
  //while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  //  stepOn = false;
}









