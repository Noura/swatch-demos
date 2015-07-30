/*************************************************** 
  COLOR GRID
  uses 6 pot control to manually turn on and off power to each warp and weft thread. This code makes no effort to limit power draw from battery
  so power must be limited manually. We need to find a way to keep marks on the boxes of where the pot should be calibrated to.
 ****************************************************/

                      //blue, red, yellow
int warp_pot_pin[3] = {A0, A1, A2}; 
int weft_pot_pin[3] = {A3, A4, A5}; 

int warp_pot_val[3] = {0, 0, 0}; 
int weft_pot_val[3] = {0, 0, 0}; 

//pwm pins are 3, 5, 6, 9, 10, and 11
                      //blue,red, yellow
int warp_thread[3] = {11, 9, 6};
                      //11 ,9, 6 
int weft_thread[3] = {3, 5, 10};
                      //3, 5, 10


void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 3; i++){
   pinMode(warp_pot_pin[i], INPUT);
  }
  
  for(int i = 0; i < 3; i++){
   pinMode(weft_pot_pin[i], INPUT);
  }
    
}

void loop() {
  updatePotVals();
  
  for(int i = 0; i < 3; i++){
    analogWrite(warp_thread[i], warp_pot_val[i]);
  }
  
  for(int i = 0; i < 3; i++){
    analogWrite(weft_thread[i], weft_pot_val[i]);
  }
  
  
  delay(100); //we may not need this but might as well give it a little time to digest. 
}

void updatePotVals(){
  int pwm_pot_val = 0;
  for(int i = 0; i < 3; i++){
    pwm_pot_val = analogRead(warp_pot_pin[i]);
    warp_pot_val[i] = map(pwm_pot_val, 0, 1023, 0, 255);
  }
  
   for(int i = 0; i < 3; i++){
    pwm_pot_val = analogRead(weft_pot_pin[i]);
    weft_pot_val[i] = map(pwm_pot_val, 0, 1023, 0, 255);
  }
}
