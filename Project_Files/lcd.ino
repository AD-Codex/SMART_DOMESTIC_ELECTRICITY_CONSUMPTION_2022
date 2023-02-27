
void lcd_Display() {
  char R_value[10];
  char text[10];

  lcd.clear();
  
  lcd.setCursor(0,0);
  dtostrf(current,5,2, R_value);
  sprintf( text, "%sA", R_value);
  Serial.println(text);
  lcd.print(text);

  lcd.setCursor(7,0);
  dtostrf(kwh, 6, 2, R_value);
  sprintf( text, "%skWh", R_value);
  Serial.println(text);
  lcd.print(text);

  lcd.setCursor(0,1);
  dtostrf(RMSvol_mean, 5, 1, R_value);
  sprintf( text, "%sV", R_value);
  Serial.println(text);
  lcd.print(text);

  lcd.setCursor(7,1);
  dtostrf(rs, 5, 1, R_value);
  sprintf( text, "%s/=", R_value);
  Serial.println(text);
  lcd.print(text);

}
