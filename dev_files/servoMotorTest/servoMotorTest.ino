// Servo Motor Test
// Zackary Endsley
// 2016_07_31
// Borrows code from MotorParty example with the AFMotor shield to test Servos

#include <AFMotor.h>
#include <Servo.h>

// DC servo motors
Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600);         // set up Serial library at 9600 bps
  Serial.println("Servo Motor Test");

  // turn on the servos
  servo1.attach(10);
  servo2.attach(9);

  
  
  servo1.write(0);
  servo2.write(0);

}

int i;
int j;

void loop() {
  for (i=0; i<18; i++) {
    servo1.write(i*10);
    delay(500);
    for (j=0; j<180; j++) {
      servo2.write(j);
      delay(10);
    }
    servo2.write(0);

  }
  
//  servo1.write(10);
//  servo2.write(30);
//  delay(2000);
//  servo1.write(90);
//  servo2.write(90);
//  delay(2000);
//  servo1.write(180);
//  servo2.write(180);
//  delay(2000);

//  for (i=0; i<255; i++) {
//    servo1.write(i);
//    servo2.write(i);
//    delay(3);
//  }
//
//  for (i=255; i!=0; i--) {
//    servo1.write(i-255);
//    servo2.write(i-255);
//    delay(3);
//  }
//
//  for (i=0; i<255; i++) {
//    servo1.write(i);
//    servo2.write(i);  
//    delay(3);
//  }
// 
//  for (i=255; i!=0; i--) {
//    servo1.write(i-255);
//    servo2.write(i-255);
//    delay(3);
// }
}
