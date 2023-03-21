void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(19200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Hello world!");
}

int number = 0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(number);
  delay(500);
  number++;
  if(number == 100) number=0;
}
