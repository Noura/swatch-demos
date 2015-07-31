/*************************************************** 
 * SEVEN SEGMENT DISPLAY
 * a pot controls "on" level for all segments
 * d1 stores all the pin mappings for the first digit, stored top to bottom, left to right. 
 * d2 stores the second digital in the same fashion
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* the seven segment display
         _______
        /   0   \
    / \ \_______/ / \
   |   |         |   |
   |   |         |   |
   | 1 |         | 2 |
   |   |         |   |
   |   | _______ |   |
    \ / /   3   \ \ /
    / \ \_______/ / \
   |   |         |   |
   |   |         |   |
   | 4 |         | 5 |
   |   |         |   |
   |   | _______ |   |
    \ / /   6   \ \ /
        \_______/

*/

int ZERO[7] = {1, 1, 1, 0, 1, 1, 1};
int ONE[7] = {0, 1, 0, 0, 1, 0, 0}; // i think 1 should be on the left not the right side of the seven segment
int TWO[7] = {1, 0, 1, 1, 1, 0, 1};
int THREE[7] = {1, 0, 1, 1, 0, 1, 1};
int FOUR[7] = {0, 1, 1, 1, 0, 1, 0};
int FIVE[7] = {1, 1, 0, 1, 0, 1, 1};
int SIX[7] ={1, 1, 0, 1, 1, 1, 1};
int SEVEN[7] ={1, 0, 1, 0, 0, 1, 0};
int EIGHT[7] ={1, 1, 1, 1, 1, 1, 1};
int NINE[7] ={1, 1, 1, 1, 0, 1, 0};


int pwm_pot_pin = A0;

//update these based on wiring
int d1[7] ={
  7, 0, 11, 3, 12, 1, 2};

  
int d2[7] = {
  10, 8, 9, 5, 6, 13, 4};
  
int counter = 0;
int frame_counter = 0;
int counter_interval = 600;

void setup() {
  Serial.begin(9600);

  pinMode(pwm_pot_pin, INPUT);

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!



}

void loop() {

  //the pot pin val sets the level for "on" on all 16 inputs
  int pwm_pot_val = analogRead(pwm_pot_pin);
  int pwm_val = map(pwm_pot_val, 0, 1023, 0, 4096);
  
  //cycleThroughPins(pwm_val);


  if(frame_counter++ % counter_interval == 0) counter++;
  if(counter > 99) counter = 0;

  int tens = counter / 10;
  int ones = counter % 10;

    writeToDigit(d1, tens, pwm_val);
    writeToDigit(d2, ones, pwm_val);  
}

void cycleThroughPins(int pwm_val){
  for(int i = 0; i < 14; i++){
      for(int j = 0; j < 14; j++){
             if(j == i) pwm.setPin(j, pwm_val, false);
              else pwm.setPin(j, 0, false);
      }
      Serial.println(i);
     delay(14000); //we may not need this but might as well give it a little time to digest. 
   }
}

void writeToDigit(int* d, int val, int pwm_val){
  Serial.println(val);
  if(val == 0) d = ZERO;
  else if(val == 1) d = ONE;
  else if(val == 2) d = TWO;
  else if(val == 3) d = THREE;
  else if(val == 4) d = FOUR;
  else if(val == 5) d = FIVE;
  else if(val == 6) d = SIX; 
  else if(val == 7) d = SEVEN;
  else if(val == 8) d = EIGHT;
  else  d = NINE;
  
  for(int i = 0; i < 14; i++){
    pwm.setPin(i, d[i]*pwm_val, false);
  }
}


