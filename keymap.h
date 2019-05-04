#ifndef KEYMAP_h
#define KEYMAP_h

#include "stdint.h"

// This must match to MODIFIERKEYS defined in hidboot.h
#define MOD_LEFT_CTRL   0b00000001
#define MOD_LEFT_SHIFT  0b00000010
#define MOD_LEFT_ALT    0b00000100
#define MOD_LEFT_GUI    0b00001000
#define MOD_RIGHT_CTRL  0b00010000
#define MOD_RIGHT_SHIFT 0b00100000
#define MOD_RIGHT_ALT   0b01000000
#define MOD_RIGHT_GUI   0b10000000

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
