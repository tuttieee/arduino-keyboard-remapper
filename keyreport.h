#ifndef KEY_REPORT_h
#define KEY_REPORT_h

#include "stdint.h"

namespace keyreport {

//  Low level key report: up to KEY_REPORT_KEYS_NUM keys and shift, ctrl etc at once
#define KEY_REPORT_KEYS_NUM 8
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[KEY_REPORT_KEYS_NUM];
} KeyReport;

// This must match to MODIFIERKEYS defined in hidboot.h
#define MOD_LEFT_CTRL   0b00000001
#define MOD_LEFT_SHIFT  0b00000010
#define MOD_LEFT_ALT    0b00000100
#define MOD_LEFT_GUI    0b00001000
#define MOD_RIGHT_CTRL  0b00010000
#define MOD_RIGHT_SHIFT 0b00100000
#define MOD_RIGHT_ALT   0b01000000
#define MOD_RIGHT_GUI   0b10000000

void releaseAllKey(KeyReport* keyReport);
void setKeyPressed(KeyReport* keyReport, uint8_t mod, uint8_t key);
void setKeyReleased(KeyReport* keyReport, uint8_t mod, uint8_t key);
}

#endif
