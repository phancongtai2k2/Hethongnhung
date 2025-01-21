#include <LoRa.h>
#include <SPI.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

#define LORA_SS 5       // Chân SS (chọn GPIO phù hợp)
#define LORA_RST 14     // Chân RST
#define LORA_DIO0 2     // Chân DIO0

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

  // Khởi tạo LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Khoi tao LoRa that bai!");
    while (1);
  }
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(500E3);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(12);
  LoRa.setSyncWord(0x34);
  LoRa.enableCrc();
  LoRa.setOCP(100);

}

void loop() {

  RtcDateTime now = Rtc.GetDateTime();

  String time = convert(now);

  LoRa.beginPacket();
  LoRa.print(time);
  LoRa.print(',');
  LoRa.print(String(countPackage));
  LoRa.endPacket();

  Serial.print(time);
  Serial.print(',');
  Serial.println(String(countPackage));

  countPackage++;
  delay(2000);

}
