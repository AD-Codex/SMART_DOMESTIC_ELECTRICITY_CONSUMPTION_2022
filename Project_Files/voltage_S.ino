
void voltage_Read() {
//  vol_Sample = 0.0;
//
//  for ( int i=0; i< 100; i=i+1) {
//    vol_read = analogRead(A1)-512;
//    vol_Sample = vol_Sample + sq(vol_read);
//  
//  }
//
//  float vol_mean = vol_Sample/100.0;
//  RMSvol_mean = (sqrt(vol_mean))*2.5;

//  Serial.print("voltage :");
//  Serial.print(RMSvol_mean);        // print rms voltage
//  Serial.println(" V");

// -------------------------------------------------
//  RMSvol_mean = 0.0;
    
  VsensorRead = getVVPP();
  VReadRMS = (VsensorRead/2.0) *0.707;   //root 2 is 0.707
  RMSvol_mean = (VReadRMS )*2.8532;
 
}

float getVVPP()
{
  float Vresult;
  int VreadValue;                // value read from the sensor
  int VmaxValue = 0;             // store max value here
  int VminValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       VreadValue = analogRead(A1);
       // see if you have a new maxValue
       if (VreadValue > VmaxValue) 
       {
           /*record the maximum sensor value*/
           VmaxValue = VreadValue;
       }
       if (VreadValue < VminValue) 
       {
           /*record the minimum sensor value*/
           VminValue = VreadValue;
       }
   }
   
   // Subtract min from max
   Vresult = ((VmaxValue - VminValue) );
   Serial.println(Vresult);
      
   return Vresult;
 }
