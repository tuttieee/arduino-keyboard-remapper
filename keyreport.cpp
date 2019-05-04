#include <string.h>
#include "keyreport.h"

namespace keyreport {

void releaseAllKey(KeyReport* keyReport) {
  memset(keyReport, 0, sizeof(KeyReport));
}

void setKeyPressed(KeyReport* keyReport, uint8_t mod, uint8_t key) {
  if (key != 0) {
    bool alreadySet = false;
    int8_t emptySlot = -1;
    for (uint8_t i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
      if (keyReport->keys[i] == key) {
        alreadySet = true;
      }
      if (keyReport->keys[i] == 0x0 && emptySlot < 0) {
        emptySlot = i;
      }
    }
    if (emptySlot < 0) {
      // Error condition.
      return;
    }

    if (!alreadySet) {
      keyReport->keys[emptySlot] = key;
    }
  }

  keyReport->modifiers = mod;
}

void setKeyReleased(KeyReport* keyReport, uint8_t mod, uint8_t key) {
  if (key != 0) {
    for (uint8_t i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
      if (keyReport->keys[i] == key) {
        keyReport->keys[i] = 0;
        break;
      }
    }
  }

  keyReport->modifiers = mod;
}

}
