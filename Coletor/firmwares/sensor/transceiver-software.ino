// -----------------------------------------------------------------------------

#include <Wire.h>
#include <TimerOne.h>
#include <math.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <LoRa.h>
#include <avr/wdt.h>
#include "defines.h"
#include "define-variable.h"

// -----------------------------------------------------------------------------

void setup() 
{
  // serial config
  Serial.begin(115200);
  while (!Serial) {}

  devConfig();
  eepromDebug();

  // Arduino initializations
  #ifdef versao_1
    pinMode(LED_ERROR_1, OUTPUT);
    digitalWrite(LED_ERROR_1, LOW);
    pinMode(LED_ERROR_2, OUTPUT);
    digitalWrite(LED_ERROR_2, LOW);
  #else
    pinMode(AD0, OUTPUT);
    digitalWrite(AD0, HIGH);    
  #endif    
  
  analogReference(EXTERNAL);
  
  Serial.print("RTC Read 0");
  
  if (!RTC.read(tm))
  {
    ui8_error = 1;
    Serial.print("ERROR!!!");
  }

  Serial.print("RTC Read 1");
  
  if(ui8_error == 0)
  {
    // override the default CS, reset, and IRQ pins (optional)
    //LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin
    LoRa.setPins(10, 6, 7);// set CS, reset, IRQ pin placa V0
    
    if (!LoRa.begin(915E6))
    {
      //Serial.println("Starting LoRa failed!");
      while (1);
    }

    LoRa.setSpreadingFactor(8);
  }
  //Configura watchdog para 4 segundos
//  wdt_enable(WDTO_4S);

  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);    
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
  
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);
  
  // Leitura dos Dados da EEPROM
  ui8_ID_number = EEPROM.read(0);
  ui8_type = EEPROM.read(1);
  ui16_Time_aquisition = EEPROM.read(2)<<8 | EEPROM.read(3);
  ui8_Time_transmition = EEPROM.read(4);      
  
  uint32_t ui32_interrupt_value = uint32_t(ui16_Time_aquisition*1000.0);
  
  Timer1.initialize(ui32_interrupt_value); //Inicializa Timer1 período de 0,25 segundos = 250000
  Timer1.attachInterrupt(callback_acquisition); //Configura a função callback() como a função para ser chamada a cada interrupção do Timer1
}

// -----------------------------------------------------------------------------

// Main loop, read and display data
void loop()
{ 
  // Quantidade de dados é referente a quanta informação de um unico elemento será armazenado
  ui8_size_data = uint8_t( ui8_Time_transmition*((1/(ui16_Time_aquisition/1000.0))));
  
  //ui8_packet_length referente ao cabeçalho (até o momento apenas 1 byte)
  //mais a quantidade de elementos (até o momento 3 - acc, gyro, temp)
  ui8_packet_length = uint8_t(3.0*ui8_size_data + DEF_SIZE_HEADER);

  //se houver interrupção por tempo, ativa o flag e fica disponivel a aquisição de dados
  if (flag_acquisition)
  {
    flag_acquisition = false;
    acc_gyro_temp_function(ui8_send_buffer);
    
  }
  check_send();
//  wdt_reset();
}
// -----------------------------------------------------------------------------
