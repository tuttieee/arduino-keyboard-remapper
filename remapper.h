#ifndef REMAPPER_h
#define REMAPPER_h

#include <hidboot.h>

#include "keyreport.h"
#include "keymap.h"

#define KEYMAP_SIZE 8

class KbdRemapper : public KeyboardReportParser
{
  private:
    keymap::KeyMap *keymaps;

    void Parse(USBHID *hid, bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf);

    void debugPrint(uint8_t modBefore, uint8_t modAfter, uint8_t *keysBefore, uint8_t *keysAfter);

  public:
    void init(void);
    void setKeymap(keymap::KeyMap *newKeymaps);
};

#endif
