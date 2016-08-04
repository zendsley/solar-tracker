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
int sweeptype;
int input;
int range;
int precision;
int x;
int y;
void loop() {
  if (Serial.available()) {
    sweeptype=Serial.read()-'0';
    if (sweeptype==1){
      Serial.print("full sweep!");
      Serial.print("\n");
      delay(20);
      while (Serial.available()) {
        if ((Serial.read()-'0')==-4){
          while (Serial.available()) {
            input=Serial.read()-'0';
            if (input==-4){
              break;
            }
            range=range*10;
            range += input;
            delay(20); 
          }
          Serial.print("range:");
          Serial.print(range);
          Serial.print("\n");
          while (Serial.available()) {
            input=Serial.read()-'0';
            if (input==-4){
              break;
            }
            precision=precision*10;
            precision += input;
            delay(20); 
          }
          Serial.print("precision:");
          Serial.print(precision);
          Serial.print("\n");
          while (Serial.available()) {
            input=Serial.read()-'0';
            if (input==-4){
              break;
            }
            x=x*10;
            x += input;
            delay(20); 
          }
          Serial.print("x:");
          Serial.print(x);
          Serial.print("\n");
          while (Serial.available()) {
            input=Serial.read()-'0';
            if (input==-4){
              break;
            }
            y=y*10;
            y += input;
            delay(20); 
          }
          Serial.print("y:");
          Serial.print(y);
          Serial.print("\n");
        }
      }
    }
    else if (sweeptype == 2){
      Serial.print("enhance scan");
      Serial.print("\n");
    }
  }
}
void full_scan(float precision) {
  float pi=3.14159;
  float DP_inc=precision; //data point increment
  float V_inc=DP_inc; //vertical increment
  float H_arc_len;
  float H_inc;
  float i;
  float j;
  for (i=0; i<180; i=i+V_inc) {
        servo1.write(i);
        delay(100);
        H_arc_len=pi*57.296*sin(pi*i/180);
        H_inc=abs(DP_inc*180/H_arc_len);
        if (H_inc>180) {
          H_inc=91;
        }
        if (j<=0) {
          for (j=0; j<180; j=j+H_inc) {
          servo2.write(j);
          photocellValue = analogRead(photocellPin);  
          photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
     
          // change brightness of LED depending on light intensity of photocell
          int ledbrightness = map(photocellValue, 0, 1500, 0, 255);
      
          Serial.print("photocell=");
          Serial.print(photocellValue);
          Serial.print("\n");
          Serial.print("x_deg=");
          Serial.print(i);
          Serial.print("\n");
          Serial.print("y_deg=");
          Serial.print(j);
          Serial.print("\n");
          analogWrite(ledPin, ledbrightness);  
  
          // Serial Output to Rasp Pi
          // Format: photocellValue,x_deg,y_deg
          Serial.print(photocellValue);
          Serial.print(",");
          Serial.print(i);
          Serial.print(",");
          Serial.print(j);
          Serial.print("\n");
          //delay(1);
          }
        }
        else if (j>=180) {
          for (j=180; j>0; j=j-H_inc) {
          servo2.write(j);
          photocellValue = analogRead(photocellPin);  
          photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
     
          // change brightness of LED depending on light intensity of photocell
          int ledbrightness = map(photocellValue, 0, 1500, 0, 255);
  
          // Debugging Text output
          Serial.print("photocell=");
          Serial.print(photocellValue);
          Serial.print("\n");
          Serial.print("x_deg=");
          Serial.print(i);
          Serial.print("\n");
          Serial.print("y_deg=");
          Serial.print(j);
          Serial.print("\n");
  
          // Serial Output to Rasp Pi
          // Format: photocellValue,x_deg,y_deg
          Serial.print(photocellValue);
          Serial.print(",");
          Serial.print(i);
          Serial.print(",");
          Serial.print(j);
          Serial.print("\n");
          
          analogWrite(ledPin, ledbrightness);  
          //delay(1);
          }
        }
    }
}


