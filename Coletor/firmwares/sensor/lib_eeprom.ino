/*******************************************************************************
* EEPROM Library
*******************************************************************************/

// Functions

/*******************************************************************************
Save config (EEPROM)
Inputs:
- Config array
Return:
- void
*******************************************************************************/
void config2eeprom(byte* cfg_array) {
  for(size_t i=0; i<(CFG_SIZE-1); i++) {
    EEPROM.write(i, cfg_array[i]);
  }
}

/*******************************************************************************
EEPROM Debug - Print config
Inputs:
- void
Return:
- void
*******************************************************************************/
void eepromDebug(void) {
  Serial.println("");
  Serial.println("---------- Config ----------");
  for(size_t i=0; i<(CFG_SIZE-1); i++) {
    Serial.print("[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.print(EEPROM.read(i));
    Serial.println("");
  }
  Serial.println("----------------------------");
}
