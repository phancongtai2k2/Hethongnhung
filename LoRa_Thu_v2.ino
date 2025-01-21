#include <LoRa.h>
#include <SPI.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 2

ThreeWire myWire(26,27,25); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

unsigned int countPackage = 1;

String convert(const RtcDateTime& dt) {
  String timeString = 
    (dt.Hour() < 10 ? "0" : "") + String(dt.Hour()) + ":" + 
    (dt.Minute() < 10 ? "0" : "") + String(dt.Minute()) + ":" + 
    (dt.Second() < 10 ? "0" : "") + String(dt.Second()); 
  return timeString;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  if (!LoRa.begin(433E6)) {  // Tần số 433 MHz
    Serial.println("Khoi tao that bai");
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(500E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(12);
  LoRa.setSyncWord(0x34);
  LoRa.enableCrc();
  LoRa.setOCP(100);
}

void loop() {
  int packetSize = LoRa.parsePacket();

  int rssi = LoRa.packetRssi();
  float snr = LoRa.packetSnr();
  long freqError = LoRa.packetFrequencyError();
  
  if (packetSize){
    while (LoRa.available()){

      String received = LoRa.readString();

      RtcDateTime now = Rtc.GetDateTime();

      String time = convert(now);

      Serial.print(countPackage);
      Serial.print(",");
      Serial.print(time);
      Serial.print(",");
      Serial.print(received);
      Serial.print(",");
      
      Serial.print(rssi);
      Serial.print(",");
      Serial.print(snr);
      Serial.print(",");
      Serial.print(freqError);
      Serial.println("");
    }
    countPackage++;
  }
  else{
    Serial.println("E");
  }
}
