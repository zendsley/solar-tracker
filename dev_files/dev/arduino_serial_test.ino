void read_serial() {
  int select;
  int range;
  int precision;
  int x_val;
  int y_val;
  if (Serial.available() >0) {
    select = Serial.read();
//    range = Serial.read();
//    precision = Serial.read();
//    x_val = Serial.read();
//    y_val = Serial.read();
  }
  Serial.print("value=");
  Serial.println(select, DEC);
  int hex;
//  Serial.print("range=");
//  Serial.println(range, DEC);
//  Serial.print("precision=");
//  Serial.println(precision, DEC);
//  Serial.print("x_val=");
//  Serial.println(x_val, DEC);
//  Serial.print("y_val=");
//  Serial.println(y_val, DEC);

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    read_serial();
  }
}
