

void emicSetup(){
  
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

  //words per minute ( 75 - 600 ) 
  emic.print("W160\n");         
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emic.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  delay(500);

  //Volume control, -48 to 18db
  emic.print("V9\n");          
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
}
