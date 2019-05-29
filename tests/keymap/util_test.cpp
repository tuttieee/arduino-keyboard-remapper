#include "gtest/gtest.h"

#include "../../keymap.h"

#include "utils.h"

using namespace keymap;

TEST(isKeyIncluded, TrueTest) {
  uint8_t key = 0x04;
  uint8_t keys[6] = {0x04, 0, 0, 0, 0, 0};

  EXPECT_TRUE(isKeyIncluded(key, keys));
}

TEST(isKeyIncluded, FalseTest) {
  uint8_t key = 0x04;
  uint8_t keys[6] = {0, 0, 0, 0, 0, 0};

  EXPECT_FALSE(isKeyIncluded(key, keys));
}

TEST(keymapMatchedTest, TrueTest) {
  KeyMap keymap = {
    {0x00, {0x04, 0x05}},
    {0x00, 0x00}
  };
  uint8_t keys[6] = {0x04, 0x05, 0, 0, 0, 0};

  EXPECT_TRUE(keymapMatched(keymap, keys));
}

TEST(keymapMatchedTest, PartiallyMatched) {
  KeyMap keymap = {
    {0x00, {0x04, 0x00}},
    {0x00, 0x00}
  };
  uint8_t keys[6] = {0x04, 0x05, 0, 0, 0, 0};

  EXPECT_TRUE(keymapMatched(keymap, keys));
}

TEST(keymapMatchedTest, FalseTest) {
  KeyMap keymap = {
    {0x00, {0x04, 0x05}},
    {0x00, 0x00}
  };
  uint8_t keys[6] = {0x04, 0, 0, 0, 0, 0};

  EXPECT_FALSE(keymapMatched(keymap, keys));
}

TEST(removeKeysTest, WhenkeymapMatched) {
  KeyMap keymap = {
    {0x00, {0x04, 0x05}},
    {0x00, 0x00}
  };
  uint8_t keys[6] = {0x04, 0x05, 0, 0, 0, 0};

  removeKeys(keymap, keys);
  for (uint8_t i = 0; i < 6; ++i) {
    EXPECT_EQ(keys[i], 0);
  }
}

TEST(removeKeysTest, WhenNotkeymapMatched) {
  KeyMap keymap = {
    {0x00, {0x04, 0x05}},
    {0x00, 0x00}
  };
  uint8_t keys[6] = {0x04, 0x05, 0x06, 0, 0, 0};

  removeKeys(keymap, keys);

  EXPECT_EQ(keys[0], 0x06);
  for (uint8_t i = 1; i < 6; ++i) {
    EXPECT_EQ(keys[i], 0);
  }
}
