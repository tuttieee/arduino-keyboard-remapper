#include "gtest/gtest.h"

#include "../../../keymap.h"

using namespace keymap;

class MultiDstModTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x39, 0}}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
    {{MOD_LEFT_SHIFT, {0, 0}}, {MOD_LEFT_CTRL, 0}},  // left-shift to left-ctrl
  };
};

TEST_F(MultiDstModTest, ) {
  uint8_t mod = 0;

  uint8_t mappedMod;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t sortedKeysAfter0[] = {0x39, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter0, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
