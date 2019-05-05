#ifndef REMAPPER_h
#define REMAPPER_h

#include <hidboot.h>

class KbdRptParser : public KeyboardReportParser
{
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
};

void initRemapper(void);

#endif
