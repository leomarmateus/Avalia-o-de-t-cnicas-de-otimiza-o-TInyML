/*******************************************************************************
* Defines Header
*******************************************************************************/

#ifndef DEFINES_HEADER
#define DEFINES_HEADER

// -----------------------------------------------------------------------------


// Versões
//#define versao_0
#define versao_1
//
//#ifdef versao_1 && versao_0
//  #error "Mais de uma versão definidas"
//#endif

#ifdef versao_1
  #define MPU9250_ADDRESS 0x69//ADO em VCC AD0=1
  #define LED_ERROR_1 3
  #define LED_ERROR_2 4
#else
  #define AD0 4
  #define MPU9250_ADDRESS 0x69//ADO em VCC AD0=1
#endif

// General
byte    CFG_START[3] =  {0x63, 0x66, 0x67};

// Return Codes
#define RC_OK           0x55    // OK
#define RC_NOK          0x56    // General Error

// Sizes
#define CFG_SIZE        11      // cfg(10) + checksum(1)
#define DEF_SIZE_HEADER 12      //define tamanho cabeçalho

// Pins
#define ANALOG_BAT_PIN  A0
#define GET_BAT_PIN     A1

// Definições para configuração do MPU9250
#define GYRO_FULL_SCALE_250_DPS  0x00 
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
 
#define ACC_FULL_SCALE_2_G  0x00 
#define ACC_FULL_SCALE_4_G  0x08
#define ACC_FULL_SCALE_8_G  0x10
#define ACC_FULL_SCALE_16_G 0x18



// -----------------------------------------------------------------------------

// header guard
#endif
