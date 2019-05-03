#include "keyboard.h"

static const uint8_t _hidReportDescriptor[] PROGMEM = {

  //  Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)

  0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

  0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x73,                    //   LOGICAL_MAXIMUM (115)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

keyboard::KeyReport keyReport;

void _releaseAll(void)
{
  memset(&keyReport, 0, sizeof(keyboard::KeyReport));
}

void _sendReport()
{
  HID().SendReport(2, &keyReport, sizeof(keyboard::KeyReport));
}

void keyboard::initKeyboard(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);

  _releaseAll();
}

void keyboard::reportPress(uint8_t mod, uint8_t key)
{
  if (key != 0) {
    bool alreadySet = false;
    int8_t emptySlot = -1;
    for (uint8_t i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
      if (keyReport.keys[i] == key) {
        alreadySet = true;
      }
      if (keyReport.keys[i] == 0x0 && emptySlot < 0) {
        emptySlot = i;
      }
    }
    if (emptySlot < 0) {
      // Error condition.
      return;
    }

    if (!alreadySet) {
      keyReport.keys[emptySlot] = key;
    }
  }

  keyReport.modifiers = mod;

  _sendReport();
}

void keyboard::reportRelease(uint8_t mod, uint8_t key)
{
  if (key != 0) {
    for (uint8_t i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
      if (keyReport.keys[i] == key) {
        keyReport.keys[i] = 0;
        break;
      }
    }
  }

  keyReport.modifiers = mod;

  _sendReport();
}

void keyboard::reportReleaseAll(void)
{
  _releaseAll();
  _sendReport();
}
