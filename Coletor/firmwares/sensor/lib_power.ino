/*******************************************************************************
* Power Library
*******************************************************************************/

// Functions

/*******************************************************************************
Read battery voltage
Inputs:
- void
Return:
- void
*******************************************************************************/
int batVoltageRead(void) {
  int voltage = 0;
//  Serial.println("Read Battery Voltage!");
  // enableBatRead(true);
  voltage = map(analogRead(ANALOG_BAT_PIN), 0, 1023, 0, 255);
  // enableBatRead(false);
  //Serial.println(voltage);
  return voltage;
}

void enableBatRead(bool value) {
  if(value) {
    Serial.println("Enable Battery Read");
    digitalWrite(GET_BAT_PIN, HIGH);
  } else {
    Serial.println("Disable Battery Read");
    digitalWrite(GET_BAT_PIN, LOW);
  }
}
