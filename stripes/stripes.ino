/*************************************************** 
 STRIPE CONTROL
  a pot controls "on" level for all 10 stripes
  thread_map stores a list of pwm controller pin mappings for the stripes as read top to bottom
  code cycles through each stripe, turning one on at a time at a rate that will allow for color change but not a power overload
 //turns at 2882
 
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int pwm_pot_pin = A6;

//update these based on wiring
int thread_map[10] ={0, 3, 8, 5, 1, 2, 6, 9, 4, 7};
int active[10] = {0, 1, 0, 1, 0, 1, 0, 0, 0, 0};
int active_id = 0;
int cycles = 0;
int interval = 4;


void setup() {
  Serial.begin(9600);

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
  
  //pwm.setPin(15, pwm_val, false);
  
  cycleInOrder();

}

void cycleThroughActive(int pwm_val){
  if(active[active_id] == 1){
    pwm.setPin(thread_map[active_id], pwm_val, false);
    Serial.println(active_id+1);
    if(cycles % interval == 0){
      pwm.setPin(thread_map[active_id], 0, false); //turn this thread off
      active_id = (active_id +1) % 10;
      cycles = 1;
    }else{
      cycles++;
    }
  }else{
    //make sure the thread is off and advance past this index since it's zero
    pwm.setPin(thread_map[active_id], 0, false); //turn this thread off
    active_id = (active_id +1) % 10;
  }
}


void cycleInOrder(){
  //the pot pin val sets the level for "on" on all 16 inputs
  int pwm_pot_val;
  int pwm_val;
  
  for(int i = 0; i < 10; i++){
      pwm_pot_val = analogRead(pwm_pot_pin);
      pwm_val = map(pwm_pot_val, 0, 1023, 0, 4096);
      
      Serial.print(pwm_val); Serial.print(" ");
      
      for(int j = 0; j < 10; j++){
        if(j == i) pwm.setPin(thread_map[j], pwm_val, false);
        else pwm.setPin(thread_map[j], 0, false);
      }
      Serial.println(i);
      
      delay(18000); //we may not need this but might as well give it a little time to digest.  
   }
}
