#include "gtest/gtest.h"

#include "../../keymap.h"

#include "utils.h"

using namespace keymap;

class ModKeyComboToKeyTest : public ::testing::Test {
protected:
  const int keymapSize = 1;
  KeyMap keymaps[1] = {
    {{MOD_LEFT_ALT, 0x2b}, {MOD_LEFT_CTRL, 0}},  // left-and and 0x2b combo (TypeMatrix 2030's 'shuffle' key) to left-ctrl
  };
  KeyPressedFlag keyPressedFlags[1];
};

TEST_F(ModKeyComboToKeyTest, onKeyPressedWithComboAndSuccessiveKey) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);

  // First, the combo is pressed
  uint8_t mod1 = MOD_LEFT_ALT;
  uint8_t key1 = 0x2b;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod1, &key1);
  EXPECT_EQ(mod1, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key1, 0);

  // Second, a printing key is pressed as the combo has been pressed together.
  uint8_t mod2 = MOD_LEFT_ALT;  // The mod key of the combo
  uint8_t key2 = 0x04;  // Printing key
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod2, &key2);
  EXPECT_EQ(mod2, MOD_LEFT_CTRL);  // Mapped mod
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key2, 0x04);
}

TEST_F(ModKeyComboToKeyTest, onKeyPressedWithSingleKey) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);

  // No effect
  uint8_t mod = 0;
  uint8_t key = 0x2b;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x2b);
}

TEST_F(ModKeyComboToKeyTest, onKeyReleasedWithComboAndSuccessiveKey) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);

  // First, the combo is released
  uint8_t mod1 = MOD_LEFT_ALT;
  uint8_t key1 = 0x2b;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod1, &key1);
  EXPECT_EQ(mod1, 0);  // Mapped key is released
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key1, 0);

  // Second, a printing key is released.
  uint8_t mod2 = 0;
  uint8_t key2 = 0x04;  // Printing key
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod2, &key2);
  EXPECT_EQ(mod2, 0);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key2, 0x04);
}

TEST_F(ModKeyComboToKeyTest, onKeyReleasedWithSingleKey) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);

  // No effect
  uint8_t mod = 0;
  uint8_t key = 0x2b;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x2b);
}

TEST_F(ModKeyComboToKeyTest, onModChangedWithMappedMod) {
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);

  // No effect
  uint8_t before = 0;
  uint8_t after = MOD_LEFT_ALT;
  uint8_t mappedMod, mappedKey;
  bool isKeyMapped, mappedKeyPressed;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_FALSE(keyPressedFlags[0]);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, MOD_LEFT_ALT);
}
