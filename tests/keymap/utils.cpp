#include "utils.h"

void clearKeyPressedFlags(keymap::KeyPressedFlag keyPressedFlags[], int size, bool value) {
  for (int i=0; i < size; i++) {
    keyPressedFlags[i] = value;
  }
}
