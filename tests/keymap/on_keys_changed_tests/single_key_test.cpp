#include "gtest/gtest.h"

#include "../../../keymap.h"

using namespace keymap;

class SingleKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x04, 0}}, {0, 0x05}},  // 'a' to 'b'
    {{0, {0x05, 0}}, {0, 0x04}},  // 'b' to 'a'
  };
};

TEST_F(SingleKeyTest, NoKeyPressed) {
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysAfter[6] = {0, 0, 0, 0, 0, 0};

  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  for (int i = 0; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(SingleKeyTest, SingleKeyPressedButNoMapMatched) {
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysAfter[6] = {0x06, 0, 0, 0, 0, 0};

  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(mappedKeys[0], 0x06);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(SingleKeyTest, SingleKeyToKey) {
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[KEY_REPORT_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysAfter[6] = {0x04, 0, 0, 0, 0, 0};

  onKeysChanged(keymaps, keymapSize, mod, sortedKeysAfter, &mappedMod, mappedKeys);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(mappedKeys[0], 0x05);
  for (int i = 1; i < KEY_REPORT_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
