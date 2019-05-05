#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "keyboard.h"
#include "keymap.h"

#define KEYMAP_SIZE 6
const keymap::KeyMap keymaps[KEYMAP_SIZE] =
{
  {{MOD_LEFT_CTRL, 0}, {0, 0x39}},  // left-ctrl to capsLock
  {{0, 0x39}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  {{0, 0x04}, {0, 0x05}},  // 'a' to 'b'
  {{0, 0x05}, {0, 0x04}},  // 'b' to 'a'
  {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}},  // left-shift to left-ctrl
  {{MOD_RIGHT_SHIFT, 0x06}, {0, 0x04}},  // R-Shift + 'c' to 'a'
};
bool keyPressedFlags[KEYMAP_SIZE];  // TODO: Confirm that this occupies KEYMAP_SIZE bits, but not KEYMAP_SIZE * 8 bits for memory efficiency.

class KbdRptParser : public KeyboardReportParser
{
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
};

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  uint8_t mappedMod, mappedKey;
  bool isKeyMapped, mappedKeyPressed;

  int mappingOccurred = keymap::onModChanged(keymaps, KEYMAP_SIZE, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed);
  if (mappingOccurred == 0) { return; }

  if (isKeyMapped) {
    if (mappedKeyPressed) {
      keyboard::reportPress(mappedMod, mappedKey);
    } else {
      keyboard::reportRelease(mappedMod, mappedKey);
    }
  }

  keyboard::reportModifier(mappedMod);
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  keymap::onKeyPressed(keymaps, KEYMAP_SIZE, keyPressedFlags, &mod, &key);

  if (key == 0) {
    if (mod > 0) {
      keyboard::reportModifier(mod);
    }
  } else {
    keyboard::reportPress(mod, key);
  }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  keymap::onKeyReleased(keymaps, KEYMAP_SIZE, keyPressedFlags, &mod, &key);

  if (key == 0) {
    if (mod > 0) {
      keyboard::reportModifier(mod);
    }
  } else {
    keyboard::reportRelease(mod, key);
  }
}

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  keyboard::initKeyboard();

  memset(&keyPressedFlags, 0, sizeof(keyPressedFlags));

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
