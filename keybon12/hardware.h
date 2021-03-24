#define PRODUCT_ID 0x29

#define SW01 PA3
#define SW02 PB0
#define SW03 PB12
#define SW04 PA15
#define SW05 PB11
#define SW06 PA9
#define SW07 PA14
#define SW08 PB10
#define SW09 PA8
#define SW10 PB6
#define SW11 PC14
#define SW12 PB8

#define OLED_MOSI    PA7
#define OLED_CLK     PA5
#define OLED_DC      PA2
#define OLED_CS01    PB5
#define OLED_CS02    PA4
#define OLED_CS03    PB13
#define OLED_CS04    PB4
#define OLED_CS05    PA6
#define OLED_CS06    PB14
#define OLED_CS07    PB3
#define OLED_CS08    PB1
#define OLED_CS09    PB15
#define OLED_CS10    PB7
#define OLED_CS11    PC15
#define OLED_CS12    PB9
#define OLED_RESET   PA1

Adafruit_SSD1306 OLED01(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS01);
Adafruit_SSD1306 OLED02(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS02);
Adafruit_SSD1306 OLED03(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS03);
Adafruit_SSD1306 OLED04(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS04);
Adafruit_SSD1306 OLED05(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS05);
Adafruit_SSD1306 OLED06(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS06);
Adafruit_SSD1306 OLED07(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS07);
Adafruit_SSD1306 OLED08(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS08);
Adafruit_SSD1306 OLED09(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS09);
Adafruit_SSD1306 OLED10(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS10);
Adafruit_SSD1306 OLED11(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS11);
Adafruit_SSD1306 OLED12(128, 48, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS12);

Button button01(SW01, 5, true, true); //pin, debounce, pullup, invert
Button button02(SW02, 5, true, true); //pin, debounce, pullup, invert
Button button03(SW03, 5, true, true); //pin, debounce, pullup, invert
Button button04(SW04, 5, true, true); //pin, debounce, pullup, invert
Button button05(SW05, 5, true, true); //pin, debounce, pullup, invert
Button button06(SW06, 5, true, true); //pin, debounce, pullup, invert
Button button07(SW07, 5, true, true); //pin, debounce, pullup, invert
Button button08(SW08, 5, true, true); //pin, debounce, pullup, invert
Button button09(SW09, 5, true, true); //pin, debounce, pullup, invert
Button button10(SW10, 5, true, true); //pin, debounce, pullup, invert
Button button11(SW11, 5, true, true); //pin, debounce, pullup, invert
Button button12(SW12, 5, true, true); //pin, debounce, pullup, invert
