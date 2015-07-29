/*************************************************** 
 STRIPE CONTROL w/ BLUETOOTH
 a pot controls "on" level for all 10 stripes
  thread_map stores a list of pwm controller pin mappings for the stripes from top to bottom
  bluetooth will send a byte of data in binary indicating which stripe need to be turned on:
    i.e. 01010 means the second and forth stripes should be turned on
  active_map keeps a log of which stripes should be turned on.
  code cycles through each stripe that is turned on in active map, turning one on at a time at a rate that will allow for color change but not a power overload
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>  
#define BUFFER_SIZE 64

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int pwm_pot_pin = A0;

//update these based on wiring
int thread_map[10] ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int active_map[10] = {0,0,0,0,0,0,0,0,0,0};

int bluetoothTx = 3;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 4;  // RX-I pin of bluetooth mate, Arduino D3
char rx_buf[BUFFER_SIZE] = {0};
int rx_len = 0;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);


void setup() {
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
  
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$$$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600

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
   checkBluetooth();

  //the pot pin val sets the level for "on" on all 16 inputs
  int pwm_pot_val = analogRead(pwm_pot_pin);
  int pwm_val = map(pwm_pot_val, 0, 1023, 0, 4096);
  
  //cycle through all values
  for(int i = 0; i < 9; i++){
    //turn only 1 on at a time, and only if it's active.
    for(int j = 0; j < 9; j++){
      if(j == i && active_map[j]== 1) pwm.setPin(thread_map[j], pwm_val, false);
      else pwm.setPin(thread_map[j], 0, false);
    }
  }
  
  delay(100); //we may not need this but might as well give it a little time to digest. 
}

void bufferData(char c) {
  if (rx_len < BUFFER_SIZE) {
      rx_buf[rx_len++] = c;
  }
}


void checkBluetooth(){
  //receive incoming data
  if (bluetooth.available()) {
    while (bluetooth.available()) {
      char c = bluetooth.read();
      if (c == 0xA || c == 0xD) { // \n or \r
        //add the NULL character as EOF
        bufferData(NULL);
        digestData();
      } 
      else {
        bufferData(c);
      }
    }
  }
}

void digestData(){
   
  Serial.write("<- ");
  Serial.write(rx_buf[0]);
  int value = atoi(rx_buf+1);
  Serial.println(value);
  
  //b prefix means bus data is coming in 
  if(rx_buf[0] == 'b'){
    parseBits(value);
    
  //c prefix means all values should be cleared
  }else if(rx_buf[0] == 'c'){
    for(int i = 0; i < 10; i++){
      active_map[i] = 0;
    }
  }
   
  rx_len = 0;
  bluetooth.flush();   
}

//this looks at the incoming byte and tells you which lights to turn on.
//currently this only works for the first 5 stripes but we can make it bigger if it works
void parseBits(int x){
  int check[5] = {1, 2, 4, 8, 16};
  for(int i = 4; i >= 0; i--){
    if(x - check[i] >= 0){
      active_map[i] = 1;
      x -= check[i];
    }else{
      active_map[i] = 0;
    }
  }
}

