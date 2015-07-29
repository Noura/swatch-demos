/*************************************************** 
 STRIPE CONTROL
  a pot controls "on" level for all 10 stripes
  thread_map stores a list of pwm controller pin mappings for the stripes as read top to bottom
  code cycles through each stripe, turning one on at a time at a rate that will allow for color change but not a power overload
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int pwm_pot_pin = A0;

//update these based on wiring
int thread_map[10] ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};


void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  pinMode(pwm_pot_pin, INPUT);

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
    
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
    
}

void loop() {
  
  //the pot pin val sets the level for "on" on all 16 inputs
  int pwm_pot_val = analogRead(pwm_pot_pin);
  int pwm_val = map(pwm_pot_val, 0, 1023, 0, 4096);
  
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      if(j == i) pwm.setPin(thread_map[j], pwm_val, false);
      else pwm.setPin(thread_map[j], 0, false);
    }
  }
  
  delay(100); //we may not need this but might as well give it a little time to digest. 
}
