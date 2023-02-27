double timeC; // convert milliseconds to seconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  double t1 = millis();
  delay(1000);
  double t2 = millis();

  timeC = timeC + (t2-t1)/1000;
  Serial.println(timeC);

  

}
