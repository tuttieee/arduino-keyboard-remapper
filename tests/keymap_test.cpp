#include "gtest/gtest.h"

#include "../keymap.h"

using namespace keymap;

TEST(onKeyPressedTest, SingleKeyToKey) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod = 0x00000000;  // No modifier key pressed;
  uint8_t key = 0x04;  // The registered source key pressed;

  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);

  ASSERT_EQ(mod, 0x00000000);
  ASSERT_EQ(key, 0x05);  // Key remapped
}

TEST(onKeyPressedTest, SwappingTwoKeys) {
  const int keymapSize = 2;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
    {{0, 0x05}, {0, 0x04}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod = 0x00000000;
  uint8_t key;

  key = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(key, 0x05);

  key = 0x05;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(key, 0x04);
}
