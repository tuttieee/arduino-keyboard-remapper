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

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);

  public:
    void init(void);
    void setKeymap(keymap::KeyMap *newKeymaps);
};

#endif
