#include <USBComposite.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //v1.7.5
#include <Adafruit_SSD1306.h>
#include <JC_Button.h> // https://github.com/JChristensen/JC_Button v2.1.2
#include "bmp.h"
#include "hardware.h"

USBHID HID;
HIDKeyboard Keyboard(HID);
HIDConsumer Consumer(HID);
USBCompositeSerial CompositeSerial;

unsigned int cursor = 0;

void fVolumeUp(void);
void fVolumeDown(void);
void fMute(void);
void fMuteMic(void);
void fLock(void);
void fCut(void);
void fPaste(void);
void fForward(void);
void fBackward(void);
void fPlay(void);
void fTerm(void);
void fCalc(void);

boolean defaultLayoutActive = true;

typedef enum eKeyName{
	key01 = 1,
	key02 = 2,
	key03 = 3,
	key04 = 4,
	key05 = 5,
	key06 = 6,
	key07 = 7,
	key08 = 8,
	key09 = 9,
	key10 = 10,
	key11 = 11,
	key12 = 12
} eKeyName;

typedef enum eFunction{
	MUTE,
	PLAY,
	FORWARD,
	BACKWARD,
	CUT,
	PASTE,
	VOL_UP,
	VOL_DOWN,
	MIC_MUTE,
	LOCK,
	TERM,
	CALC
} eFunction;

typedef struct tFunction {
	eFunction Function;
  void (*Call) (void);
  const unsigned char *Icon ;
} tFunction;

tFunction functionsArray[] = {
  {MUTE,      fMute,        bmp_mute},
  {PLAY,      fPlay,        bmp_play},
  {FORWARD,   fForward,     bmp_forward},
  {BACKWARD,  fBackward,    bmp_backward},
  {CUT,       fCut,         bmp_cut},
  {PASTE,     fPaste,       bmp_paste},
  {VOL_UP,    fVolumeUp,    bmp_volume_up},
  {VOL_DOWN,  fVolumeDown,  bmp_volume_down},
  {MIC_MUTE,  fMuteMic,     bmp_microphone},
  {LOCK,      fLock,        bmp_lock},
  {TERM,      fTerm,        bmp_term},
  {CALC,      fCalc,        bmp_calc2}
};

typedef struct tkey {
	eKeyName Name;
  Button* Key;
  Adafruit_SSD1306* OLED;
	eFunction Function;
} tkey;

/* KeyMap of the Keybon12 device

  Key01 | Key02 | Key03
  Key04 | Key05 | Key06
  Key01 | Key02 | Key09
  Key10 | Key11 | Key12

*/
tkey keyArray[] = {
  {key01, &button01, &OLED01, LOCK},      {key02, &button02, &OLED02, CALC}, {key03, &button03, &OLED03, MIC_MUTE},
	{key04, &button04, &OLED04, CUT}, 			{key05, &button05, &OLED05, PASTE},     {key06, &button06, &OLED06, TERM},
	{key07, &button07, &OLED07, VOL_DOWN},	{key08, &button08, &OLED08, MUTE},	   {key09, &button09, &OLED09, VOL_UP},
	{key10, &button10, &OLED10, BACKWARD},	{key11, &button11, &OLED11, PLAY},	   {key12, &button12, &OLED12, FORWARD}
};

void setup() {
  pinMode(SW01, INPUT_PULLUP);
  pinMode(SW02, INPUT_PULLUP);
  pinMode(SW03, INPUT_PULLUP);
  pinMode(SW04, INPUT_PULLUP);
  pinMode(SW05, INPUT_PULLUP);
  pinMode(SW06, INPUT_PULLUP);
  pinMode(SW07, INPUT_PULLUP);
  pinMode(SW08, INPUT_PULLUP);
  pinMode(SW09, INPUT_PULLUP);
  pinMode(SW10, INPUT_PULLUP);
  pinMode(SW11, INPUT_PULLUP);
  pinMode(SW12, INPUT_PULLUP);

  for (uint8_t i = 0; i < 12; i++) {
    keyArray[i].Key->begin();
  };

  USBComposite.setProductId(PRODUCT_ID);
  HID.registerComponent();
  CompositeSerial.registerComponent();
  USBComposite.begin();

  OLED01.begin(SSD1306_SWITCHCAPVCC, 0, true, true); // OLEDs share same reset line, o
  OLED01.setRotation(0);

  for (uint8_t i = 1; i < 12; i++) {
    keyArray[i].OLED->begin(SSD1306_SWITCHCAPVCC, 0, false, true);
    keyArray[i].OLED->setRotation(0);
  };

  displayContrast(LOW);

  //delay(500);
  CompositeSerial.setTimeout(200);

  for (uint8_t i = 0; i < 12; i++) {
    keyArray[i].OLED->clearDisplay();   // clears the screen and buffer
    keyArray[i].OLED->drawBitmap(32, 0, functionsArray[keyArray[i].Function].Icon, 64, 48, WHITE);
  };
}


