#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("=== UNIWERSALNY TEST GMT020-02-8P ===");
  Serial.println("Ten kod przetestuje różne konfiguracje");
  
  // Test 1: Sprawdź fizyczne połączenia
  Serial.println("\n--- TEST PINÓW ---");
  testPins();
  
  // Test 2: Inicjalizacja z różnymi ustawieniami
  Serial.println("\n--- TEST INICJALIZACJI ---");
  testInitialization();
  
  Serial.println("\n--- ROZPOCZYNAM TESTY STEROWNIKÓW ---");
  Serial.println("Sprawdź ekran przez następne ~30 sekund");
  Serial.println("Jeśli zobaczysz COKOLWIEK na ekranie, zapisz numer testu!");
}

void testPins() {
  // Sprawdź czy piny są prawidłowo podłączone
  pinMode(5, OUTPUT);  // CS
  pinMode(2, OUTPUT);  // DC  
  pinMode(4, OUTPUT);  // RST
  
  Serial.println("CS pin (5) test...");
  digitalWrite(5, HIGH); delay(500); digitalWrite(5, LOW); delay(500);
  
  Serial.println("DC pin (2) test...");  
  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW); delay(500);
  
  Serial.println("RST pin (4) test...");
  digitalWrite(4, LOW); delay(500); digitalWrite(4, HIGH); delay(500);
  
  Serial.println("Piny przetestowane");
}

void testInitialization() {
  Serial.println("Manualny reset LCD...");
  
  // Bardzo dokładny reset
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); delay(100);
  digitalWrite(4, LOW);  delay(200);  // Dłuższy reset
  digitalWrite(4, HIGH); delay(100);
  
  Serial.println("Reset zakończony, czekam 500ms...");
  delay(500);
}

void loop() {
  static int testNumber = 1;
  static unsigned long lastTest = 0;
  
  if (millis() - lastTest > 5000) { // Co 5 sekund nowy test
    lastTest = millis();
    
    Serial.print("\n=== TEST "); Serial.print(testNumber); Serial.println(" ===");
    
    // Reset przed każdym testem
    digitalWrite(4, LOW); delay(100);
    digitalWrite(4, HIGH); delay(100);
    
    switch(testNumber) {
      case 1:
        Serial.println("Test 1: ST7789 + normalna inicjalizacja");
        test_ST7789_normal();
        break;
        
      case 2:  
        Serial.println("Test 2: ST7789 + inwersja ON");
        test_ST7789_inversion_on();
        break;
        
      case 3:
        Serial.println("Test 3: ST7789 + inwersja OFF"); 
        test_ST7789_inversion_off();
        break;
        
      case 4:
        Serial.println("Test 4: ILI9341");
        test_ILI9341();
        break;
        
      case 5:
        Serial.println("Test 5: ST7735 GREENTAB");
        test_ST7735_green();
        break;
        
      case 6:
        Serial.println("Test 6: ST7735 REDTAB");  
        test_ST7735_red();
        break;
        
      case 7:
        Serial.println("Test 7: ST7735 BLACKTAB");
        test_ST7735_black();
        break;
        
      default:
        Serial.println("Wszystkie testy zakończone. Resetuję...");
        testNumber = 0;
        break;
    }
    
    testNumber++;
    
    if(testNumber <= 7) {
      Serial.println("Sprawdź ekran TERAZ! Jeśli coś widzisz - zapisz numer testu!");
    }
  }
}

void test_ST7789_normal() {
  tft.init();
  delay(100);
  
  // Test podstawowych kolorów
  tft.fillScreen(TFT_RED);    delay(500);
  tft.fillScreen(TFT_GREEN);  delay(500); 
  tft.fillScreen(TFT_BLUE);   delay(500);
  tft.fillScreen(TFT_WHITE);  delay(500);
  tft.fillScreen(TFT_BLACK);  delay(500);
  
  // Test tekstu
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 1");
  tft.println("ST7789");
}

void test_ST7789_inversion_on() {
  tft.init();
  tft.invertDisplay(true);  // Inwersja ON
  delay(100);
  
  tft.fillScreen(TFT_RED);    delay(500);
  tft.fillScreen(TFT_GREEN);  delay(500);
  tft.fillScreen(TFT_BLUE);   delay(500);
  tft.fillScreen(TFT_WHITE);  delay(500);
  
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);  
  tft.setCursor(10, 10);
  tft.println("TEST 2");
  tft.println("INV ON");
}

void test_ST7789_inversion_off() {
  tft.init(); 
  tft.invertDisplay(false); // Inwersja OFF
  delay(100);
  
  tft.fillScreen(TFT_YELLOW); delay(500);
  tft.fillScreen(TFT_CYAN);   delay(500);
  tft.fillScreen(TFT_MAGENTA); delay(500);
  
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 3");
  tft.println("INV OFF");
}

void test_ILI9341() {
  // Dla tego testu musisz zmienić User_Setup.h na ILI9341_DRIVER
  tft.init();
  delay(100);
  
  tft.fillScreen(TFT_ORANGE); delay(500);
  tft.fillScreen(TFT_PINK);   delay(500);
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 4");
  tft.println("ILI9341");
}

void test_ST7735_green() {
  // Dla tego testu musisz zmienić User_Setup.h na ST7735_DRIVER + ST7735_GREENTAB
  tft.init();
  delay(100);
  
  tft.fillScreen(TFT_GREEN);  delay(1000);
  tft.fillScreen(TFT_BLACK);  delay(500);
  
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 5");
  tft.println("ST7735");
  tft.println("GREEN");
}

void test_ST7735_red() {
  tft.init();
  delay(100);
  
  tft.fillScreen(TFT_RED);    delay(1000);
  tft.fillScreen(TFT_BLACK);  delay(500);
  
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 6");
  tft.println("ST7735");
  tft.println("RED");
}

void test_ST7735_black() {
  tft.init();
  delay(100);
  
  tft.fillScreen(TFT_WHITE);  delay(1000);
  tft.fillScreen(TFT_BLACK);  delay(500);
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TEST 7");
  tft.println("ST7735");
  tft.println("BLACK");
}