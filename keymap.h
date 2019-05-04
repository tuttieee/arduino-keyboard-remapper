#ifndef KEYMAP_h
#define KEYMAP_h

#include "stdint.h"

namespace keymap {

typedef struct {
  uint8_t mod;
  uint8_t key;
} KeyCode;
typedef struct {
  KeyCode from;
  KeyCode to;
} KeyMap;

typedef bool KeyPressedFlag;

void onKeyPressed(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key);
void onKeyReleased(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* key);
int onModChanged(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t before, uint8_t after, uint8_t* mappedMod, bool* isKeyMapped, uint8_t* mappedKey, bool* mappedKeyPressed);

}

#endif
