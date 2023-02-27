 /*
current reading
*/


#define calibration_const 355.55

float cur_val;
float cur_Samples = 0.0;
float Avg_cur = 0.0;

void setup() {
  pinMode( A0, INPUT);
  Serial.begin(9600);

}

void loop() {
  cur_val = analogRead(A0);
  Serial.println(cur_val);
  
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
//  Serial.print("current : ");
//  Serial.println( Avg_cur);


  
}
