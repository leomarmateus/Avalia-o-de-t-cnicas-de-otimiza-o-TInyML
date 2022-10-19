/*
  Copyright (c) 2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "tempo.h"
#include "transmitter.h"

/*NÃO ALTERAR NADA DE LORA BEGIN*/
void loraBegin(){
  printf("LoRa Config...\n");

  if (!LoRa.begin(915E6)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }

  // Spreading Factor
  LoRa.setSpreadingFactor(8);

  // Dump Register
  LoRa.dumpRegisters();

  printf("\nLoRa Config Done!\n");
  sleep_ms(2000);
}

void send_packet_full(uint8_t data[]){

    uint8_t monta_pacote;
    unsigned char ui8_send_buffer[MAX_SIZE_SEND];
    monta_pacote = 0;
    ui8_send_buffer[monta_pacote+0] = 10;  //ID dispositivo
    ui8_send_buffer[monta_pacote+1] = 1;   //Tipo de device
    ui8_send_buffer[monta_pacote+2] = MAX_SIZE_SEND; //size
    ui8_send_buffer[monta_pacote+3] = 0x00; //comando
    ui8_send_buffer[monta_pacote+4] = t.year-2000;
    ui8_send_buffer[monta_pacote+5] = t.month;
    ui8_send_buffer[monta_pacote+6] = t.day;
    ui8_send_buffer[monta_pacote+7] = t.hour;
    ui8_send_buffer[monta_pacote+8] = t.min;
    ui8_send_buffer[monta_pacote+9] = t.sec;

    monta_pacote = 10;
    //printf("Payload: %d -- Space: %d \n", sizeof(*data), sizeof(ui8_send_buffer));
    for(int i = 0; i<50; i++)
    {      
      ui8_send_buffer[i*4+monta_pacote+0] = data[i*4+0];
      ui8_send_buffer[i*4+monta_pacote+1] = data[i*4+1];
      ui8_send_buffer[i*4+monta_pacote+2] = data[i*4+2];
      ui8_send_buffer[i*4+monta_pacote+3] = data[i*4+3];   
    }
    monta_pacote =+ 200;

    LoRa.beginPacket();
    for(int i = 0; i<sizeof(ui8_send_buffer)-2; i++)
      LoRa.write(ui8_send_buffer[i]);
    LoRa.write(checksumCalc(ui8_send_buffer, MAX_SIZE_SEND));
    LoRa.endPacket();
    debug_data_send(ui8_send_buffer);
    memset(ui8_send_buffer,0,sizeof(ui8_send_buffer));
}


void send_packet(uint8_t data[]){
    uint8_t monta_pacote;
    unsigned char ui8_send_buffer[MAX_SIZE_SEND];
    monta_pacote = 0;
    ui8_send_buffer[monta_pacote+0] = 10;  //ID dispositivo
    ui8_send_buffer[monta_pacote+1] = 1;   //Tipo de device
    ui8_send_buffer[monta_pacote+2] = MAX_SIZE_SEND; //size
    ui8_send_buffer[monta_pacote+3] = 0x02; //comando
    ui8_send_buffer[monta_pacote+4] = t.year-2000;
    ui8_send_buffer[monta_pacote+5] = t.month;
    ui8_send_buffer[monta_pacote+6] = t.day;
    ui8_send_buffer[monta_pacote+7] = t.hour;
    ui8_send_buffer[monta_pacote+8] = t.min;
    ui8_send_buffer[monta_pacote+9] = t.sec;

    monta_pacote = 10;
   
    ui8_send_buffer[monta_pacote] = data[monta_pacote];  
    
    monta_pacote =+ 1;

    LoRa.beginPacket();
    for(int i = 0; i<sizeof(ui8_send_buffer)-2; i++)
      LoRa.write(ui8_send_buffer[i]);
    LoRa.write(checksumCalc(ui8_send_buffer, MAX_SIZE_SEND));
    LoRa.endPacket();
    debug_data_send(ui8_send_buffer);
    memset(ui8_send_buffer,0,sizeof(ui8_send_buffer));

}

void send_packet_inference(uint8_t data[]){
  unsigned char ui8_buffer[20];
  
  ui8_buffer[0] = 10;  //ID dispositivo
  ui8_buffer[1] = 1;   //Tipo de device
  ui8_buffer[2] = 20; //size  
  ui8_buffer[3] = 0x01; //comando
  ui8_buffer[4] = t.year-2000;
  ui8_buffer[5] = t.month;
  ui8_buffer[6] = t.day;
  ui8_buffer[7] = t.hour;
  ui8_buffer[8] = t.min;
  ui8_buffer[9] = t.sec;
  ui8_buffer[10] = data[0]; // bearing 1
  ui8_buffer[11] = data[1]; // bearing 2
  ui8_buffer[12] = data[2]; // bearing 3
  ui8_buffer[13] = data[3]; // bearing 4
  ui8_buffer[14] = data[5]; // MSE

  LoRa.beginPacket();
  for(int i = 0; i<20; i++)
      LoRa.write(ui8_buffer[i]);
  // debug_data_send(ui8_send_buffer);
  LoRa.write(checksumCalc(ui8_buffer, 20));
  LoRa.endPacket();
  printf("send");
  memset(ui8_buffer,0,sizeof(ui8_buffer));

  
}

int checksumCalc(uint8_t *data, int bytes) {
  int checksum = 0;
  for(size_t i=0; i<bytes; i++) {
    checksum ^= data[i];
  }
  return checksum;
}


int map_to_send(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float map_to_inference(float x, int index) {
  switch (index+1){
  case 1:
    x = (x - MEAN_BEARING_1)/STD_BEARING_1;
  break;

  case 2:
    x = (x - MEAN_BEARING_2)/STD_BEARING_2;
  break;

  case 3:
    x = (x - MEAN_BEARING_2)/STD_BEARING_2;
  break;

  case 4:
    x = (x - MEAN_BEARING_2)/STD_BEARING_2;
  break;
  }
  return x;
}

void debug_data_send(uint8_t buffer[]){
  for (size_t i = 0; i < 10; i++)
    printf("%d\t",i);
  printf("\n");
  for (size_t j = 0; j < 22; j++)
  {
    for (size_t i = 0; i < 10; i++)
    {        
      printf("%d\t",buffer[j*10+i]);
    }
    printf("\n");
  }    
}


