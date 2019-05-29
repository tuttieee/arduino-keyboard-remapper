#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "keymap.h"
#include "remapper.h"

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

/**
 * NOTE: keymaps MUST be sorted to follow the rules
 * 1. Each keymap.src.keys are ASC order
 * 2. Upper keymaps have higher priority than lower ones.
 *    Therefore, combination keymaps like `CapsLock + 'p'`
 *    should be upper than single keymaps like `CapsLock`.
 */
const keymap::KeyMap emacsKeymaps[KEYMAP_SIZE] =
{
  {{0, {0x13, 0x39}}, {0, 0x52}},  // ctrl-p to up
  {{0, {0x11, 0x39}}, {0, 0x51}},  // ctrl-n to down
  {{0, {0x05, 0x39}}, {0, 0x50}},  // ctrl-b to left
  {{0, {0x09, 0x39}}, {0, 0x4f}},  // ctrl-f to right
  {{0, {0x0a, 0x39}}, {0, 0x29}},  // ctrl-g to ESC
  {{0, {0x0b, 0x39}}, {0, 0x2a}},  // ctrl-h to BKSP
  {{0, {0x10, 0x39}}, {0, 0x28}},  // ctrl-m to Enter
  {{0, {0x39, 0   }}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
};

KbdRemapper Rmp;

void setup()
{
  Rmp.init();

  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Rmp);

  Rmp.setKeymap(emacsKeymaps);  // TODO: Be switchable
}

void loop()
{
  Usb.Task();
}
