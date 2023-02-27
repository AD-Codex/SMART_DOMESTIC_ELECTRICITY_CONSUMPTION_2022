
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

int M_send = 0;
double timeCount;
float OnekwhPrice = 100.00;

// ------------------- gsm module ------------------------------------------
SoftwareSerial mySerial(3, 4); //SIM800L Tx & Rx is connected to Arduino #2 & #3
String message = "";

// -------------------- lcd display --------------------------------------
LiquidCrystal lcd( 13, 12, 11, 10, 9, 8);
float current = 0.00;
float kwh = 0.00;
float power = 0.00;
float rs = 0.00;

// -------------------- current sensor -------------------------------- 
//#define calibration_const 355.55
//float cur_val;
//float cur_Samples = 0.0;
float Avg_cur = 0.0;

int mVperAmp = 185;           // this the 5A version of the ACS712 -use 100 for 20A Module and 66 for 30A Module
double sensorRead = 0;
double ReadRMS = 0;
double AmpsRMS = 0;

// -------------------- voltage sensor -------------------------------
//float vol_read;
//float vol_Sample = 0.0;
float RMSvol_mean = 0.0;

double VsensorRead = 0;
double VReadRMS = 0;




void setup() {
  
  pinMode( A0, INPUT);      // current
  pinMode( A1, INPUT);      // voltage
  pinMode( A2, OUTPUT);      // Relay
  pinMode( 5, OUTPUT);      // green LED
  pinMode( 6, OUTPUT);      // yellow LED
  pinMode( 7, OUTPUT);      // red LED

  attachInterrupt(digitalPinToInterrupt(2), Rset, FALLING);    // interrupt

  digitalWrite(A2, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  
  lcd.begin(16,2);

  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(500);

  int delayTime = 1000;
  mySerial.println("AT\r"); //Once the handshake test is successful, it will back to OK
  GSM_update();
  delay(delayTime);
  mySerial.println("AT+CSQ\r"); //Signal quality test, value range is 0-31 , 31 is the best
  GSM_update();
  delay(delayTime);
  mySerial.println("AT+CCID\r"); //Read SIM information to confirm whether the SIM is plugged
  GSM_update();
  delay(delayTime);
  mySerial.println("AT+CREG?\r"); //Check whether it has registered in the network
  GSM_update();
  delay(delayTime);
  mySerial.println("AT+CMGF=1\r"); // Configuring TEXT mode
  GSM_update();
  delay(delayTime);
  mySerial.println("AT+CNMI=1,2,0,0,0\r"); // Decides how newly arrived SMS messages should be handled
  GSM_update();
  delay(delayTime);

  delay(5000);
  Serial.println("");
  Serial.println("-------- starting-----------");

}

void loop() {
  double t1 = millis();

  if ( kwh >= 0 && kwh < 8) {
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  else if ( kwh >= 8 && kwh < 10) {
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }
  else if ( kwh >= 10) {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }

  // sending messsage if kwh higher that given value
  if ( kwh >= 10 && M_send == 0) {
    mySerial.println("AT+CMGF=1");                          // Configuring TEXT mode
    GSM_update();
    mySerial.println("AT+CMGS=\"+94728637971\"");           // change phone number
    GSM_update();
    mySerial.print("power overload");                       //text content
    GSM_update();
    mySerial.write(26);
    Serial.println("message send");
    M_send = 1;      
  }
  
  
  
  current_Read();
  voltage_Read();

  GSM_update();

  current = Avg_cur/1000;                   // current in A
  power = current * RMSvol_mean/1000;            // calculate power kW
  
  double t2 = millis();
  timeCount = timeCount + (t2-t1)/1000;
  
  kwh = kwh + (power * timeCount)/3600;     // calculate kmh
  timeCount = 0.0;

  if ( kwh <= 5) {
    OnekwhPrice = 100;
  }
  else if ( kwh <= 10) {
    OnekwhPrice = 120;
  }
  else if ( kwh <= 20) {
    OnekwhPrice = 140;
  }
  else if ( kwh <= 40) {
    OnekwhPrice = 160;
  }
  else{
    OnekwhPrice = 200;
  }
  rs = kwh * OnekwhPrice;                   // calculate cost

  Serial.println(current);
  Serial.println(RMSvol_mean);
  Serial.println(power);
  Serial.println(kwh);
  Serial.println(rs);
  lcd_Display();

  Serial.println("\n");  
  double t3 = millis();
  timeCount = timeCount + (t3-t2)/1000;
  

}



// GSM code -----------------------
void GSM_update() {
  message = "";
  delay(1000);
  
  if (Serial.available()) {
    mySerial.write(Serial.read());
    
    while (Serial.available() >0) {
      byte dumpByte = Serial.read();            //Forward what Serial received to Software Serial Port
    }
  }
  
  while (mySerial.available() > 1) {
    char c = mySerial.read();
    message = message + c ;
  }

//  Serial.println("vvvvvvvvvvvvv");
  Serial.print(message);
//  Serial.println("^^^^^^^^^^^^^^^^^");
//  Serial.println(message.substring(51));

  if ( message.substring(51,54) == "OFF") {
     Serial.println("===============switch off==============");
     digitalWrite( A2, HIGH);                  // on the relay
  }
  else if( message.substring(51,53) == "ON") {
    digitalWrite( A2, LOW);                  // off the relay
  }
  else if ( message.substring(51,58) == "details") {
    char R_value1[10];
    char R_value2[10];
    char R_value3[10];
    char R_value4[10];
    char text[40];
    dtostrf(current,5,2, R_value1);
    dtostrf(kwh, 5, 1, R_value3);
    dtostrf(RMSvol_mean, 5, 1, R_value2);
    dtostrf(rs, 5, 1, R_value4);
    sprintf( text, "%sA, %sV, %skwh, %s/=", R_value1, R_value2, R_value3, R_value4);
    
    Serial.println("Sending details");
    mySerial.println("AT+CMGF=1");                          // Configuring TEXT mode
    GSM_update();
    mySerial.println("AT+CMGS=\"+94728637971\"");           // change phone number
    GSM_update();
    mySerial.print(text);                                   //text content
    GSM_update();
    mySerial.write(26);
    Serial.println("message send");
  }

}


void Rset() {
  kwh = 0.00;
  power = 0.00;
  rs = 0.00;
}
