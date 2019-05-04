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

void releaseAllKey(KeyReport* keyReport);
void setKeyPressed(KeyReport* keyReport, uint8_t mod, uint8_t key);
void setKeyReleased(KeyReport* keyReport, uint8_t mod, uint8_t key);
}

#endif
