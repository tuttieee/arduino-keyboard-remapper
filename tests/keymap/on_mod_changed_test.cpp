#include "gtest/gtest.h"

#include "../../keymap.h"

#include "utils.h"

using namespace keymap;

TEST(onModChanged, ModToModPressed) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}}
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t key = 0;
  uint8_t before = 0;
  uint8_t after;

  uint8_t mappedMod, mappedKey;
  bool isKeyMapped, mappedKeyPressed;

  // The registered source mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  after = MOD_LEFT_SHIFT;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);

  // The target mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  after = MOD_LEFT_CTRL;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, MOD_LEFT_CTRL);

  // Another mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  after = MOD_RIGHT_GUI;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, MOD_RIGHT_GUI);
}

TEST(onModChanged, ModToModReleased) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}}
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t key = 0;
  uint8_t before;
  uint8_t after = 0;

  uint8_t mappedMod, mappedKey;
  bool isKeyMapped, mappedKeyPressed;

  // The registered source mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  before = MOD_LEFT_SHIFT;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, 0);

  // The target mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  before = MOD_LEFT_CTRL;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, 0);

  // Another mod is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  before = MOD_RIGHT_GUI;
  EXPECT_TRUE(
    onModChanged(keymaps, keymapSize, keyPressedFlags, before, after, &mappedMod, &isKeyMapped, &mappedKey, &mappedKeyPressed)
  );
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_FALSE(isKeyMapped);
  EXPECT_EQ(mappedMod, 0);
}
