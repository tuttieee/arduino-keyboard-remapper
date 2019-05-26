#ifndef REMAPPER_h
#define REMAPPER_h

#include <hidboot.h>

#include "keymap.h"

#define KEYMAP_SIZE 8

class KbdRemapper : public KeyboardReportParser
{
  private:
    keymap::KeyMap *keymaps;
    bool keyPressedFlags[KEYMAP_SIZE];

    void Parse(USBHID *hid, bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf);
    void printMod(uint8_t m);

    void debugPrint(uint8_t mod, uint8_t key, bool pressed, uint8_t *keysBefore, uint8_t *keysAfter);

  protected:

    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);

  public:
    void init(void);
    void setKeymap(keymap::KeyMap *newKeymaps);
};

#endif
