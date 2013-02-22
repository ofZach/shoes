/*
  
 Emic 2 Text-to-Speech Module
 
 datasheet: http://www.parallax.com/Portals/0/Downloads/docs/prod/sens/30016-Emic2TextToSpeech-v1.1.pdf
 
 */

//1 - perfect paul, 140 WPM
//b betty, 140
//dr.denn, 140
//4 - perfect paul, 160 WPM
//frail frank, 160
//dr.denn, 160
//7 - perfect paul, 180 WPM
//frail frank, 180
//dr.denn, 180
//all robot, 160

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>

#define rxPin 9    // Serial input (connects to Emic 2 SOUT)
#define txPin 8    // Serial output (connects to Emic 2 SIN)
#define ledPin 13  

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);

//super robot: [:rate 200][:n0][:dv ap 90 pr 0]
String sentence[] = { 
  "[:rate 200][:n0][:dv ap 90 pr 0]eyed pat you on the back if I could.", 
  //"[:phone arpa speak on][:rate 200][:n0][hxae<300,10>piy<300,10>brrrx<600,12>th<100>dey<600,10>tuw<600,15> yu<1200,14>_<300> hxae<300,10>piy<300,10>brrrx<600,12>th<100>dey<600,10> tuw<600,17>yu<1200,15>_<300>",  //"Getting off the ground? Good job.", 
  "[:rate 200][:n0][:dv ap 90 pr 0]Hey internets, whose got two thumbs, speaks limited French, and is on a run. This mwah.",
  "[:rate 200][:n0][:dv ap 90 pr 0]Its hard being a shoe when youre carrying around all this dead wait.",
  //"If my bank account was filled with runs, eyed be Warren Buffet.",
  "[:rate 200][:n0][:dv ap 90 pr 0]That sun would do your pale skin some good.",
  "[:rate 200][:n0][:dv ap 90 pr 0]Yayyy! We did it.",
  "[:rate 200][:n0][:dv ap 90 pr 0]You call that running?  Nancy.",
  "[:rate 200][:n0][:dv ap 90 pr 0]Boom! Goes the dynamite.",
  //"dun, dun, dun dun, dun, dun, duuuuuuuuun" //(humming the tune to eye of the tiger) 
};

void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);
  Serial.begin(9600);
  digitalWrite(ledPin, LOW);  // turn LED off

  /*
    When the Emic 2 powers on, it takes about 3 seconds for it to successfully
   intialize. It then sends a ":" character to indicate it's ready to accept
   commands. If the Emic 2 is already initialized, a CR will also cause it
   to send a ":"
   */
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emicSerial.flush();                 // Flush the receive buffer

  emicSerial.print("W160\n");          //words per minute ( 75 - 600 ) 
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  delay(500);

  emicSerial.print("V9\n");          //Volume control, -48 to 18db
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  delay(500);

  // Change voice
  emicSerial.print("N4\n");          
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  
  // Change Parser: P0 = DECtalk  // P1 = Epson
  emicSerial.print("P0\n");          
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void loop()  // Main code, to run repeatedly
{  
  for(int i=0; i<7; i++){
    emicSerial.print('S');
    Serial.print(i);
    emicSerial.print(sentence[i]);  // Send the desired string to convert to speech
    Serial.println(sentence[i]);
    emicSerial.print('\n');
    digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
    while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
    digitalWrite(ledPin, LOW);
    delay(500);
  }

  //  emicSerial.flush();                 // Flush the receive buffer
  delay(1000);

  // Change voice
  //  emicSerial.print("N1\n");          //Volume control, -48 to 18db
  //  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  //  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  //  digitalWrite(ledPin, LOW);

  //  emicSerial.print("phone arpa speak on");
  //  emicSerial.print("rate 200");
  //  emicSerial.print("n0");
  //  emicSerial.print("hxae<300,10>piy<300,10> brrrx<600,12>th<100>dey<600,10>tuw<600,15> yu<1200,14>_<300> hxae<300,10>piy<300,10> brrrx<600,12>th<100>dey<600,10> tuw<600,17>yu<1200,15>_<300> hxae<300,10>piy<300,10> brrrx<600,22>th<100>dey<600,19>");
  //  emicSerial.print('\n');
  //  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  //  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  //  digitalWrite(ledPin, LOW);

  //  emicSerial.print("W75\n");          //words per minute ( 75 - 600 ) 

}








