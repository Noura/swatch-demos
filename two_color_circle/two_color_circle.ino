#define redPin 9
#define bluePin 10
#define redPot A0
#define bluePot A1

void setup() {
  pinMode(13, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPot, INPUT);
  pinMode(bluePot, INPUT);
  
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println();
  
  int redPotVal = analogRead(redPot);
  int bluePotVal = analogRead(bluePot);

  int  redVal = map(redPotVal, 0, 1023, 0, 255);
  int blueVal = map(bluePotVal, 0, 1023, 0, 255);
  
  Serial.print("warp: "); Serial.print(redVal); 
  Serial.print("weft: "); Serial.print(blueVal);

  analogWrite(redPin, redVal);
  analogWrite(bluePin, blueVal);
  
  delay(30);
 
}
