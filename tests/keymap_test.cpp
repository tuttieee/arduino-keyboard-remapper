#include "gtest/gtest.h"

#include "../keymap.h"

using namespace keymap;

#define EXPECT_ARR_EQ(val1, val2, size) for (int i=0; i<size; i++) EXPECT_EQ(val1[i], val2[i])

void clearKeyPressedFlags(KeyPressedFlag keyPressedFlags[], int size) {
  for (int i=0; i < size; i++) {
    keyPressedFlags[i] = false;
  }
}

TEST(onKeyPressedTest, SingleKeyToKey) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize] = {false};

  uint8_t mod = 0x00000000;  // No modifier key pressed;
  uint8_t key = 0x04;  // The registered source key pressed;

  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);

  EXPECT_EQ(mod, 0x00000000);
  KeyPressedFlag expectedKeyPressedFlags[] = {true};
  EXPECT_ARR_EQ(keyPressedFlags, expectedKeyPressedFlags, keymapSize);
  EXPECT_EQ(key, 0x05);  // Key remapped
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

  clearKeyPressedFlags(keyPressedFlags, keymapSize);
  key = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  KeyPressedFlag expectedKeyPressedFlags1[] = {true, false};
  EXPECT_ARR_EQ(keyPressedFlags, expectedKeyPressedFlags1, keymapSize);
  EXPECT_EQ(key, 0x05);

  clearKeyPressedFlags(keyPressedFlags, keymapSize);
  key = 0x05;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  KeyPressedFlag expectedKeyPressedFlags2[] = {false, true};
  EXPECT_ARR_EQ(keyPressedFlags, expectedKeyPressedFlags2, keymapSize);
  EXPECT_EQ(key, 0x04);
}
