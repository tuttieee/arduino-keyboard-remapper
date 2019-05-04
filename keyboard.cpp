#include "keyboard.h"
#include "keyreport.h"

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

keyreport::KeyReport keyReport;

typedef struct {
  uint8_t mod;
  uint8_t key;
} KeyCode;
typedef struct {
  KeyCode from;
  KeyCode to;
} KeyMap;

#define KEYMAP_SIZE 3
const KeyMap keymaps[KEYMAP_SIZE] =
{
  {{MOD_LEFT_CTRL, 0}, {0, 0x39}},  // left-ctrl to capsLock
  {{0, 0x39}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  // {{0, 0x04}, {0, 0x05}},  // 'a' to 'b'  // Not working
  {{0, 0x05}, {0, 0x04}},  // 'b' to 'a'
  // {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}},  // Not working
};
bool keyPressed[KEYMAP_SIZE];  // TODO: Confirm that this occupies KEYMAP_SIZE bits, but not KEYMAP_SIZE * 8 bits for memory efficiency.

void _sendReport()
{
  HID().SendReport(2, &keyReport, sizeof(keyreport::KeyReport));
}

void keyboard::initKeyboard(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);

  keyreport::releaseAllKey(&keyReport);

  memset(&keyPressed, 0, sizeof(keyPressed));
}

void keyboard::reportPress(uint8_t mod, uint8_t key)
{
  // FROM key TO key and mod
  for (int i=0; i<KEYMAP_SIZE; i++) {
    if (keymaps[i].from.key == key) {
      key = keymaps[i].to.key;  // This assumes one 'from' key is mapped to at most 1 'to' key. If There are multiple mappings, only the last one affects.
      keyPressed[i] = true;     // On the other hand, 'from' key can be mapped to multiple 'to' mods.
    }
  }

  // TO mod
  for (int i=0; i<KEYMAP_SIZE; i++) {
    if (keyPressed[i]) {
      mod |= keymaps[i].to.mod;
    }
  }

  keyreport::setKeyPressed(&keyReport, mod, key);
  _sendReport();
}

void keyboard::reportRelease(uint8_t mod, uint8_t key)
{
  // FROM key TO key and mod
  for (int i=0; i<KEYMAP_SIZE; i++) {
    if (keymaps[i].from.key == key) {
      key = keymaps[i].to.key;
      keyPressed[i] = false;
    }
  }

  // TO mod
  for (int i=0; i<KEYMAP_SIZE; i++) {
    if (keyPressed[i]) {
      mod |= keymaps[i].to.mod;
    }
  }

  keyreport::setKeyReleased(&keyReport, mod, key);
  _sendReport();
}

void keyboard::reportModifier(uint8_t before, uint8_t after) {
  uint8_t change = before ^ after;
  if (change == 0) { return; }

  uint8_t mappedMod = after;

  for (int i=0; i<KEYMAP_SIZE; i++) {
    if (keymaps[i].from.mod & change) {
      // keymaps[i].from.mod changes

      // keymaps[i].from.mod's bit is restored from `before`
      mappedMod = (mappedMod & ~keymaps[i].from.mod) | (before & keymaps[i].from.mod);

      if (after & change) {
        // keymaps[i].from.mod DOWN
        keyPressed[i] = true;

        // 'To' key is added. This assumes one 'from' mod is mapped to at most KEY_REPORT_KEYS_NUM 'to' keys. If there are more 'to' keys, they are ignored and end up an error.
        keyreport::setKeyPressed(&keyReport, mappedMod, keymaps[i].to.key);

        // 'To' mod is added.
        mappedMod |= keymaps[i].to.mod;
      } else {
        // keymaps[i].from.mod UP
        keyPressed[i] = false;

        // 'To' key is released.
        keyreport::setKeyReleased(&keyReport, mappedMod, keymaps[i].to.key);

        // 'To' mod is removed
        mappedMod |= ~keymaps[i].to.mod;
      }
    }
  }

  keyReport.modifiers = mappedMod;
  _sendReport();
}

void keyboard::reportReleaseAll(void)
{
  keyreport::releaseAllKey(&keyReport);
  _sendReport();
}
