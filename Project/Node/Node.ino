#include <LoRa.h>
#include <Wire.h>

#define SS 10
#define RST 2
#define DI0 3
#define BAND 433E6

String id = "B2016887";
int ADXL345 = 0x53;
float x_out, y_out, z_out;
float roll = 0,pitch = 0, pitchF = 0 ,rollF = 0;
String msg;
void setup() {
  // put your setup code here, to run once:
  //Lora init
  Serial.begin(115200);
  Serial.println("LoRa Sender");
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  } else {
    Serial.println("Starting LoRa completed");
  }
  
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);

  Wire.write(8);
  Wire.endTransmission();
  delay(10);

  LoRa.setTxPower(15); // Tăng công suất truyền lên 20dBm
  LoRa.setSpreadingFactor(10); // Đặt tỷ lệ phổ mở rộng là 7
  LoRa.setSignalBandwidth(125E3); // Đặt băng thông tín hiệu là 125kHz
}
void loop() {
  //Doc gia tri cam bien
  readADXL345();
  //Xu li LoRa
  msg = id + '@' + (String)x_out + '$' + (String)y_out + '*' + (String)z_out;
  sendMsg(msg);
  delay(100);

//  roll = atan(y_out / sqrt(pow(x_out, 2) + pow(z_out, 2))) * 180 / PI;
//  pitch = atan(-1 * x_out / sqrt(pow(y_out, 2) + pow(z_out, 2))) * 180 / PI;
//  rollF = 0.94 * rollF + 0.06 * roll;
//  pitchF = 0.94 * pitchF + 0.06 * pitch;
//  Serial.print(rollF);
//  Serial.print("/");
//  Serial.println(pitchF);
}
void readADXL345()
{
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); //read 6 registers, each axits 2 registers
  x_out = (Wire.read() | Wire.read() << 8);
  x_out = x_out / 256;
  y_out = (Wire.read() | Wire.read() << 8);
  y_out = y_out / 256;
  z_out = (Wire.read() | Wire.read() << 8);
  z_out = z_out / 256;
  Serial.print("X: "); Serial.print(x_out);
  Serial.print("  Y: "); Serial.print(y_out);
  Serial.print("  Z: "); Serial.println(z_out);
}
void sendMsg(String msg)
{
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
  Serial.println(msg);
}
