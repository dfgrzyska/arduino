#include <SPI.h>
#include <TFT_eSPI.h>
#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// LCD setup
TFT_eSPI tft = TFT_eSPI();

// DFPlayer setup
HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;

// Define DFPlayer control pins
#define RX_PIN 26  // ESP32 receives from DFPlayer TX
#define TX_PIN 27  // ESP32 transmits to DFPlayer RX

// Timing constants
#define ESP32_STARTUP_DELAY 1000    
#define DFPLAYER_POWER_DELAY 1000   
#define DFPLAYER_INIT_DELAY 1000    

// Global variables
bool dfPlayerReady = false;
int currentTrack = 1;
int currentVolume = 20;

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD first
  Serial.println("Initializing LCD...");
  initializeLCD();
  
  // Display startup message on LCD
  displayStartupScreen();
  
  Serial.println("ESP32 started. DFPlayer power OFF.");
  Serial.println("Waiting for ESP32 to stabilize...");
  
  delay(ESP32_STARTUP_DELAY);
  
  // Initialize DFPlayer
  dfPlayerReady = initializeDFPlayer();
  
  // Update LCD with final status
  displayMainScreen();
  
  Serial.println("Setup complete. Commands: p=play, s=stop, n=next, b=prev, +=vol up, -=vol down");
}

void initializeLCD() {
  // Manual reset for better reliability
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); delay(100);
  digitalWrite(4, LOW);  delay(200);
  digitalWrite(4, HIGH); delay(100);
  
  tft.init();
  tft.setRotation(0);  // Portrait mode
  tft.fillScreen(TFT_BLACK);
  
  // Test if LCD is working
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.println("LCD Initialized");
  
  Serial.println("LCD initialized successfully");
}

void displayStartupScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("DFPlayer");
  tft.println("  + LCD");
  tft.println("  Setup");
  
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(10, 100);
  tft.println("Initializing...");
}

bool initializeDFPlayer() {
  // Power on DFPlayer
  Serial.println("Powering ON DFPlayer Mini...");  
  delay(DFPLAYER_POWER_DELAY);
  
  Serial.println("Initializing DFPlayer communication...");
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(DFPLAYER_INIT_DELAY);
  
  // Try to initialize with retries
  int attempts = 0;
  const int maxAttempts = 5;
  
  // Update LCD
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(10, 120);
  tft.println("DFPlayer init...");
  
  while (attempts < maxAttempts) {
    Serial.print("DFPlayer init attempt ");
    Serial.print(attempts + 1);
    Serial.print("/");
    Serial.println(maxAttempts);
    
    // Update LCD with attempt number
    tft.fillRect(10, 140, 200, 20, TFT_BLACK);
    tft.setCursor(10, 140);
    tft.print("Attempt: ");
    tft.print(attempts + 1);
    tft.print("/");
    tft.print(maxAttempts);
    
    if (myDFPlayer.begin(mySerial)) {
      Serial.println("DFPlayer Mini ready!");
      
      // Configure DFPlayer
      myDFPlayer.volume(currentVolume);
      delay(100);
      myDFPlayer.play(currentTrack);
      
      Serial.println("DFPlayer configured and playing");
      return true;
    } else {
      attempts++;
      if (attempts < maxAttempts) {
        Serial.println("DFPlayer not responding, retrying...");
        delay(1000);
      }
    }
  }
  
  Serial.println("DFPlayer initialization failed");
  return false;
}

void displayMainScreen() {
  tft.fillScreen(TFT_BLACK);
  
  // Title
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Music Player");
  
  // ASCII Cat
  displayASCIICat();
  
  // Status
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 180);
  
  if (dfPlayerReady) {
    tft.setTextColor(TFT_GREEN);
    tft.println("Status: READY");
    tft.setTextColor(TFT_WHITE);
    tft.print("Track: ");
    tft.println(currentTrack);
    tft.print("Volume: ");
    tft.println(currentVolume);
  } else {
    tft.setTextColor(TFT_RED);
    tft.println("Status: ERROR");
    tft.setTextColor(TFT_WHITE);
    tft.println("Check connections");
  }
  
  // Controls
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(10, 240);
  tft.println("Controls:");
  tft.setTextColor(TFT_YELLOW);
  tft.println("p=play s=stop");
  tft.println("n=next b=prev");
  tft.println("+=vol+ -=vol-");
}

