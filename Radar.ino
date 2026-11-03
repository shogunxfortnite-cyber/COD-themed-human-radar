#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

int centerX = 120;
int centerY = 160;
int radarRadius = 110;
int sweepAngle = 0;

#define RXD2 16
#define TXD2 17

bool targetDetected = false;
int targetDistanceCm = 0;
int blipX = 0;
int blipY = 0;
int blipFade = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(256000, SERIAL_8N1, RXD2, TXD2);

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  img.createSprite(240, 320);
}

void loop() {
  if (Serial2.available() >= 4) {
    if (Serial2.read() == 0xF4 && Serial2.read() == 0xF3 && Serial2.read() == 0xF2 && Serial2.read() == 0xF1) {
      for (int i = 0; i < 4; i++) {
        Serial2.read();
      }
      
      int statusByte = Serial2.read();
      if (statusByte == 0x01 || statusByte == 0x02 || statusByte == 0x03) {
        targetDetected = true;
        int lowDist = Serial2.read();
        int highDist = Serial2.read();
        targetDistanceCm = (highDist << 8) | lowDist;
      } else {
        targetDetected = false;
      }
    }
  }

  img.fillSprite(TFT_BLACK);

  uint16_t darkGreen = img.color565(0, 80, 0);
  uint16_t neonGreen = img.color565(0, 255, 0);
  
  img.drawCircle(centerX, centerY, radarRadius, darkGreen);
  img.drawCircle(centerX, centerY, radarRadius / 2, darkGreen);
  img.drawCircle(centerX, centerY, radarRadius / 4, darkGreen);
  img.drawFastHLine(centerX - radarRadius, centerY, radarRadius * 2, darkGreen);
  img.drawFastVLine(centerX, centerY - radarRadius, radarRadius * 2, darkGreen);

  float rad = radians(sweepAngle);
  int lineX = centerX + radarRadius * cos(rad);
  int lineY = centerY + radarRadius * sin(rad);

  for (int i = 0; i < 15; i++) {
    float trailRad = radians(sweepAngle - i);
    int trailX = centerX + radarRadius * cos(trailRad);
    int trailY = centerY + radarRadius * sin(trailRad);
    uint16_t trailColor = img.color565(0, 255 - (i * 16), 0);
    img.drawLine(centerX, centerY, trailX, trailY, trailColor);
  }

  img.drawLine(centerX, centerY, lineX, lineY, neonGreen);

  if (targetDetected && targetDistanceCm > 0) {
    int pixelDist = map(targetDistanceCm, 0, 600, 0, radarRadius);
    float targetRad = radians(sweepAngle - 20); 
    blipX = centerX + pixelDist * cos(targetRad);
    by = centerY + pixelDist * sin(targetRad);
    blipFade = 255; 
  }

  if (blipFade > 0) {
    uint16_t blipColor = img.color565(0, blipFade, 0);
    img.fillCircle(blipX, blipY, 5, blipColor);
    img.drawCircle(blipX, blipY, 8, blipColor);
    blipFade -= 3;
  }

  img.pushSprite(0, 0);

  sweepAngle = (sweepAngle + 3) % 360;
  delay(15); 
}
