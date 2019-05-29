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

void removeMod(KeyMap keymap, uint8_t* mod) {
  *mod &= ~keymap.src.mod;
}

void onKeysChanged(KeyMap keymaps[], int keymapSize, uint8_t mod, uint8_t* sortedKeys, uint8_t* mappedMod, uint8_t* mappedKeys) {
  *mappedMod = 0;
  memset(mappedKeys, 0, KEY_REPORT_KEYS_NUM * sizeof(uint8_t));

  uint8_t mappedKeySetIdx = 0;

  uint8_t modPool = mod;
  uint8_t keyPool[6];
  memcpy(keyPool, sortedKeys, sizeof(uint8_t) * 6);

  for (uint8_t iKeymap = 0; iKeymap < keymapSize; iKeymap++) {
    if ((keymaps[iKeymap].src.mod == 0 || keymaps[iKeymap].src.mod == modPool)
        && keymapMatched(keymaps[iKeymap], keyPool)) {
      removeKeys(keymaps[iKeymap], keyPool);
      removeMod(keymaps[iKeymap], &modPool);

      if (keymaps[iKeymap].dst.key != 0) {
        mappedKeys[mappedKeySetIdx++] = keymaps[iKeymap].dst.key;
      }

      if (keymaps[iKeymap].dst.mod != 0) {
        *mappedMod |= keymaps[iKeymap].dst.mod;
      }
    }
  }

  memcpy(mappedKeys + mappedKeySetIdx, keyPool, sizeof(uint8_t) * (6 - mappedKeySetIdx));
  *mappedMod |= modPool;
}

}
