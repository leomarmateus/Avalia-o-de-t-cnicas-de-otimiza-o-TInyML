/*******************************************************************************
* Util Library
*******************************************************************************/

// Functions

/*******************************************************************************
Checksum XOR calc
Inputs:
- Data array
- Número de bytes
Return:
- checksum
*******************************************************************************/
int checksumCalc(byte *data, int bytes) {
  int checksum = 0;
  for(size_t i=0; i<bytes; i++) {
    checksum ^= data[i];
  }
  return checksum;
}

/*******************************************************************************
This function read Nbytes bytes from I2C device at address Address. 
 Put read bytes starting at register Register in the Data array. 
Inputs:
- Address
- Register
- Nbytes
- Data

Return:
- 
*******************************************************************************/
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
    // Set register address
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.endTransmission();
    
    // Read Nbytes
    Wire.requestFrom(Address, Nbytes);
    uint8_t index=0;
    while (Wire.available())
        Data[index++]=Wire.read();
}

/*******************************************************************************
Write a byte (Data) in device (Address) at register (Register):
Inputs:
- Address
- Register
- Data

Return:
- 
*******************************************************************************/
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
    // Set register address
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.write(Data);
    Wire.endTransmission();
}

/*******************************************************************************
Software reset MCU
Inputs:
-
Return:
- 
*******************************************************************************/
void(* resetFunc) (void) = 0;

/*******************************************************************************
Aquisição habilitada
Inputs:
-
Return:
- 
*******************************************************************************/
void callback_acquisition()
{
    flag_acquisition=true;
}

/*******************************************************************************
Envio de pacote via Lora
Inputs:
-
Return:
- 
*******************************************************************************/
void packet_send()
{
  static uint16_t ui16_counter_send = 1;    
//  if(flag_send_start)
//  {
//    LoRa.beginPacket();
//    for(int i = 0; i<4; i++)
//    {
//      LoRa.write(ui8_buffer_start[i]);
//    } 
//    LoRa.endPacket();
//    flag_send_start = false;            
//  }
//  else
//  {
    ui8_send_buffer[0] = ui8_ID_number;
    ui8_send_buffer[1] = ui8_type;
    ui8_send_buffer[2] = ui16_Time_aquisition>>8 & 0xFF;
    ui8_send_buffer[3] = ui16_Time_aquisition & 0xFF;
    ui8_send_buffer[4] = ui8_Time_transmition;
    ui8_send_buffer[5] = batVoltageRead();

    
    uint32_t ui32_unix_stamp = RTC.get(); 
//    Serial.print(ui32_unix_stamp);
    ui8_send_buffer[6] = ui32_unix_stamp>>24 & 0xFF;
    ui8_send_buffer[7] = ui32_unix_stamp>>16 & 0xFF;
    ui8_send_buffer[8] = ui32_unix_stamp>>8 & 0xFF;
    ui8_send_buffer[9]  = ui32_unix_stamp & 0xFF;
        
    ui8_send_buffer[10] = ui16_counter_send>>8 & 0xFF;
    ui8_send_buffer[11] = ui16_counter_send & 0xFF;
    
    LoRa.beginPacket();
    LoRa.write(ui8_packet_length);
    Serial.println(ui8_packet_length);
    for(int i = 0; i<ui8_packet_length; i++)
    {
      LoRa.write(ui8_send_buffer[i]);
//      Serial.println(ui8_send_buffer[i]);
    }
    LoRa.write(checksumCalc(ui8_send_buffer, ui8_packet_length));
//    Serial.println(checksumCalc(ui8_send_buffer, ui8_packet_length));
//    checksumCalc((byte)ui8_send_buffer, ui8_packet_length);
    LoRa.endPacket();
    ui16_counter_send++;
//  }
  ui8_index_buffer_send = DEF_SIZE_HEADER;
}

/*******************************************************************************
Monta pacote para envio
Inputs:
- ui8_send_buffer - buffer de envio
Return:
- 
*******************************************************************************/
void acc_gyro_temp_function(byte* ui8_send_buffer)
{ 
  // Read accelerometer, gyroscope and temperature
  I2Cread(MPU9250_ADDRESS,0x3B,14,ui8_Buf);
  
  // Accelerometer
  int16_t ax = (ui8_Buf[0]<<8 | ui8_Buf[1]);
  int16_t ay = (ui8_Buf[2]<<8 | ui8_Buf[3]);
  int16_t az = (ui8_Buf[4]<<8 | ui8_Buf[5]);
  
  int16_t i16_acc_total = sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
  
  ui8_most_significant = i16_acc_total>>8 & 0xFF;
  ui8_less_significant = i16_acc_total & 0xFF;
  
  if(ui8_less_significant>127)
    ui8_most_significant++;
  
  //armazenamento dos dados do acelerometro no buffer
  ui8_send_buffer[ui8_index_buffer_send] = ui8_most_significant;        
  
  // Gyroscope
  int16_t gx = (ui8_Buf[8]<< 8 | ui8_Buf[9] );
  int16_t gy = (ui8_Buf[10]<<8 | ui8_Buf[11]);
  int16_t gz = (ui8_Buf[12]<<8 | ui8_Buf[13]);       
  
  int16_t i16_gyro_total = sqrt(pow(gx,2)+pow(gy,2)+pow(gz,2));
  
  ui8_most_significant = i16_gyro_total>>8 & 0xFF;
  ui8_less_significant = i16_gyro_total & 0xFF;
  
  if(ui8_less_significant>127)
    ui8_most_significant++;
  
  //armazenamento dos dados do giroscopio no buffer
  ui8_send_buffer[ui8_index_buffer_send + ui8_size_data] = ui8_most_significant;        
  
  // Temperatura
  int16_t ui16_temperature = (ui8_Buf[6]<<8 | ui8_Buf[7]); 
  
  ui8_most_significant = ui16_temperature>>8 & 0xFF;
  ui8_less_significant = ui16_temperature & 0xFF;
  
  if(ui8_less_significant>127)
    ui8_most_significant++;
  
  //Conversão de temperatura retira da biblioteca MPU9250.h
  float f_temp = ((((float) ui16_temperature) - 21.0)/333.87) + 21.0;
  
  // COnversão para 1byte -> 100 graus = 255
  uint8_t ui8_send_temp = uint8_t((f_temp*255.0)/100.0);
  
  //temperatura
  ui8_send_buffer[ui8_index_buffer_send +2*ui8_size_data] = ui8_send_temp;              
  
  ui8_index_buffer_send++;
//  if (ui8_index_buffer_send >= ui8_size_data)
//    ui8_index_buffer_send = ui8_size_data;
}

/*******************************************************************************
Verifica se é o momento de enviar
Inputs:
- 
Return:
- 
*******************************************************************************/
void check_send ()
{
  if (RTC.read(tm))
  {
    if(tm.Second%ui8_Time_transmition == ui8_ID_number)
    {
      if(!flag_send_sec)
      {
        flag_send_sec = true;       
        packet_send();
      }      
    }
    else if(flag_send_sec)
    {
      flag_send_sec = false;
    }
    
  }
}
