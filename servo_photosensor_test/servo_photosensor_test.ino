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

int i;
int j;

void loop() {
  for (i=0; i<18; i++) {
    // move the x servo 
    servo1.write(i*10);
    delay(100);
    for (j=0; j<18; j++) {
      servo2.write(j*10);
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
    
    servo2.write(0);

  }
}
