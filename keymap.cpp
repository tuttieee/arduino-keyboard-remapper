#include "keymap.h"

namespace keymap {

void onKeyPressed(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  uint8_t mappedKey = *key;

  // FROM key TO key and mod
  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.key == *key) {
      mappedKey = keymaps[i].to.key;  // This assumes one 'from' key is mapped to at most 1 'to' key. If There are multiple mappings, only the last one affects.
      keyPressedFlags[i] = true; // On the other hand, 'from' key can be mapped to multiple 'to' mods.
    }
  }
  *key = mappedKey;

  // TO mod
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      *mod |= keymaps[i].to.mod;
    }
  }
}

void onKeyReleased(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  uint8_t mappedKey = *key;

  // FROM key TO key and mod
  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.key == *key) {
      mappedKey = keymaps[i].to.key;
      keyPressedFlags[i] = false;
    }
  }
  *key = mappedKey;

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
  uint8_t mappedModBits = 0;
  uint8_t mappedKey = 0;
  bool isKeyMapped = false, mappedKeyPressed;

  for (int i=0; i<keymapSize; i++) {
    if (keymaps[i].from.mod & change) {
      // keymaps[i].from.mod changes

      if (after & change) {
        // keymaps[i].from.mod DOWN (newly set to 1)
        keyPressedFlags[i] = true;

        // 'To' key is added. This assumes one 'from' mod is mapped to at most 1 'to' keys. If there are multiple 'to' keys, the only last one affects.
        if (keymaps[i].to.key != 0) {
          isKeyMapped = true;
          mappedKey = keymaps[i].to.key;
          mappedKeyPressed = true;
        }

        // Set mappedMod's target bit to 1
        mappedMod |= keymaps[i].to.mod;
      } else {
        // keymaps[i].from.mod UP (newly set to 0)
        keyPressedFlags[i] = false;

        // 'To' key is released.
        if (keymaps[i].to.key != 0) {
          isKeyMapped = true;
          mappedKey = keymaps[i].to.key;
          mappedKeyPressed = false;
        }

        // Set mappedMod's target bit to 0
        mappedMod &= ~keymaps[i].to.mod;
      }

      // Set mappedMod's source bit to 0 if the bit is not mapped from any mod bits;
      mappedMod &= ~(keymaps[i].from.mod & ~mappedModBits);

      // Save mapping bit
      mappedModBits |= keymaps[i].from.mod;
    }
  }

  *_mappedMod = mappedMod;
  *_isKeyMapped = isKeyMapped;
  *_mappedKey = mappedKey;
  *_mappedKeyPressed = mappedKeyPressed;

  return 1;
}

}
