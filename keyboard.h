#ifndef keyboard_h
#define keyboard_h

#include "HID.h"

namespace keyboard {

void initKeyboard(void);
void reportPress(uint8_t mod, uint8_t key);
void reportRelease(uint8_t mod, uint8_t key);
void reportReleaseAll(void);

}
#endif
