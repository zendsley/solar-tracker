/*

OSEPP example of measured ambient light intensity from photocell .
depending on brightness, an LED brightness is changed.
more light = brighter LED.

*/

int photocellPin = A0;    // photocell sensor input
int ledPin = 11;      // select the pin for the LED
int photocellValue = 0;  // variable to store the value coming from the photocell val

void setup() {
Serial.begin(9600);

}

void loop() {
  // read the value from the sensor:
  photocellValue = analogRead(photocellPin);  
photocellValue = constrain(photocellValue, 0, 1500); //adjust depending on environment.   
 
 
  // change brightness of LED depending on light intensity of photocell
  int ledbrightness = map(photocellValue, 0, 1500, 0, 255);
  
  Serial.print("incoming value from photocell sensor =");
  Serial.println( photocellValue);
  analogWrite(ledPin, ledbrightness);  
  delay(100);
           
}
