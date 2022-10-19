/*******************************************************************************
* Config Library
*******************************************************************************/

// Functions

/*******************************************************************************
Device Config
Inputs:
- void
Return:
- void
*******************************************************************************/
void devConfig(void) {

  Serial.write(CFG_START, sizeof(CFG_START));

  byte ui8_buffer_RTC[5];
  int bytes = serialRead(data_read);
  int checksum = checksumCalc(data_read, bytes-1);

  if ((bytes == CFG_SIZE) && (checksum == data_read[bytes-1]))
  {
    for(size_t i=0; i<(CFG_SIZE-1); i++)
    {
      cfg_array[i] = data_read[i];
    }
    config2eeprom(cfg_array);    
    for(size_t i=6; i<(CFG_SIZE-1); i++)
    {
      // [ID, Type, Acqui_1, Acqui_2, Trans, RTC_1, RTC_2, RTC_3, RTC_4, RTC_5, checksum]
      ui8_buffer_RTC[i-6] = data_read[i];
//      Serial.print(" ");
//      Serial.print(ui8_buffer_RTC[i-6]);
    } 
    configRTC (ui8_buffer_RTC);
    Serial.write(RC_OK);
    delay(5000);
  }
  else
  {
    Serial.write(RC_NOK);
  }

}

void configRTC(byte* ui8_buffer_RTC)
{
  uint32_t ui32_unix_stamp = 0;
    
  ui32_unix_stamp = (ui32_unix_stamp | ui8_buffer_RTC[0])<<8;
  ui32_unix_stamp = (ui32_unix_stamp | ui8_buffer_RTC[1])<<8;
  ui32_unix_stamp = (ui32_unix_stamp | ui8_buffer_RTC[2])<<8;
  ui32_unix_stamp = (ui32_unix_stamp | ui8_buffer_RTC[3]);
  
  tm.Year = CalendarYrToTm(year(ui32_unix_stamp));
//  Serial.write(year(ui32_unix_stamp));
  tm.Month = month(ui32_unix_stamp);
//  Serial.write(tm.Month);
  tm.Day = day(ui32_unix_stamp);
//  Serial.write(tm.Day);
  tm.Hour = hour(ui32_unix_stamp);
//  Serial.write(tm.Hour);
  tm.Minute = minute(ui32_unix_stamp);
//  Serial.write(tm.Minute);
  tm.Second = second(ui32_unix_stamp);  
//  Serial.write(tm.Second);
  RTC.write(tm);  
}
