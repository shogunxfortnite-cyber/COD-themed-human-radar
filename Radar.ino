#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite img = TFT_eSprite(&tft); // Sprite for double-buffering (prevents flicker)

// Radar animation variables
int centerX = 120; // Center of the 240x320 screen (Width-wise)
int centerY = 160; // Center of the 240x320 screen (Height-wise)
int radarRadius = 110;
int sweepAngle = 0;

// Hardware Serial 2 for LD2410 Radar Sensor
#define RXD2 16
#define TXD2 17

// Target tracking variables from LD2410
bool targetDetected = false;
int targetDistanceCm = 0;
int blipX = 0;
int blipY = 0;
int blipFade = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(256000, SERIAL_8N1, RXD2, TXD2); // LD2410 default baud rate is 256000

  tft.init();
  tft.setRotation(2); // Portrait mode (240x320)
  tft.fillScreen(TFT_BLACK);

  // Create a full-screen frame buffer sprite using ESP32 internal RAM
  img.createSprite(240, 320);
}

void loop() {
  readRadarSensor();

  // 1. Clear buffer with pure black
  img.fillSprite(TFT_BLACK);

  // 2. Draw CoD Radar Grid Lines (Dark Green - RGB565: 0x03E0 or dark custom)
  uint16_t darkGreen = img.color565(0, 80, 0);
  uint16_t neonGreen = img.color565(0, 255, 0);
  
  img.drawCircle(centerX, centerY, radarRadius, darkGreen);
  img.drawCircle(centerX, centerY, radarRadius / 2, darkGreen);
  img.drawCircle(centerX, centerY, radarRadius / 4, darkGreen);
  img.drawFastHLine(centerX - radarRadius, centerY, radarRadius * 2, darkGreen);
  img.drawFastVLine(centerX, centerY - radarRadius, radarRadius * 2, darkGreen);

  // 3. Calculate sweep line position
  float rad = radians(sweepAngle);
  int lineX = centerX + radarRadius * cos(rad);
  int lineY = centerY + radarRadius * sin(rad);

  // Draw fading trail effect (CoD style)
  for (int i = 0; i < 15; i++) {
    float trailRad = radians(sweepAngle - i);
    int trailX = centerX + radarRadius * cos(trailRad);
    int trailY = centerY + radarRadius * sin(trailRad);
    uint16_t trailColor = img.color565(0, 255 - (i * 16), 0);
    img.drawLine(centerX, centerY, trailX, trailY, trailColor);
  }

  // Draw main cutting sweep line
  img.drawLine(centerX, centerY, lineX, lineY, neonGreen);

  // 4. Handle Target Blip Rendering
  if (targetDetected && targetDistanceCm > 0) {
    // Map target distance into screen pixel boundaries
    int pixelDist = map(targetDistanceCm, 0, 600, 0, radarRadius); // Max sensor range ~6 meters
    
    // Smooth angle lock approximation for demo placement
    float targetRad = radians(sweepAngle - 20); 
    blipX = centerX + pixelDist * cos(targetRad);
    blipY = centerY + pixelDist * sin(targetRad);
    blipFade = 255; 
  }

  // Draw and fade the blip over time
  if (blipFade > 0) {
    uint16_t blipColor = img.color565(0, blipFade, 0);
    img.fillCircle(blipX, blipY, 5, blipColor);       // Inner bright core
    img.drawCircle(blipX, blipY, 8, blipColor);       // Radar echo ring
    blipFade -= 3; // Fade out slowly frame-by-frame
  }

  // 5. Push the completed frame buffer to the real screen instantly
  img.pushSprite(0, 0);

  // Advance sweep line angle
  sweepAngle = (sweepAngle + 3) % 360;
  delay(15); 
}

void readRadarSensor() {
  if (Serial2.available() >= 4) {
    // Basic structural parse check for HLK-LD2410 frame headers
    if (Serial2.read() == 0xF4 && Serial2.read() == 0xF3 && Serial2.read() == 0xF2 && Serial2.read() == 0xF1) {
      // Fast forward byte offset to target extraction indexes
      for (int i = 0; i < 4; i++) Serial2.read(); 
      
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
}
