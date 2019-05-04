#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "keyboard.h"

bool orgCapsLockPressed = false;
bool orgLeftCtrlPressed = false;

class KbdRptParser : public KeyboardReportParser
{
    // void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
};

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  uint8_t change = before ^ after;
  if (change == 0) {
    return;
  }

  MODIFIERKEYS changeMod;
  *((uint8_t*)&changeMod) = change;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  // Swap
  orgLeftCtrlPressed = afterMod.bmLeftCtrl;
  afterMod.bmLeftCtrl = orgCapsLockPressed;

  if (changeMod.bmLeftCtrl) {
    // LeftCtrl (remapping対象)がDown or Upの場合

    if (orgLeftCtrlPressed) {
      // Down
      keyboard::reportPress(*(uint8_t*)&afterMod, 0x39);
    } else {
      // Up
      keyboard::reportRelease(*(uint8_t*)&afterMod, 0x39);
    }
  } else {
    keyboard::reportModifier(*(uint8_t*)&afterMod);
  }
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  if (key == 0x39) {
    orgCapsLockPressed = true;  // 記録
    key = 0;  // オリジナルを無効化
  } else if (orgCapsLockPressed) {
    MODIFIERKEYS leftCtrl;
    leftCtrl.bmLeftCtrl = 1;
    mod |= *(uint8_t*)&leftCtrl;
  }

  keyboard::reportPress(mod, key);
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  if (key == 0x39) {
    orgCapsLockPressed = false;
    key = 0;  // 無効化
  } else if (orgCapsLockPressed) {
    MODIFIERKEYS leftCtrl;
    leftCtrl.bmLeftCtrl = 1;
    mod |= *(uint8_t*)&leftCtrl;
  }

  keyboard::reportRelease(mod, key);
}

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  keyboard::initKeyboard();

  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);
}

void loop()
{
  Usb.Task();
}
