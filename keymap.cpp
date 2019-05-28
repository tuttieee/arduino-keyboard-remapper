#include <string.h>
#include "keymap.h"

namespace keymap {

bool isKeyIncluded(uint8_t key, uint8_t keys[]) {
  for (uint8_t i = 0; i < 6; ++i) {
    if (key == keys[i]) {
      return true;
    }
  }
  return false;
}

bool keymapMatched(KeyMap keymap, uint8_t keys[]) {
  for (uint8_t j = 0; j < KEYS_NUM; ++j) {
    if (!isKeyIncluded(keymap.src.keys[j], keys)) {
      return false;
    }
  }

  return true;
}

void removeKeys(KeyMap keymap, uint8_t keys[]) {
  for (uint8_t j = 0; j < KEYS_NUM; ++j) {
    uint8_t keyToRemove = keymap.src.keys[j];
    for (uint8_t i = 0; i < 6; i++) {
      if (keyToRemove == keys[i]) {
        // Remove
        keys[i] = 0;
        // Push the followings
        for (uint8_t k = i + 1; k < 6; ++k) {
          keys[k - 1] = keys[k];
        }
        continue;
      }
    }
  }
}

void onKeysChanged(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t mod, uint8_t* sortedKeys, bool* isMappedModChanged, uint8_t* mappedMod, uint8_t* mappedKeys) {
  memset(mappedKeys, 0, KEY_REPORT_KEYS_NUM * sizeof(uint8_t));

  uint8_t mappedKeySetIdx = 0;

  *mappedMod = mod;
  *isMappedModChanged = false;
  uint8_t mappedModBits = 0;

  // Already mapped mods are ignored
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      *mappedMod &= ~keymaps[i].src.mod;
    }
  }

  uint8_t keyPool[6];
  memcpy(keyPool, sortedKeys, sizeof(uint8_t) * 6);

  for (uint8_t iKeymap = 0; iKeymap < keymapSize; iKeymap++) {
    if ((keymaps[iKeymap].src.mod == 0 || keymaps[iKeymap].src.mod == mod)
        && keymapMatched(keymaps[iKeymap], keyPool)) {
      removeKeys(keymaps[iKeymap], keyPool);

      if (keyPressedFlags[iKeymap] == false) {
        // Newly pressed
        if (keymaps[iKeymap].dst.key != 0) {
          mappedKeys[mappedKeySetIdx++] = keymaps[iKeymap].dst.key;
        }
      }

      // mappedMod is set even if the keymap is already matched
      if (keymaps[iKeymap].dst.mod != 0) {
        // Set keymaps[i].dst.mod's bit in mappedMod to 1;
        *mappedMod |= keymaps[iKeymap].dst.mod;
        // Set keymaps[i].src.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
        *mappedMod &= ~(keymaps[iKeymap].src.mod & ~mappedModBits);
        // Save the mapped bit
        mappedModBits |= keymaps[iKeymap].src.mod;

        if (keyPressedFlags[iKeymap] == false) {
          *isMappedModChanged = true;
        }
      }

      keyPressedFlags[iKeymap] = true;
    } else {
      // mappedMod is set even if the keymap is already matched
      if (keymaps[iKeymap].dst.mod != 0) {
        // // Set keymaps[i].dst.mod's bit in mappedMod to 0 since it is regarded as being released;
        // *mappedMod &= ~keymaps[iKeymap].dst.mod;
        // // Set keymaps[i].src.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
        // *mappedMod &= ~(keymaps[iKeymap].src.mod & ~mappedModBits);
        // // Save the mapped bit
        // mappedModBits |= keymaps[iKeymap].src.mod;

        if (keyPressedFlags[iKeymap] == true) {
          *isMappedModChanged = true;
        }
      }

      keyPressedFlags[iKeymap] = false;
    }
  }

  memcpy(mappedKeys + mappedKeySetIdx, keyPool, sizeof(uint8_t) * (6 - mappedKeySetIdx));
}

}
