#include "gtest/gtest.h"

#include "../../../keymap.h"

#include "../utils.h"

using namespace keymap;

class CapsCtrlPlusKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x13, 0x39}}, {0, 0x52}},  // capsLock + 'p' to 'up'
    {{0, {0x39, 0}}, {MOD_LEFT_CTRL, 0}},  // capsLock to left-ctrl
  };
  KeyPressedFlag keyPressedFlags[2];
};

TEST_F(CapsCtrlPlusKeyTest, onKeyPressedCaps) {
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

  uint8_t sortedKeysAfter1[] = {0x13, 0x39, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysAfter1, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(isMappedModChanged, true);
  EXPECT_EQ(mappedKeys[0], 0x52);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }

  uint8_t sortedKeysAfter2[] = {0x39, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysAfter2, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  EXPECT_EQ(isMappedModChanged, true);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }

  uint8_t sortedKeysAfter3[] = {0, 0, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysAfter3, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(isMappedModChanged, true);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(CapsCtrlPlusKeyTest, CapsLockWithUnmappedKey) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);

  uint8_t mod = 0;

  uint8_t mappedMod;
  bool isMappedModChanged;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  keyPressedFlags[1] = true;  // 1st keymap is already matched
  uint8_t sortedKeysAfter0[] = {0x04, 0x39, 0, 0, 0, 0};
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysAfter0, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedKeys[0], 0x04);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
