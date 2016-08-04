#include <AFMotor.h>
#include <Servo.h>

// DC servo motors
Servo servo1;  
Servo servo2;
int photocellPin = A0;    // photocell sensor input
int ledPin = 11;      // select the pin for the LED
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

int input;

// Variables used to determine sweep patter; expected to be read from serial stream
int select;
int range;
int precision;
int x;
int y;

//void loop() {
//  // look for available serial data in the format select,range,precision,x,y
//  if (Serial.available() > 0) {
//    select=Serial.read()-'0';
//    if (select==1){
//      Serial.print("Starting Scan with inputs:");
//      Serial.print("\n");
//      delay(20);
//      while (Serial.available()) {
//        //since the code subracts the character '0' (which is read as 48) from the serial input
//        //and a ',' is 44, we need to search for -4 to separate the separate input values 
//        if ((Serial.read()-'0')==-4){
//
//          //for each input value consisting of more than one digit we need to iterate through
//          //each digit, multiply by ten, and take in the next digit until a ',' is found
//          while (Serial.available()) {
//            input=Serial.read()-'0';
//            if (input==-4){
//              break;
//            }
//            range=range*10;
//            range += input;
//            delay(20); 
//          }
//          Serial.print("range=");
//          Serial.print(range);
//          Serial.print("\n");
//          while (Serial.available()) {
//            input=Serial.read()-'0';
//            if (input==-4){
//              break;
//            }
//            precision=precision*10;
//            precision += input;
//            delay(20); 
//          }
//          Serial.print("precision=");
//          Serial.print(precision);
//          Serial.print("\n");
//          while (Serial.available()) {
//            input=Serial.read()-'0';
//            if (input==-4){
//              break;
//            }
//            x=x*10;
//            x += input;
//            delay(20); 
//          }
//          Serial.print("x=");
//          Serial.print(x);
//          Serial.print("\n");
//          while (Serial.available()) {
//            input=Serial.read()-'0';
//            if (input==-4){
//              break;
//            }
//            y=y*10;
//            y += input;
//            delay(20); 
//          }
//          Serial.print("y=");
//          Serial.print(y);
//          Serial.print("\n");
//        }
//      }
//    }
//    scan(range, precision,x,y); // initiates scan with the given inputs
//    // reset inputs back to zero to ready for next scan
//    range = 0; 
//    precision = 0;
//    x=0;
//    y=0;
//  }
//}
//


// New Test loop

int InstArray[5];
int temp;
int count;

void read_serial(int InstArray[]) {
 if (Serial.available() > 0) {
    //read_serial(InstArray, command_number);
    temp = Serial.read();
    Serial.print("temp=");
    Serial.println(temp);
    Serial.print("count=");
    Serial.println(count);
    InstArray[count] = int(temp);
    count++;
    Serial.print("InstArray=");
    for (int i; i<5; i++) {
      Serial.println(InstArray[i]);
    }
  }
  

  
  if (count == 5) {
    count = 0;
  }
}

void loop() {
  read_serial(InstArray);
  
}


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
  //finding the maximums and minimum values for i and j and adjusting their ranges 
  //such that they do not exceed the limits of the servos
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
  
  for (i=i_range_min; i<i_range_max; i=i+V_inc) {
        servo1.write(i);
        delay(100);
        //calculate the horizontal increment based on the horizontal arc lenth to 
        //maintain even spacing between the points where data is taken
        H_arc_len=pi*57.296*sin(pi*i/180);
        H_inc=abs(DP_inc*180/H_arc_len);
        if (H_inc>180) {
          H_inc=91;
        }
        if (j<=j_range_min) {
          for (j=j_range_min; j<j_range_max; j=j+H_inc) {
          servo2.write(j);
          photocellValue = analogRead(photocellPin);  
          photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
     
          // change brightness of LED depending on light intensity of photocell
          int ledbrightness = map(photocellValue, 0, 1500, 0, 255);

            // Debugging Text output
//          Serial.print("photocell=");
//          Serial.print(photocellValue);
//          Serial.print("\n");
//          Serial.print("x_deg=");
//          Serial.print(i);
//          Serial.print("\n");
//          Serial.print("y_deg=");
//          Serial.print(j);
//          Serial.print("\n");
//          analogWrite(ledPin, ledbrightness);  
  
          // Serial Output to Rasp Pi
          // Format: photocellValue,x_deg,y_deg
          Serial.print(photocellValue);
          Serial.print(",");
          Serial.print(i);
          Serial.print(",");
          Serial.print(j);
          Serial.print(";");
          Serial.print("\n");
          //delay(1);
          }
        }
        else if (j>=j_range_max) {
          for (j=j_range_max; j>j_range_min; j=j-H_inc) {
          servo2.write(j);
          photocellValue = analogRead(photocellPin);  
          photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
     
          // change brightness of LED depending on light intensity of photocell
          int ledbrightness = map(photocellValue, 0, 1500, 0, 255);
  
          // Debugging Text output
//          Serial.print("photocell=");
//          Serial.print(photocellValue);
//          Serial.print("\n");
//          Serial.print("x_deg=");
//          Serial.print(i);
//          Serial.print("\n");
//          Serial.print("y_deg=");
//          Serial.print(j);
//          Serial.print("\n");
  
          // Serial Output to Rasp Pi
          // Format: photocellValue,x_deg,y_deg
          Serial.print(photocellValue);
          Serial.print(",");
          Serial.print(i);
          Serial.print(",");
          Serial.print(j);
          Serial.print(";");
          Serial.print("\n");
          analogWrite(ledPin, ledbrightness);  
          //delay(1);
          }
        }
      }
    }



