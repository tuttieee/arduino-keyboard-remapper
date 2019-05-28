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

#define MAPPED_KEYS_NUM 6

#define KEYS_NUM 2

typedef struct {
  uint8_t mod;
  uint8_t keys[KEYS_NUM];
} SrcKeyState;
typedef struct {
  uint8_t mod;
  uint8_t key;
} DstKeyState;
typedef struct {
  SrcKeyState src;
  DstKeyState dst;
} KeyMap;

typedef bool KeyPressedFlag;

// void onKeyPressed(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* keys, uint8_t *mappedKey);
// void onKeyReleased(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t* mod, uint8_t* keys, uint8_t *mappedKey);
void onKeysChanged(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t mod, uint8_t* sortedKeysBefore, uint8_t* sortedKeysAfter, bool* isMappedModChanged, uint8_t* mappedMod, uint8_t* pressedKeys, uint8_t* releasedKeys);
int onModChanged(KeyMap keymaps[], int keymapSize, KeyPressedFlag keyPressedFlags[], uint8_t before, uint8_t after, uint8_t* mappedMod, bool* isKeyMapped, uint8_t* mappedKey, bool* mappedKeyPressed);

bool isKeyIncluded(uint8_t key, uint8_t keys[]);
bool keymapMatched(KeyMap keymap, uint8_t keys[]);
void removeKeys(KeyMap keymap, uint8_t keys[]);
}

#endif
