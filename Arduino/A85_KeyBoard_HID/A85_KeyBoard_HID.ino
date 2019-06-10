#include <Arduino.h>
#include <TrinketHidCombo.h>
#include <A85_PCF8574.h>
// SDA - P0 - 0 // CLK - P2 - 2

#define PCF8574_ADDRESS (0x20)
A85_PCF8574 pcf20;

unsigned int buttonMillis;

void fnSendKeyStroke(uint8_t data_in){
  TrinketHidCombo.poll();
  // https://github.com/adafruit/Adafruit-Trinket-USB/blob/master/TrinketKeyboard/TrinketKeyboard.h
  switch(data_in){
    case 0:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_C);
     break;
    case 1:
     TrinketHidCombo.pressKey(0, KEYCODE_HOME);
     break;
    case 2:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI | KEYCODE_MOD_RIGHT_SHIFT, KEYCODE_S);
     break;
    case 3:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI, KEYCODE_D);
     delay(20);
     TrinketHidCombo.pressKey(0, 0);
     TrinketHidCombo.pressMultimediaKey(MMKEY_MUTE);
     break;
    case 4:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_V);
     break;
    case 5:
     TrinketHidCombo.pressKey(0, KEYCODE_END);
     break;
    case 6:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI, KEYCODE_L);
     break;
    case 7:
     TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_CONTROL | KEYCODE_MOD_RIGHT_ALT, KEYCODE_DELETE);
     break;
  }
  TrinketHidCombo.pressKey(0, 0);
  TrinketHidCombo.poll();
}

void setup() {
  pcf20.begin(PCF8574_ADDRESS);
  TrinketHidCombo.begin();
}

void loop() {
  unsigned int currentMillis = millis();
  TrinketHidCombo.poll();
  
  //Limit button read to 10 times a second
  //Fast enough for most buttons 
  //but this way you don't have a dimmer output because it's blanked during button read
  //a read takes 460us t 16Mhz Arduino and normal I2C speed.
  if(currentMillis - buttonMillis >= 100){
    buttonMillis = currentMillis;
    pcf20.setByte(B11111111);
    uint8_t inputStates = pcf20.getByte();
    for(uint8_t i = 0; i < 8; i++){
      bool bitval = bitRead(inputStates, i);
      if (bitval) fnSendKeyStroke(i);
    }
  }
}