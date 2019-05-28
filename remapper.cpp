#include <Usb.h>

#include "remapper.h"

#include "keymap.h"
#include "keyboard.h"

const keymap::KeyMap defaultKeymaps[KEYMAP_SIZE] =
{
  {{MOD_LEFT_CTRL, {0, 0}}, {0, 0x39}},  // left-ctrl to capsLock
  {{0, {0x39, 0}}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  {{0, {0x04, 0}}, {0, 0x05}},  // 'a' to 'b'
  {{0, {0x05, 0}}, {0, 0x04}},  // 'b' to 'a'
  {{MOD_LEFT_SHIFT, {0, 0}}, {MOD_LEFT_CTRL, 0}},  // left-shift to left-ctrl
  {{MOD_RIGHT_SHIFT, {0x06, 0}}, {0, 0x04}},  // R-Shift + 'c' to 'a'
};

void sortKeys(uint8_t *src, uint8_t *dst) {
  // TODO: Replace with more efficient implementation. This is just bubble sort.
  for (uint8_t i=0; i<6; i++) {
    dst[i] = src[i];
  }

  for (uint8_t i = 0; i < 6; i++) {
    if (dst[i] == 0) {
      continue;
    }

    for (uint8_t j = 0; j < i; j++) {
      if (dst[j+1] != 0 && dst[j] > dst[j+1]) {
        uint8_t tmp = dst[j+1];
        dst[j+1] = dst[j];
        dst[j] = tmp;
      }
    }
  }
}

void KbdRemapper::Parse(USBHID *hid, bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf) {
  // On error - return
  if (buf[2] == 1)
    return;

  //KBDINFO       *pki = (KBDINFO*)buf;

  uint8_t sortedKeys[6];
  sortKeys(buf+2, sortedKeys);

  bool isMappedModChanged;
  uint8_t mappedMod;
  uint8_t mappedKeys[MAPPED_KEYS_NUM];
  keymap::onKeysChanged(keymaps, KEYMAP_SIZE, keyPressedFlags, *buf, sortedKeys, &isMappedModChanged, &mappedMod, mappedKeys);
  keyboard::updateKeys(mappedMod, mappedKeys);

  debugPrint(prevState.bInfo[0], *buf, prevState.bInfo+2, sortedKeys);

  Serial.print("Mapped keys: ");
  printMod(mappedMod);
  Serial.print(" ");
  for (uint8_t i=0; i<MAPPED_KEYS_NUM; i++) {
    PrintHex<uint8_t>(mappedKeys[i], 0x80);
    Serial.print(" ");
  }
  Serial.print("\n");

  for (uint8_t i = 0; i < 2; i++)
    prevState.bInfo[i] = buf[i];
  for (uint8_t i = 0; i < 6; i++)
    prevState.bInfo[i+2] = sortedKeys[i];
};

void KbdRemapper::debugPrint(uint8_t modBefore, uint8_t modAfter, uint8_t *sortedKeysBefore, uint8_t *sortedKeysAfter) {
  printMod(modBefore);
  Serial.print(" ");
  for (uint8_t i=0; i<6; i++) {
    PrintHex<uint8_t>(sortedKeysBefore[i], 0x80);
    Serial.print(" ");
  }
  Serial.print("-> ");
  printMod(modAfter);
  Serial.print(" ");
  for (uint8_t i=0; i<6; i++) {
    PrintHex<uint8_t>(sortedKeysAfter[i], 0x80);
    Serial.print(" ");
  }
  Serial.print("\n");
}

void KbdRemapper::init(void) {
  keyboard::initKeyboard();

  setKeymap(defaultKeymaps);
  memset(&keyPressedFlags, 0, sizeof(keyPressedFlags));
}

void KbdRemapper::setKeymap(keymap::KeyMap *newKeymaps) {
  keymaps = newKeymaps;
}

void KbdRemapper::printMod(uint8_t m) {
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;

  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : "_");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : "_");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : "_");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : "_");

  Serial.print(" ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : "_");
  Serial.print((mod.bmRightShift  == 1) ? "S" : "_");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : "_");
  Serial.print((mod.bmRightGUI    == 1) ? "G" : "_");
}
