#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "keyboard.h"

class KbdRptParser : public KeyboardReportParser
{
    // void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  uint8_t change = before ^ after;
  if (change != 0) {
    keyboard::reportModifier(after);
  }
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  keyboard::reportPress(mod, key);
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  keyboard::reportRelease(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("OnKeyPressed\n");
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
