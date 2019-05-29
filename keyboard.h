#ifndef keyboard_h
#define keyboard_h

#include "HID.h"

namespace keyboard {

void initKeyboard(void);
void updateKeys(uint8_t mod, uint8_t keys[]);
void reportReleaseAll(void);

}
#endif
