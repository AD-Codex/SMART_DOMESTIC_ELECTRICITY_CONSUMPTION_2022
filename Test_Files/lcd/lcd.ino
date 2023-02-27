#include <LiquidCrystal.h>

LiquidCrystal lcd( 13, 12, 11, 10, 9, 8);

float current = 0.00;
float kwh = 0.00;
float power = 0.00;
float rs = 0.00;
char text[10];

void setup() {
  lcd.begin(16,2);

}

void loop() {
  lcd.setCursor(0,0);
  sprintf( text, "%f A", current);
  lcd.print(text);

  lcd.setCursor(7,0);
  sprintf( text, "%f kWh", kwh);
  lcd.print(text);

  lcd.setCursor(0,1);
  sprintf( text, "%f W", power);
  lcd.print(text);

  lcd.setCursor(7,1);
  sprintf( text, "%f /=", rs);
  lcd.print(text);

}
