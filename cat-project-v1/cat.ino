#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use UART1 on ESP32 (we can remap pins)
HardwareSerial mySerial(1);
DFRobotDFPlayerMini myDFPlayer;

// Define DFPlayer control pins
#define RX_PIN 26  // ESP32 receives from DFPlayer TX
#define TX_PIN 27  // ESP32 transmits to DFPlayer RX

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing DFPlayer...");

  // Start UART for DFPlayer
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer not detected. Check wiring and SD card.");
    while (true) {
      delay(1000);
    }
  }
  
  Serial.println("DFPlayer Mini ready.");
  
  // Initial settings
  myDFPlayer.volume(20);  // Volume 0-30
  Serial.println("Volume set to 20.");
  
  // Play the first MP3 file on SD card
  myDFPlayer.play(1);
  Serial.println("Playing track 1.");
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

  // Example: control via Serial Monitor commands
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'p') {  // Play
      myDFPlayer.play();
      Serial.println("Play");
    } else if (cmd == 's') {  // Stop
      myDFPlayer.stop();
      Serial.println("Stop");
    } else if (cmd == 'n') {  // Next
      myDFPlayer.next();
      Serial.println("Next track");
    } else if (cmd == 'b') {  // Previous
      myDFPlayer.previous();
      Serial.println("Previous track");
    } else if (cmd == '+') {  // Volume up
      int vol = myDFPlayer.readVolume();
      if (vol < 30) vol++;
      myDFPlayer.volume(vol);
      Serial.print("Volume: ");
      Serial.println(vol);
    } else if (cmd == '-') {  // Volume down
      int vol = myDFPlayer.readVolume();
      if (vol > 0) vol--;
      myDFPlayer.volume(vol);
      Serial.print("Volume: ");
      Serial.println(vol);
    }
  }
}
