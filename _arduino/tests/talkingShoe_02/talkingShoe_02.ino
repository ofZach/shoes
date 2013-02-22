

int buttonPin = 12;
int ledPin = 13;
int ctr = 0;

String msg;

void setup(){

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  delay(100);

}

void loop(){

  if(digitalRead(buttonPin) == LOW){
    msg = "hello, i'm a shoe- counter: ";
    msg += ctr;
    msg += ".";
    ctr++;
    Serial.print(msg);
    digitalWrite(ledPin, HIGH);
    delay(1000);
  }
  digitalWrite(ledPin, LOW);
}


