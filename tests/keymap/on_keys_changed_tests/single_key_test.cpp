#include "gtest/gtest.h"

#include "../../../keymap.h"

#include "../utils.h"

using namespace keymap;

class SingleKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 2;
  KeyMap keymaps[2] = {
    {{0, {0x04, 0}}, {0, 0x05}},  // 'a' to 'b'
    {{0, {0x05, 0}}, {0, 0x04}},  // 'b' to 'a'
  };
  KeyPressedFlag keyPressedFlags[1];
};

TEST_F(SingleKeyTest, NoKeyPressed) {
  bool isMappedModChanged;
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[MAPPED_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysBefore[6] = {0, 0, 0, 0, 0, 0};
  uint8_t sortedKeysAfter[6] = {0, 0, 0, 0, 0, 0};

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysBefore, sortedKeysAfter, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedMod, 0);
  for (int i = 0; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(SingleKeyTest, SingleKeyPressedButNoMapMatched) {
  bool isMappedModChanged;
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[MAPPED_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysBefore[6] = {0, 0, 0, 0, 0, 0};
  uint8_t sortedKeysAfter[6] = {0x06, 0, 0, 0, 0, 0};

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysBefore, sortedKeysAfter, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(mappedKeys[0], 0x06);
  for (int i = 1; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}

TEST_F(SingleKeyTest, SingleKeyToKey) {
  bool isMappedModChanged;
  uint8_t mappedMod = 0;
  uint8_t mappedKeys[MAPPED_KEYS_NUM];

  uint8_t mod = 0;
  uint8_t sortedKeysBefore[6] = {0, 0, 0, 0, 0, 0};
  uint8_t sortedKeysAfter[6] = {0x04, 0, 0, 0, 0, 0};

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  onKeysChanged(keymaps, keymapSize, keyPressedFlags, mod, sortedKeysBefore, sortedKeysAfter, &isMappedModChanged, &mappedMod, mappedKeys);
  EXPECT_EQ(isMappedModChanged, false);
  EXPECT_EQ(mappedMod, 0);
  EXPECT_EQ(mappedKeys[0], 0x05);
  for (int i = 1; i < MAPPED_KEYS_NUM; i++) {
    EXPECT_EQ(mappedKeys[i], 0);
  }
}
