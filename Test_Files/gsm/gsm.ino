
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #2 & #3

String message = "";

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

//  int delayTime = 500;
//
//  mySerial.println("AT\r"); //Once the handshake test is successful, it will back to OK
//  updateSerial();
//  delay(delayTime);
//  mySerial.println("AT+CSQ\r"); //Signal quality test, value range is 0-31 , 31 is the best
//  updateSerial();
//  delay(delayTime);
//  mySerial.println("AT+CCID\r"); //Read SIM information to confirm whether the SIM is plugged
//  updateSerial();
//  delay(delayTime);
//  mySerial.println("AT+CREG?\r"); //Check whether it has registered in the network
//  updateSerial();
//  delay(delayTime);
//  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
//  updateSerial();
//  delay(delayTime);
//  mySerial.println("AT+CNMI=1,2,0,0,0\r"); // Decides how newly arrived SMS messages should be handled
//  updateSerial();
//  delay(delayTime);
////  mySerial.println("AT+COPS=?\r"); // Decides how newly arrived SMS messages should be handled
////  updateSerial();

//  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
//  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+94728637971\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("bokka"); //text content
  updateSerial();
  mySerial.write(26);

}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  message = "";
  delay(1000);
  if (Serial.available()) {
    mySerial.write(Serial.read());
    
    while (Serial.available() >0) {
      byte dumpByte = Serial.read();//Forward what Serial received to Software Serial Port
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
     Serial.println("===============off==============");    
  }

}
