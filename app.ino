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

const keymap::KeyMap emacsKeymaps[KEYMAP_SIZE] =
{
  {{0, 0x39}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  {{MOD_LEFT_CTRL, 0x13}, {0, 0x52}},  // ctrl-p to up
  {{MOD_LEFT_CTRL, 0x11}, {0, 0x51}},  // ctrl-n to down
  {{MOD_LEFT_CTRL, 0x05}, {0, 0x50}},  // ctrl-b to left
  {{MOD_LEFT_CTRL, 0x09}, {0, 0x4f}},  // ctrl-f to right
  {{MOD_LEFT_CTRL, 0x0a}, {0, 0x29}},  // ctrl-g to ESC
  {{MOD_LEFT_CTRL, 0x0b}, {0, 0x2a}},  // ctrl-h to BKSP
  {{MOD_LEFT_CTRL, 0x10}, {0, 0x28}},  // ctrl-m to Enter
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
