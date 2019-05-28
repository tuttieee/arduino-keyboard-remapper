#include "gtest/gtest.h"

#include "../../../keymap.h"

#include "../utils.h"

using namespace keymap;

class MultiDstModTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x39, 0}}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
    {{MOD_LEFT_SHIFT, {0, 0}}, {MOD_LEFT_CTRL, 0}},  // left-shift to left-ctrl
  };
  KeyPressedFlag keyPressedFlags[2];
};

TEST_F(MultiDstModTest, ) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);

  uint8_t mod = 0;

  uint8_t mappedMod;
  bool isMappedModChanged;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t sortedKeysAfter0[] = {0x39, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysAfter0, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  EXPECT_EQ(isMappedModChanged, true);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
