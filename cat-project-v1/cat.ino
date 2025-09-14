#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use UART1 on ESP32 (we can remap pins)
HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;

// Define DFPlayer control pins
#define RX_PIN 26  // ESP32 receives from DFPlayer TX
#define TX_PIN 27  // ESP32 transmits to DFPlayer RX

// Timing constants
#define ESP32_STARTUP_DELAY 1000    // Wait for ESP32 to fully initialize
#define DFPLAYER_POWER_DELAY 1000   // Wait after powering DFPlayer before init
#define DFPLAYER_INIT_DELAY 1000    // Wait for DFPlayer to fully initialize

void setup() {
  Serial.begin(115200);
  
  Serial.println("ESP32 started. DFPlayer power OFF.");
  Serial.println("Waiting for ESP32 to stabilize...");
  
  // Wait for ESP32 to fully initialize and stabilize
  delay(ESP32_STARTUP_DELAY);
  
  // Now power on the DFPlayer Mini
  Serial.println("Powering ON DFPlayer Mini...");  
  // Wait for DFPlayer power to stabilize
  delay(DFPLAYER_POWER_DELAY);
  Serial.println("Initializing DFPlayer communication...");
  // Start UART for DFPlayer
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Wait for DFPlayer to fully initialize
  delay(DFPLAYER_INIT_DELAY);
  
  // Try to initialize DFPlayer with retries
  int attempts = 0;
  const int maxAttempts = 5;
  Serial.println("Version: 0.0.1");
  while (attempts < maxAttempts) {
    Serial.print("DFPlayer init attempt ");
    Serial.print(attempts + 1);
    Serial.print("/");
    Serial.println(maxAttempts);
    
    if (myDFPlayer.begin(mySerial)) {
      Serial.println("DFPlayer Mini ready!");
      break;
    } else {
      attempts++;
      if (attempts < maxAttempts) {
        Serial.println("DFPlayer not responding, retrying...");
        delay(1000);
      } else {
        Serial.println("DFPlayer initialization failed after all attempts.");
        Serial.println("Check wiring, SD card, and power connections.");
      }
    }
  }
  
  // If initialization successful, configure DFPlayer
  if (attempts < maxAttempts) {
    // Initial settings
    myDFPlayer.volume(20);  // Volume 0-30
    Serial.println("Volume set to 20.");
    
    delay(100);  // Small delay between commands
    
    // Play the first MP3 file on SD card
    myDFPlayer.play(1);
    Serial.println("Playing track 1.");
  }
  
  Serial.println("Setup complete. Commands: p=play, s=stop, n=next, b=prev, +=vol up, -=vol down, r=restart DFPlayer");
}

void loop() {
  // Check DFPlayer state and print info
  if (myDFPlayer.available()) {
    int type = myDFPlayer.readType();
    int value = myDFPlayer.read();
    
    switch (type) {
      case DFPlayerPlayFinished:
        Serial.print("Track finished: ");
        Serial.println(value);
        break;
      case DFPlayerError:
        Serial.print("DFPlayer Error: ");
        Serial.println(value);
        break;
    }
  }

  // Enhanced Serial Monitor commands
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case 'p':  // Play
        myDFPlayer.play();
        Serial.println("Play");
        break;
        
      case 's':  // Stop
        myDFPlayer.stop();
        Serial.println("Stop");
        break;
        
      case 'n':  // Next
        myDFPlayer.next();
        Serial.println("Next track");
        break;
        
      case 'b':  // Previous
        myDFPlayer.previous();
        Serial.println("Previous track");
        break;
        
      case '+':  // Volume up
        {
          int vol = myDFPlayer.readVolume();
          if (vol < 30) vol++;
          myDFPlayer.volume(vol);
          Serial.print("Volume: ");
          Serial.println(vol);
        }
        break;
        
      case '-':  // Volume down
        {
          int vol = myDFPlayer.readVolume();
          if (vol > 0) vol--;
          myDFPlayer.volume(vol);
          Serial.print("Volume: ");
          Serial.println(vol);
        }
        break;
    }
  }
}