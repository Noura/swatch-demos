#define transistorPin 10
#define potPin A0

void setup() {
  pinMode(transistorPin, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println();
  
  int potVal = analogRead(potPin);

  int transistorVal = map(potVal, 0, 1023, 0, 255);
  
  Serial.print("pot: "); Serial.print(potVal); Serial.print(", write to transistor: "); Serial.print(transistorVal);

  analogWrite(transistorPin, transistorVal);
  
  delay(30);
 
}
