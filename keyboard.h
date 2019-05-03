#ifndef keyboard_h
#define keyboard_h

#include "HID.h"

namespace keyboard {

//  Low level key report: up to KEY_REPORT_KEYS_NUM keys and shift, ctrl etc at once
#define KEY_REPORT_KEYS_NUM 8
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[KEY_REPORT_KEYS_NUM];
} KeyReport;

void initKeyboard(void);
void reportPress(uint8_t mod, uint8_t key);
void reportRelease(uint8_t mod, uint8_t key);
void reportReleaseAll(void);

}
#endif
