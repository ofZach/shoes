/*
  
 Emic 2 Text-to-Speech Module: Basic Demonstration       
 
 Author: Joe Grand [www.grandideastudio.com]             
 Contact: support@parallax.com                            
 
 Program Description:
 
 This program provides a simple demonstration of the Emic 2 Text-to-Speech
 Module. Please refer to the product manual for full details of system 
 functionality and capabilities.
 
 Revisions:
 
 1.0 (February 13, 2012): Initial release
 
 datasheet: http://www.parallax.com/Portals/0/Downloads/docs/prod/sens/30016-Emic2TextToSpeech-v1.1.pdf
 
 */

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>

#define rxPin 9    // Serial input (connects to Emic 2 SOUT)
#define txPin 8    // Serial output (connects to Emic 2 SIN)
#define ledPin 13  // Most Arduino boards have an on-board LED on this pin

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);


void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);

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
}

void loop()  // Main code, to run repeatedly
{

  emicSerial.print("W120\n");          //words per minute ( 75 - 600 ) 
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  // Speak some text
  emicSerial.print("V0\n");          //Volume control, -48 to 18db
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);


  // Change voice
  emicSerial.print("N1\n");          //Volume control, -48 to 18db
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  emicSerial.print('S');
  emicSerial.print("I’d pat you on the back if I could.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);    

  // Sing a song
  //emicSerial.print("D1\n");
  emicSerial.print('S');
  emicSerial.print("Getting off the ground. Good job.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("Hey internets, who’s got two thumbs, speaks limited French and is on a run. This moi.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("It’s hard being a shoe when you’re carrying around all this dead weight.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("If my bank account was filled with runs, I’d be Warren Buffet.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("That sun would do your pale skin some good.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);
  
    emicSerial.print('S');
  emicSerial.print("Yay! We did it.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("You call that running, Nancy.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);

  emicSerial.print('S');
  emicSerial.print("Boom! Goes the dynamite.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  delay(500);
  
  emicSerial.print("phone arpa speak on");
  emicSerial.print("rate 200");
  emicSerial.print("n0");
  emicSerial.print("hxae<300,10>piy<300,10> brrrx<600,12>th<100>dey<600,10>tuw<600,15> yu<1200,14>_<300> hxae<300,10>piy<300,10> brrrx<600,12>th<100>dey<600,10> tuw<600,17>yu<1200,15>_<300> hxae<300,10>piy<300,10> brrrx<600,22>th<100>dey<600,19>");
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);

  //  emicSerial.print("W75\n");          //words per minute ( 75 - 600 ) 

}





