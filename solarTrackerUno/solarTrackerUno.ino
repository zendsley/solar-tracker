// Solar Tracker Uno
// SIUE Solar Racing Team
// Authors: Drew Dunham, Zack Endsley
// Course: ECE492/ME492
// Date: 2016_08_05
// Description: This Uno code is designed to be paired with a python server to power
// a set of servo motors and photocell via serial communication to track the brightest
// point in the sky.

#include <AFMotor.h>
#include <Servo.h>

// DC servo motors
Servo servo1;  
Servo servo2;
int photocellPin = A0;    // photocell sensor input
int photocellValue = 0;  // variable to store the value coming from the photocell val

void setup() {
  Serial.begin(9600);         // set up Serial library at 9600 bps

  // turn on the servos
  servo1.attach(10);  //The servo labels appear to be backwards as listed on the board
  servo2.attach(9);

  // set the positions to 0 when starting up
  servo1.write(0);
  servo2.write(0);
}

// Array for storing serial input from rasp pi
int InstArray[5];  //Expecting 5 values; select, range, precision, x_val, y_val
int temp;
int count;

// This bool is used to determine when to read/write data 
bool serial_input = true;

// Read in available serial values and store them in Array
void read_serial(int InstArray[]) {
 if (Serial.available() > 0) {
    temp = Serial.read();
    InstArray[count] = int(temp);
    count++;
  }

  // Setup bool to write instead of read once all 5 values are recorded
  if (count == 5) {
    count = 0;
    serial_input = false;
  }
  
}

// Scanning algorithm to move servos based on user input
void scan(float range,float precision,float x,float y) {
  float pi=3.14159;
  float DP_inc=precision; //data point increment
  float V_inc=DP_inc; //vertical increment
  float H_arc_len; //horizontal arc lenght
  float H_inc; //horizontal increment
  float i;
  float j;
  float i_range_max;
  float i_range_min;
  float j_range_max;
  float j_range_min;
  
  servo1.write(x);
  servo2.write(y);
  
  delay(100);
  
  // find the maximum and minimum values for i and j and adjust their ranges 
  // such that they do not exceed the limits of the servos
  i_range_max=x+range/2;
  if (i_range_max>180){
    i_range_max=180;
  }
  i_range_min=x-range/2;
  if (i_range_min<0){
    i_range_min=0;
  }
  j_range_max=y+range/2;
  if (j_range_max>180){
    j_range_max=180;
  }
  j_range_min=y-range/2;
  if (j_range_min<0){
    j_range_min=0;
  }
  
  // begin scanning process
  for (i=i_range_min; i<i_range_max; i=i+V_inc) {
        servo1.write(i);
        delay(100);
        
        // calculate the horizontal increment based on the horizontal arc lenth to 
        // maintain even spacing between the points where data is taken
        H_arc_len=pi*57.296*sin(pi*i/180);   
        H_inc=abs(DP_inc*180/H_arc_len);
        
        // Prevents skipping rows in arc if H_inc value is too large
        if (H_inc>180) {
          H_inc=91;
        }
        
        // Min range -> Max range
        if (j<=j_range_min) {
          for (j=j_range_min; j<j_range_max; j=j+H_inc) {
            servo2.write(j);
            delay(100);   // give photocell time to react
            photocellValue = analogRead(photocellPin);  
            photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
    
            // Serial Output to Rasp Pi
            // Format: photocellValue,x_deg,y_deg
            Serial.print(photocellValue);
            Serial.print(",");
            Serial.print(i);
            Serial.print(",");
            Serial.print(j);
            Serial.print("\n");
          }
        }
        
        // Max range -> Min range
        else if (j>=j_range_max) {
          for (j=j_range_max; j>j_range_min; j=j-H_inc) {
            servo2.write(j);
            delay(100);  // give photocell time to react
            photocellValue = analogRead(photocellPin);  
            photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
            
            // Serial Output to Rasp Pi
            // Format: photocellValue,x_deg,y_deg
            Serial.print(photocellValue);
            Serial.print(",");
            Serial.print(i);
            Serial.print(",");
            Serial.print(j);
            Serial.print("\n");
          }
        }
      }
    }

void loop() {
  // Populate Array with instructions from Rasp Pi source
  read_serial(InstArray);
  delay(200);
  
  // Check if select = 1 and if serial_input is in write mode
  if (InstArray[0] == 1 && serial_input==false) {
    if (Serial.availableForWrite() > 0) {
      scan(InstArray[1],InstArray[2],InstArray[3],InstArray[4]);
      serial_input=true;
    }
  }
  
}


