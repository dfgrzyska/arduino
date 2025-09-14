//                            USER DEFINED SETTINGS
//   Set driver type, fonts to be loaded, pins used and SPI control method etc.

#define USER_SETUP_INFO "User_Setup"

// ##################################################################################
// Section 1. Call up the right driver file and any options for it
// ##################################################################################

// #define ILI9341_DRIVER       // ❌
#define ST7789_DRIVER          // ✅ GMT020-02-8P uses ST7789
//#define ILI9341_DRIVER
//#define ST7735_DRIVER
//#define ST7735_GREENTAB

// For ST7789 ONLY, define the pixel width and height in portrait orientation
//#define TFT_WIDTH  240
//#define TFT_HEIGHT 320

// If colours are inverted (white shows as black) then uncomment one of the next
// 2 lines try both options, one of the options should correct the inversion.
// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

// ##################################################################################
// Section 2. Define the pins that are used to interface with the display here
// ##################################################################################

// For ESP32 Dev board with GMT020-02-8P v1.21
#define TFT_MOSI 23  // SDA
#define TFT_SCLK 18  // SCL  
#define TFT_CS   5   // CS
#define TFT_DC   2   // DC
#define TFT_RST  4   // RST

// Backlight control (optional)
// #define TFT_BL   15            // LED back-light control pin
// #define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

// ##################################################################################
// Section 3. Define the fonts that are to be used here
// ##################################################################################

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// ##################################################################################
// Section 4. Other options
// ##################################################################################

// Define the SPI clock frequency
#define SPI_FREQUENCY  27000000  // 27MHz - safe for most displays

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// Comment out the following #define if "SPI Transactions" do not need to be supported
#define SUPPORT_TRANSACTIONS