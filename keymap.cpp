#include "keymap.h"

namespace keymap {

void onKeyPressed(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  uint8_t mappedMod = *mod;
  uint8_t mappedModBits = 0;
  uint8_t mappedKey = *key;

  // Combination keymaps are first priority at detecting pressing.
  bool combinationFound = false;
  for (int i=0; i<keymapSize; i++) {
    // from mod-key combination
    if (keymaps[i].from.key == *key && keymaps[i].from.mod & *mod) {
      // to key
      mappedKey = keymaps[i].to.key;
      // to mod
      // Set pressed flag to set mod later (see below)
      keyPressedFlags[i] = true;
      // Set keymaps[i].to.mod's bit in mappedMod to 1;
      mappedMod |= keymaps[i].to.mod;
      // Set keymaps[i].from.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
      mappedMod &= ~(keymaps[i].from.mod & ~mappedModBits);
      // Save the mapped bit
      mappedModBits |= keymaps[i].from.mod;

      combinationFound = true;
    }
  }

  // If no combination keymap matched to the given `key` is found, then search single keymaps.
  if (!combinationFound) {
    for (int i=0; i<keymapSize; i++) {
      // from key
      if (keymaps[i].from.key == *key && keymaps[i].from.mod == 0) {
        // to key
        mappedKey = keymaps[i].to.key;  // This assumes one 'from' key is mapped to at most 1 'to' key. If There are multiple mappings, only the last one affects.
        // to mod (continue to below)
        keyPressedFlags[i] = true; // On the other hand, 'from' key can be mapped to multiple 'to' mods.
      }

      // from mod
      if (keymaps[i].from.mod & *mod && keymaps[i].from.key == 0) {
        // keymaps[i].from.mod is pressed
        keyPressedFlags[i] = true;

        // TODO: to key

        // to mod
        // Set keymaps[i].to.mod's bit in mappedMod to 1;
        mappedMod |= keymaps[i].to.mod;
        // Set keymaps[i].from.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
        mappedMod &= ~(keymaps[i].from.mod & ~mappedModBits);
        // Save the mapped bit
        mappedModBits |= keymaps[i].from.mod;
      }
    }
  }

  // from key to mod
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      mappedMod |= keymaps[i].to.mod;
    }
  }

  *key = mappedKey;
  *mod = mappedMod;
}

void onKeyReleased(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key) {
  uint8_t mappedMod = *mod;
  uint8_t mappedModBits = 0;
  uint8_t mappedKey = *key;

  // Search single keymaps
  for (int i=0; i<keymapSize; i++) {
    // from key
    if (keymaps[i].from.key == *key) {
      // to key
      mappedKey = keymaps[i].to.key;
      // to mod (continue to below)
      keyPressedFlags[i] = false;
    }
    // from mod
    if (keymaps[i].from.mod & *mod) {
      // keymaps[i].from.mod is pressed
      keyPressedFlags[i] = true;

      // TODO: to key

      // to mod
      // Set keymaps[i].to.mod's bit in mappedMod to 1;
      mappedMod |= keymaps[i].to.mod;
      // Set keymaps[i].from.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
      mappedMod &= ~(keymaps[i].from.mod & ~mappedModBits);
      // Save the mapped bit
      mappedModBits |= keymaps[i].from.mod;
    }
  }

  // Combination keymaps are handled as OR condition at releasing.
  bool combinationFound = false;
  for (int i=0; i<keymapSize; i++) {
    // from mod-key combination
    if (keymaps[i].from.key == *key && keymaps[i].from.mod & *mod) {
      // to key
      mappedKey = keymaps[i].to.key;
      // to mod
      // Set pressed flag to set mod later (see below)
      keyPressedFlags[i] = false;
      // Set keymaps[i].to.mod's bit in mappedMod to 1;
      mappedMod |= keymaps[i].to.mod;
      // Set keymaps[i].from.mod's bit in mappedMod to 0 if the bit is not mapped from any mod bits;
      mappedMod &= ~(keymaps[i].from.mod & ~mappedModBits);
      // Save the mapped bit
      mappedModBits |= keymaps[i].from.mod;

      combinationFound = true;
    }
  }

  // from key to mod
  for (int i=0; i<keymapSize; i++) {
    if (keyPressedFlags[i]) {
      mappedMod |= keymaps[i].to.mod;
    }
  }

  *key = mappedKey;
  *mod = mappedMod;
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
