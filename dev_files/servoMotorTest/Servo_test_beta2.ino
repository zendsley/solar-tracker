#include <AFMotor.h>
#include <Servo.h>

// DC servo motors
Servo servo1;  
Servo servo2;
int photocellPin = A0;    // photocell sensor input
int ledPin = 11;      // select the pin for the LED
int photocellValue = 0;  // variable to store the value coming from the photocell val

byte byteRead;

void setup() {
  Serial.begin(9600);         // set up Serial library at 9600 bps

  // turn on the servos
  servo1.attach(10);  //The servo labels appear to be backwards as listed on the board
  servo2.attach(9);

  // set the positions to 0 when starting up
  servo1.write(20);
  servo2.write(20);

}
float pi=3.14159;
float DP_inc=10; //data point increment
float V_inc=DP_inc; //vertical increment
float H_arc_len;
float H_inc;
float i;
float j;
int input = 0 ;
void loop() {
  if (Serial.available()){
    input=Serial.read();
    Serial.println(input);
    if (input==49) {

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
          Serial.println(photocellValue);
          Serial.print("x_deg=");
          Serial.println(i*10);
          Serial.print("y_deg=");
          Serial.println(j);
          analogWrite(ledPin, ledbrightness);  
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
      
          Serial.print("photocell=");
          Serial.println(photocellValue);
          Serial.print("x_deg=");
          Serial.println(i);
          Serial.print("y_deg=");
          Serial.println(j);
          analogWrite(ledPin, ledbrightness);  
          //delay(1);
          }
        }
      }
      Serial.println("end");
      input = 0;
    }
    else {
      Serial.println("waiting for input");
    }
  }
}

