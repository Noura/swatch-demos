#define warpPin 9
#define weftPin 10
#define warpPot A0
#define weftPot A1

void setup() {
  pinMode(13, OUTPUT);
  pinMode(warpPin, OUTPUT);
  pinMode(weftPin, OUTPUT);
  pinMode(warpPot, INPUT);
  pinMode(weftPot, INPUT);
  
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println();
  
  int warpPotVal = analogRead(warpPot);
  int weftPotVal = analogRead(weftPot);

  int warpVal = map(warpPotVal, 0, 1023, 0, 255);
  int weftVal = map(weftPotVal, 0, 1023, 0, 255);
  
  Serial.print("warp: "); Serial.print(warpPotVal); Serial.print(" pot, "); Serial.print(warpVal); Serial.print(" PWM, ");
  Serial.print("weft: "); Serial.print(weftPotVal); Serial.print(" pot, "); Serial.print(weftVal); Serial.print(" PWM, ");

  analogWrite(warpPin, warpVal);
  analogWrite(weftPin, weftVal);
  
  delay(30);
 
}
