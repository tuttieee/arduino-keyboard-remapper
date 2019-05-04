#include "keymap.h"

namespace keymap {

void onKeyPressed(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  // FROM key TO key and mod
  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.key == *key) {
      *key = keymaps[i].to.key;  // This assumes one 'from' key is mapped to at most 1 'to' key. If There are multiple mappings, only the last one affects.
      keyPressedFlags[i] = true; // On the other hand, 'from' key can be mapped to multiple 'to' mods.
    }
  }

  // TO mod
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      *mod |= keymaps[i].to.mod;
    }
  }
}

void onKeyReleased(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  // FROM key TO key and mod
  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.key == *key) {
      *key = keymaps[i].to.key;
      keyPressedFlags[i] = false;
    }
  }

  // TO mod
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      *mod |= keymaps[i].to.mod;
    }
  }
}

int onModChanged(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t before, uint8_t after, uint8_t* _mappedMod, bool* _isKeyMapped, uint8_t* _mappedKey, bool* _mappedKeyPressed) {
  uint8_t change = before ^ after;
  if (change == 0) { return 0; }

  uint8_t mappedMod = after;
  uint8_t mappedKey = 0;
  bool mappedKeyPressed;

  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.mod & change) {
      // keymaps[i].from.mod changes

      // keymaps[i].from.mod's bit is restored from `before`
      mappedMod = (mappedMod & ~keymaps[i].from.mod) | (before & keymaps[i].from.mod);

      if (after & change) {
        // keymaps[i].from.mod DOWN
        keyPressedFlags[i] = true;

        // 'To' key is added. This assumes one 'from' mod is mapped to at most 1 'to' keys. If there are multiple 'to' keys, the only last one affects.
        *_isKeyMapped = true;
        mappedKey = keymaps[i].to.key;
        mappedKeyPressed = true;

        // 'To' mod is added.
        mappedMod |= keymaps[i].to.mod;
      } else {
        // keymaps[i].from.mod UP
        keyPressedFlags[i] = false;

        // 'To' key is released.
        *_isKeyMapped = true;
        mappedKey = keymaps[i].to.key;
        mappedKeyPressed = false;

        // 'To' mod is removed
        mappedMod |= ~keymaps[i].to.mod;
      }
    }
  }

  *_mappedMod = mappedMod;
  *_mappedKey = mappedKey;
  *_mappedKeyPressed = mappedKeyPressed;

  return 1;
}

}
