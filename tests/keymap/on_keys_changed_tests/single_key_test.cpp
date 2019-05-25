#include "gtest/gtest.h"

#include "../../../keymap.h"

#include "../utils.h"

using namespace keymap;

class SingleKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 1;
  KeyMap keymaps[1] = {
    {{0, {0x04, 0}}, {0, 0x05}},  // 'a' to 'b'
  };
  KeyPressedFlag keyPressedFlags[1];
};

TEST_F(SingleKeyTest, NoKeyPressed) {
  bool isMappedModChanged;
  uint8_t mappedMod = 0;
  uint8_t pressedKeys[MAPPED_KEYS_NUM];
  uint8_t releasedKeys[MAPPED_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeys[6] = {0, 0, 0, 0, 0, 0};

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeys, &isMappedModChanged, &mappedMod, pressedKeys, releasedKeys);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedMod, 0);
  for (int i = 0; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(pressedKeys[i], 0);
  }
  for (int i = 0; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(releasedKeys[i], 0);
  }
}

TEST_F(SingleKeyTest, SingleKeyToKey) {
  bool isMappedModChanged;
  uint8_t mappedMod = 0;
  uint8_t pressedKeys[MAPPED_KEYS_NUM];
  uint8_t releasedKeys[MAPPED_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeys[6] = {0x04, 0, 0, 0, 0, 0};

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeys, &isMappedModChanged, &mappedMod, pressedKeys, releasedKeys);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(pressedKeys[0], 0x05);
  for (int i = 1; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(pressedKeys[i], 0);
  }
  for (int i = 0; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(releasedKeys[i], 0);
  }
}
