
//void current_Read() {
//  cur_Samples = 0.0;
//  Avg_cur = 0.0;
//
//  for ( int i=0; i< 150; i=i+1) {
//    cur_val = analogRead(A0);
//    cur_Samples = cur_Samples + cur_val;
//    delay(3);
//  }
//
//  Avg_cur = (cur_Samples/150.0) * (5.0/1024.0) * calibration_const;
////  Serial.print("current : ");
////  Serial.print( Avg_cur);             //  print average current
////  Serial.println( " mA");
//  
//}

void current_Read() {
  Avg_cur = 0.0;
    
  sensorRead = getVPP();
  ReadRMS = (sensorRead/2.0) *0.707;   //root 2 is 0.707
  AmpsRMS = (ReadRMS * 1000)/mVperAmp;

  Avg_cur = AmpsRMS * 1000;
 
//  Serial.print(AmpsRMS);
//  Serial.println(" Amps RMS  ---  ");
}
 
 
float getVPP()
{
  float result;
  int readValue;                // value read from the sensor
  int maxValue = 0;             // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 500) //sample for 1 Sec
   {
       readValue = analogRead(A0);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the minimum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
