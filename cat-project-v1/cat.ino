#include <SPI.h>
#include <TFT_eSPI.h>
#include "AnimatedGIF.h"
#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//-------------------------
// LCD setup
//-------------------------
TFT_eSPI tft = TFT_eSPI();

//-------------------------
// DFPlayer setup
//-------------------------
HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;

//-------------------------
// GIF animation setup
//-------------------------
AnimatedGIF gif;

// Include header-based GIF
#include "cat.h"   // make sure cat.h is available in your project

//-------------------------
// DFPlayer pins & constants
//-------------------------
#define RX_PIN 26  // ESP32 receives from DFPlayer TX
#define TX_PIN 27  // ESP32 transmits to DFPlayer RX

#define ESP32_STARTUP_DELAY 1000    
#define DFPLAYER_POWER_DELAY 1000   
#define DFPLAYER_INIT_DELAY 1000    

bool dfPlayerReady = false;
int currentTrack = 1;
int currentVolume = 20;

//-------------------------
// Enhanced GIF Draw callback with better performance
//-------------------------
void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s;
  uint16_t *usPalette;
  static uint16_t lineBuffer[320];  // Static buffer for better performance

  int iWidth = pDraw->iWidth;
  if (iWidth + pDraw->iX > tft.width()) iWidth = tft.width() - pDraw->iX;
  if (iWidth <= 0) return;

  usPalette = pDraw->pPalette;
  s = pDraw->pPixels;
  int y = pDraw->iY + pDraw->y;

  // Bounds check
  if (y < 0 || y >= tft.height()) return;

  if (pDraw->ucHasTransparency) {
    // Optimized transparency handling
    uint8_t transparent = pDraw->ucTransparent;
    for (int x = 0; x < iWidth; x++) {
      uint8_t pixel = s[x];
      if (pixel != transparent) {
        tft.drawPixel(pDraw->iX + x, y, usPalette[pixel]);
      }
    }
  } else {
    // Fast non-transparent line drawing
    for (int x = 0; x < iWidth; x++) {
      lineBuffer[x] = usPalette[s[x]];
    }
    // Use DMA-accelerated pushImage for better performance
    tft.pushImage(pDraw->iX, y, iWidth, 1, lineBuffer);
  }
}

//-------------------------
// Setup
//-------------------------
void setup() {
  Serial.begin(115200);

  // Initialize TFT with optimal settings
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  Serial.println("LCD initialized");

  // Initialize GIF
  gif.begin(LITTLE_ENDIAN_PIXELS);
  if (gif.open((uint8_t *)cat, sizeof(cat), GIFDraw)) {
    Serial.printf("Opened cat.gif (%dx%d)\n", gif.getCanvasWidth(), gif.getCanvasHeight());
  } else {
    Serial.println("Failed to open cat.gif!");
  }

  // Delay for stability
  delay(ESP32_STARTUP_DELAY);

  // Initialize DFPlayer
  dfPlayerReady = initializeDFPlayer();
}

void loop() {    
  int result = gif.playFrame(true, NULL);
  if (result == 0) {
    gif.reset(); // Loop the animation
  } else if (result < 0) {
    Serial.printf("GIF error: %d\n", result);
  }
}

//-------------------------
// DFPlayer Initialization
//-------------------------
bool initializeDFPlayer() {
  Serial.println("Powering ON DFPlayer Mini...");  
  delay(DFPLAYER_POWER_DELAY);

  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(DFPLAYER_INIT_DELAY);

  for (int attempts = 0; attempts < 5; attempts++) {
    if (myDFPlayer.begin(mySerial)) {
      Serial.println("DFPlayer Mini ready!");
      myDFPlayer.volume(currentVolume);
      myDFPlayer.play(currentTrack);
      return true;
    }
    Serial.println("DFPlayer not responding, retrying...");
    delay(1000);
  }
  Serial.println("DFPlayer initialization failed");
  return false;
}
