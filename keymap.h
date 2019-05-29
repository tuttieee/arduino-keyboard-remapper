#ifndef KEYMAP_h
#define KEYMAP_h

#include "stdint.h"

#include "keyreport.h"

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

void onKeysChanged(KeyMap keymaps[], int keymapSize, uint8_t mod, uint8_t* sortedKeys, uint8_t* mappedMod, uint8_t* mappedKeys);

bool isKeyIncluded(uint8_t key, uint8_t keys[]);
bool keymapMatched(KeyMap keymap, uint8_t keys[]);
void removeKeys(KeyMap keymap, uint8_t keys[]);
}

#endif
