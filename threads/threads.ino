/*************************************************** 
 STRIPE CONTROL
  a pot controls "on" level for all 10 stripes
  thread_map stores a list of pwm controller pin mappings for the stripes as read top to bottom
  code cycles through each stripe, turning one on at a time at a rate that will allow for color change but not a power overload
 //turns at 2882
 
 ****************************************************/

int pwm_pot_pin = A0;

//update these based on wiring
int thread_map[3] ={9, 10, 11};
int active[10] = {1, 1, 1};
int active_id = 0;
int cycles = 0;
int interval = 1;


void setup() {
  Serial.begin(9600);

  pinMode(pwm_pot_pin, INPUT);

    
}

void loop() {
  
  //the pot pin val sets the level for "on" on all 16 inputs
  int pwm_pot_val = analogRead(pwm_pot_pin);
  int pwm_val = map(pwm_pot_val, 0, 1023, 0, 255);
   cycleThroughActive(pwm_val);

}

void cycleThroughActive(int pwm_val){
  if(active[active_id] == 1){
    analogWrite(thread_map[active_id], pwm_val);
    Serial.println(active_id+1);
    if(cycles % interval == 0){
      analogWrite(thread_map[active_id], 0);
      active_id = (active_id +1) % 3;
      cycles = 1;
    }else{
      cycles++;
    }
  }else{
    //make sure the thread is off and advance past this index since it's zero
    analogWrite(thread_map[active_id], 0);
    active_id = (active_id +1) % 3;
  }
}


void cycleInOrder(int pwm_val){
  for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        if(j == i) analogWrite(thread_map[j], pwm_val);
        else analogWrite(thread_map[j], 0);
      }
      Serial.println(i);
     delay(18000); //we may not need this but might as well give it a little time to digest. 
   }
}
