#ifndef REMAPPER_h
#define REMAPPER_h

#include <hidboot.h>

class KbdRemapper : public KeyboardReportParser
{
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);

  public:
    void init(void);
};

#endif
