#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X3f, 16, 2); //SCL A5 SDA A4
#define SS 10
#define RST 2
#define DI0 3
#define BAND 433E6
#define buzzer 4
int pos1, pos2, pos3;
String x, y, z, id;
String incoming;
unsigned long lastPacketTime = 0;
void setup() {
  // put your setup code here, to run once:
  //lcd init
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  Serial.println("LoRa Reciever");
  lcd.setCursor(0, 0);
  lcd.print("Lora Reciever");
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    lcd.setCursor(0, 1);
    lcd.print("LoRa failed");
    while (1)
      ;
  } else {
    Serial.println("Starting LoRa completed");
    lcd.setCursor(0, 1);
    lcd.print("LoRa completed");
  }
  LoRa.setSpreadingFactor(10); // Đặt tỷ lệ phổ mở rộng là 7
  LoRa.setSignalBandwidth(125E3); // Đặt băng thông tín hiệu là 125kHz
  delay(3000);
}
void loop() {
  // try to parse packet

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    lcd.clear();
    while (LoRa.available()) {
      incoming = LoRa.readString();
      Serial.println(incoming);
      pos1 = incoming.indexOf('@');
      pos2 = incoming.indexOf('$');
      pos3 = incoming.indexOf('*');

      id = incoming.substring(0, pos1);
      x = incoming.substring(pos1 + 1, pos2);
      y = incoming.substring(pos2 + 1, pos3);
      z = incoming.substring(pos3 + 1, incoming.length());

      if (id == "B2016887")
      {
        lcd.setCursor(0, 0);
        lcd.print("X:");
        lcd.setCursor(9, 0);
        lcd.print("Y:");
        lcd.setCursor(0, 1);
        lcd.print("Z:");

        lcd.setCursor(2, 0);
        lcd.print(x);
        lcd.setCursor(11, 0);
        lcd.print(y);
        lcd.setCursor(2, 1);
        lcd.print(z);

        lcd.setCursor(8, 1);
        lcd.print("RSSI:");
        lcd.setCursor(13, 1);
        lcd.print(LoRa.packetRssi());
        Serial.println(LoRa.packetRssi());
        delay(100);

        float x_val = x.toFloat();
        float y_val = y.toFloat();
        float z_val = z.toFloat();
        if (x_val > 0.15 || x_val < -0.15 || y_val > 0.15 || y_val < -0.15 || z_val < 0.8)
        {
          tone(buzzer, 1200, 500);
        }
        else {
          noTone(buzzer);
        }
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Packet not found");
        delay(350);
      }
    }
    lastPacketTime = millis();
  }
  else {
    // Kiểm tra nếu không nhận được gói tin trong 2 giây
    if (millis() - lastPacketTime > 2000) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Node is OFF");
      delay(350);
      lastPacketTime = millis(); // Cập nhật lại thời gian nhận gói tin cuối cùng
    }
  }
}
