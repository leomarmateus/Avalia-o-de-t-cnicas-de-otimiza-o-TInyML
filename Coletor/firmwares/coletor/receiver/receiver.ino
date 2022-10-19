#include <SPI.h>
#include <LoRa.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <avr/wdt.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  //  Serial.println("LoRa Receiver");
  // override the default CS, reset, and IRQ pins (optional)
  //LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin
  LoRa.setPins(10, 2, 3);// set CS, reset, IRQ pin
//  LoRa.setPins(10, 6, 7);// set CS, reset, IRQ pin placa V0
  
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
//  Serial.println("Starting LoRa!");
  LoRa.setSpreadingFactor(8);
      
//  wdt_enable(WDTO_4S);

}

void loop()
{ 
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize)
  {    
    // wait for a second
    // read packet
    int contador = 0;
    while (LoRa.available())
    {       
    //  Serial.print((char)LoRa.read());
      Serial.write(LoRa.read());
//      Serial.print(";");
    }
    Serial.write(LoRa.packetRssi()+157);//157 definido na biblioteca do lora
//    Serial.println(LoRa.packetRssi()+157);//157 definido na biblioteca do lora
  }  
//  wdt_reset();
}
