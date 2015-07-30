/*************************************************** 
  4x4 weave control
  a pot controls "on" level for all 16 squares
  thread_map stores a list of pwm controller pin mappings for the square as read left to right and down the rows. i.e.
    thread_map[0] represents the top left square
    thread_map[4] represents the first square in the second row
    thread_map[15] represents the bottom right thread
  code cycles through each square, turning one on at a time at a rate that will allow for color change but not a power overload
  //as of 7/29 - each grid cell activated at about 410
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int pwm_pot_pin = A0;

//update these based on wiring
int thread_map[16] ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};


void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");

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
  
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      if(j == i) pwm.setPin(thread_map[j], pwm_val, false);
      else pwm.setPin(thread_map[j], 0, false);
    }
    Serial.println(i);
   delay(4000); //we may not need this but might as well give it a little time to digest. 
  }
  
}