void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    if (keyArray[i].Key->wasPressed() or keyArray[i].Key->pressedFor(300)) {
      if (defaultLayoutActive) {
        functionsArray[keyArray[i].Function].Call();
      }
      else CompositeSerial.print(char(i+0x30)); // 0x30 is the ASCII table offset for character '0'
    }
  };

  while (CompositeSerial.available() > 0) {
    byte command = CompositeSerial.read();
		switch (command) {
			case '0':		// Key1
			case '1':		// Key2, etc...
			case '2':		// they all fall through to the same handling code
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ':':
			case ';':
				defaultLayoutActive = false;
				CompositeSerial.readBytes(bmp_swap, 384);
				keyArray[command-0x30].OLED->clearDisplay();
				keyArray[command-0x30].OLED->drawBitmap(32, 0, bmp_swap, 64, 48, WHITE);
				break;
			case 'A': //Answer to call
	      CompositeSerial.print("a");
				break;
			case 'B': //high brightness
	      displayContrast(HIGH);
				break;
			case 'b': //low brightness
	      displayContrast(LOW);
				break;
			default:
				CompositeSerial.print("Command not recognized");
		}
  }

  for (uint8_t i = 0; i < 12; i++) {
    keyArray[i].OLED->display();
  };

  for (uint8_t i = 0; i < 12; i++) {
    keyArray[i].Key->read();
  };

}

void displayContrast(boolean contrast) {
  byte contrastSetting = 0x35;
  if (contrast == HIGH) contrastSetting = 0x7F;

  for (uint8_t i = 0; i < 12; i++) {
    keyArray[i].OLED->ssd1306_command(SSD1306_SETCONTRAST);
    keyArray[i].OLED->ssd1306_command(contrastSetting);
  };
}


// See https://github.com/arpruss/USBComposite_stm32f1/blob/master/USBHID.h for actual key presses and value list
// For consumer style value, see https://www.usb.org/sites/default/files/hut1_22.pdf, starting p117
void fVolumeUp(void){
  Consumer.press(HIDConsumer::VOLUME_UP);
  Consumer.release();
}

void fVolumeDown(void){
	Consumer.press(HIDConsumer::VOLUME_DOWN);
  Consumer.release();
}

void fMute(void){
  Consumer.press(HIDConsumer::MUTE);
  Consumer.release();
}

void fLock(void){
	// From https://www.usb.org/sites/default/files/hut1_22.pdf, starting p122
	// AL Terminal Lock/Screensaver
	Consumer.press(0x19E);
  Consumer.release();
}

void fCalc(void){
	Keyboard.press(KEY_LEFT_GUI);
	Keyboard.press('k');
	Keyboard.release('k');
	Keyboard.release(KEY_LEFT_GUI);
}

void fMuteMic(void){
//	Keyboard.press((uint16_t)(0x6F + 0x88)); // Keyboard F20
	Keyboard.press((uint16_t)(0xB0 + 0x88)); // Keyboard F20
	Keyboard.releaseAll();
}

void fCut(void){
	Keyboard.press(KEY_LEFT_CTRL);
	Keyboard.press('x');
	Keyboard.releaseAll();
	// Keyboard.release(KEY_LEFT_CTRL);

	// From https://www.usb.org/sites/default/files/hut1_22.pdf, starting p123
	// AC Cut
	// Consumer.press(0x21C);
	// Consumer.release();
}

void fPaste(void){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('v');
	Keyboard.releaseAll();
  // Keyboard.release(KEY_LEFT_CTRL);

	// From https://www.usb.org/sites/default/files/hut1_22.pdf, starting p123
	// AC Paste
	// Consumer.press(0x21D);
	// Consumer.release();
}

void fForward(void){
  Consumer.press(182);
  Consumer.release();
}

void fBackward(void){
  Consumer.press(181);
  Consumer.release();
}

void fPlay(void){
  Consumer.press(HIDConsumer::PLAY_OR_PAUSE);
  Consumer.release();
}

void fTerm(void){
  Keyboard.press(KEY_F12);
  Keyboard.release(KEY_F12);
}