void displayASCIICat() {
  tft.setTextColor(TFT_MAGENTA);
  tft.setTextSize(1);
  
  // ASCII cat - positioned carefully to fit on 240px wide screen
  tft.setCursor(5, 60);
  tft.println(" _._     _,-'\"\"```-._");
  tft.setCursor(5, 75);
  tft.println("(,-.```._,'(       |\\```-/|");
  tft.setCursor(5, 90);
  tft.println("    ```.-' \\ )-```( , o o)");
  tft.setCursor(5, 105);
  tft.println("          ```-    \\```_```\"'-");
}

void updateTrackDisplay() {
  // Clear and update track number
  tft.fillRect(60, 190, 60, 10, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(60, 190);
  tft.println(currentTrack);
}

void updateVolumeDisplay() {
  // Clear and update volume
  tft.fillRect(70, 200, 60, 10, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(70, 200);
  tft.println(currentVolume);
}

void displayMessage(String message, uint16_t color) {
  // Display temporary message at bottom
  tft.fillRect(0, 300, 240, 20, TFT_BLACK);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.setCursor(10, 305);
  tft.println(message);
}

void loop() {
  // Check DFPlayer state
  if (dfPlayerReady && myDFPlayer.available()) {
    int type = myDFPlayer.readType();
    int value = myDFPlayer.read();
    
    switch (type) {
      case DFPlayerPlayFinished:
        Serial.print("Track finished: ");
        Serial.println(value);
        displayMessage("Track finished", TFT_YELLOW);
        // Auto play next track
        currentTrack++;
        if (currentTrack > 99) currentTrack = 1;  // Loop back to track 1
        myDFPlayer.play(currentTrack);
        updateTrackDisplay();
        break;
        
      case DFPlayerError:
        Serial.print("DFPlayer Error: ");
        Serial.println(value);
        displayMessage("Player Error", TFT_RED);
        break;
    }
  }

  // Handle Serial commands
  if (Serial.available()) {
    char cmd = Serial.read();
    
    if (!dfPlayerReady && cmd != 'r') {
      Serial.println("DFPlayer not ready. Use 'r' to retry initialization.");
      displayMessage("Player not ready", TFT_RED);
      return;
    }
    
    switch (cmd) {
      case 'p':  // Play
        myDFPlayer.play();
        Serial.println("Play");
        displayMessage("Playing", TFT_GREEN);
        break;
        
      case 's':  // Stop
        myDFPlayer.stop();
        Serial.println("Stop");
        displayMessage("Stopped", TFT_ORANGE);
        break;
        
      case 'n':  // Next
        currentTrack++;
        if (currentTrack > 99) currentTrack = 1;
        myDFPlayer.play(currentTrack);
        Serial.print("Next track: ");
        Serial.println(currentTrack);
        updateTrackDisplay();
        displayMessage("Next track", TFT_CYAN);
        break;
        
      case 'b':  // Previous
        currentTrack--;
        if (currentTrack < 1) currentTrack = 99;
        myDFPlayer.play(currentTrack);
        Serial.print("Previous track: ");
        Serial.println(currentTrack);
        updateTrackDisplay();
        displayMessage("Previous track", TFT_CYAN);
        break;
        
      case '+':  // Volume up
        if (currentVolume < 30) {
          currentVolume++;
          myDFPlayer.volume(currentVolume);
          Serial.print("Volume: ");
          Serial.println(currentVolume);
          updateVolumeDisplay();
          displayMessage("Volume up", TFT_GREEN);
        }
        break;
        
      case '-':  // Volume down
        if (currentVolume > 0) {
          currentVolume--;
          myDFPlayer.volume(currentVolume);
          Serial.print("Volume: ");
          Serial.println(currentVolume);
          updateVolumeDisplay();
          displayMessage("Volume down", TFT_GREEN);
        }
        break;
        
      case 'r':  // Restart/Retry DFPlayer initialization
        Serial.println("Retrying DFPlayer initialization...");
        displayMessage("Reinitializing...", TFT_YELLOW);
        dfPlayerReady = initializeDFPlayer();
        displayMainScreen();
        break;
    }
  }
}