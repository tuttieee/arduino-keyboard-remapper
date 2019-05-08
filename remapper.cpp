#include <Usb.h>

#include "remapper.h"

#include "keymap.h"
#include "keyboard.h"

const keymap::KeyMap defaultKeymaps[KEYMAP_SIZE] =
{
  {{MOD_LEFT_CTRL, 0}, {0, 0x39}},  // left-ctrl to capsLock
  {{0, 0x39}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  {{0, 0x04}, {0, 0x05}},  // 'a' to 'b'
  {{0, 0x05}, {0, 0x04}},  // 'b' to 'a'
  {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}},  // left-shift to left-ctrl
  {{MOD_RIGHT_SHIFT, 0x06}, {0, 0x04}},  // R-Shift + 'c' to 'a'
};

void sortKeys(uint8_t *src, uint8_t *dst) {
  // TODO: Replace with more efficient implementation. This is just bubble sort.
  for (uint8_t i=0; i<6; i++) {
    dst[i] = src[i];
  }

  for (uint8_t i=0; i<5; i++) {
    for (uint8_t j=0; j<i; j++) {
      if (dst[j] < dst[j+1]) {
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

  // provide event for changed control key state
  if (prevState.bInfo[0x00] != buf[0x00]) {
    OnControlKeysChanged(prevState.bInfo[0x00], buf[0x00]);
  }

  uint8_t sortedKeys[6];
  sortKeys(buf+2, sortedKeys);

  for (uint8_t i = 0; i < 6; i++) {
    bool down = false;
    bool up = false;

    for (uint8_t j = 0; j < 6; j++) {
      if (sortedKeys[i] == prevState.bInfo[j+2] && buf[i] != 1)
        down = true;
      if (buf[j] == prevState.bInfo[i+2] && prevState.bInfo[i+2] != 1)
        up = true;
    }
    if (!down) {
      // HandleLockingKeys(hid, buf[i]);
      // OnKeyDown(*buf, buf[i]);
      OnKeyChanged(*buf, sortedKeys[i], true, prevState.bInfo+2, sortedKeys);
    }
    if (!up)
      // OnKeyUp(prevState.bInfo[0], prevState.bInfo[i]);
      OnKeyChanged(prevState.bInfo[0], prevState.bInfo[i+2], false, prevState.bInfo+2, sortedKeys);
  }
  for (uint8_t i = 0; i < 2; i++)
    prevState.bInfo[i] = buf[i];
  for (uint8_t i = 0; i < 6; i++)
    prevState.bInfo[i+2] = sortedKeys[i];
};

void KbdRemapper::OnKeyChanged(uint8_t mod, uint8_t key, bool pressed, uint8_t *sortedKeysBefore, uint8_t *sortedKeysAfter) {
  printMod(mod);
  Serial.print(" ");
  for (uint8_t i=0; i<6; i++) {
    PrintHex<uint8_t>(sortedKeysBefore[i], 0x80);
    Serial.print(" ");
  }
  Serial.print("-> ");
  for (uint8_t i=0; i<6; i++) {
    PrintHex<uint8_t>(sortedKeysAfter[i], 0x80);
    Serial.print(" ");
  }
  Serial.print(" (");
  if (pressed) Serial.print("DN "); else Serial.print("UP ");
  PrintHex<uint8_t>(key, 0x80);
  Serial.println(")");
}

void KbdRemapper::OnControlKeysChanged(uint8_t before, uint8_t after) {
  Serial.print("MO ");
  printMod(before);
  Serial.print(" -> ");
  printMod(after);
  Serial.print("\n");

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

void KbdRemapper::OnKeyDown(uint8_t mod, uint8_t key) {
  Serial.print("DN ");
  printMod(mod);
  Serial.print(" ");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("\n");

  keymap::onKeyPressed(keymaps, KEYMAP_SIZE, keyPressedFlags, &mod, &key);

  if (key == 0) {
    if (mod > 0) {
      keyboard::reportModifier(mod);
    }
  } else {
    keyboard::reportPress(mod, key);
  }
}

void KbdRemapper::OnKeyUp(uint8_t mod, uint8_t key) {
  Serial.print("UP ");
  printMod(mod);
  Serial.print(" ");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("\n");

  keymap::onKeyReleased(keymaps, KEYMAP_SIZE, keyPressedFlags, &mod, &key);

  if (key == 0) {
    if (mod > 0) {
      keyboard::reportModifier(mod);
    }
  } else {
    keyboard::reportRelease(mod, key);
  }
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
