#include "gtest/gtest.h"

#include "../../../keymap.h"

using namespace keymap;

class CapsCtrlPlusKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x13, 0x39}}, {0, 0x52}},  // capsLock + 'p' to 'up'
    {{0, {0x39, 0}}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  };
};

TEST_F(CapsCtrlPlusKeyTest, onKeyPressedCaps) {
  uint8_t mod = 0;

  uint8_t mappedMod;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t sortedKeysAfter0[] = {0x39, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter0, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }

  uint8_t sortedKeysAfter1[] = {0x13, 0x39, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter1, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(mappedKeys[0], 0x52);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }

  uint8_t sortedKeysAfter2[] = {0x39, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter2, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }

  uint8_t sortedKeysAfter3[] = {0, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter3, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(CapsCtrlPlusKeyTest, CapsLockWithUnmappedKey) {
  uint8_t mod = 0;

  uint8_t mappedMod;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t sortedKeysAfter0[] = {0x04, 0x39, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter0, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  EXPECT_EQ(mappedKeys[0], 0x04);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
