#include "gtest/gtest.h"

#include "../../keymap.h"

#include "utils.h"

using namespace keymap;

TEST(onKeyPressedTest, SingleKeyToKey) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod = 0x00000000;  // No modifier key pressed;
  uint8_t key;

  // The registered source key pressed;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  key = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key, 0x05);  // Key remapped

  // No effect when the target key pressed;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  key = 0x05;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x05);

  // No effect when another key pressed;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  key = 0x0a;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x0a);
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

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  key = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(keyPressedFlags[1], false);
  EXPECT_EQ(key, 0x05);

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  key = 0x05;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(keyPressedFlags[1], true);
  EXPECT_EQ(key, 0x04);
}

TEST(onKeyPressedTest, SingleModToModMapping) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod;
  uint8_t key = 0x04;

  // The source modifier is pressed;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_LEFT_SHIFT;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key, 0x04);

  // The target modifier is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_LEFT_CTRL;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x04);

  // Another modifier is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_RIGHT_GUI;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_RIGHT_GUI);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x04);
}

// TODO: Parameterize the order of keymaps
TEST(onKeyPressedTest, ModKeyCombination) {
  const int keymapSize = 2;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_ALT, 0x04}, {MOD_LEFT_CTRL, 0}},  // from mod-key combo
    {{0, 0x04}, {0, 0x05}},  // another keymap with same key without mod
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  // When the key is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  uint8_t mod1 = 0;
  uint8_t key1 = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod1, &key1);
  EXPECT_EQ(mod1, 0);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(keyPressedFlags[1], true);
  EXPECT_EQ(key1, 0x05);

  // When the mod and key are pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  uint8_t mod2 = MOD_LEFT_ALT;
  uint8_t key2 = 0x04;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod2, &key2);
  EXPECT_EQ(mod2, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(keyPressedFlags[1], false);
  EXPECT_EQ(key2, 0);
}

TEST(onKeyReleasedTest, SingleKeyToKey) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod = 0x00000000;  // No modifier key pressed;
  uint8_t key;

  // The registered source key released;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  key = 0x04;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x05);  // Key remapped

  // No effect when the target key released;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  key = 0x05;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key, 0x05);

  // No effect when another key released;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  key = 0x0a;
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key, 0x0a);
}

TEST(onKeyReleasedTest, SwappingTwoKeys) {
  const int keymapSize = 2;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x04}, {0, 0x05}},
    {{0, 0x05}, {0, 0x04}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod = 0x00000000;
  uint8_t key;

  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  key = 0x04;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(keyPressedFlags[1], true);
  EXPECT_EQ(key, 0x05);

  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  key = 0x05;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, 0x00000000);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(keyPressedFlags[1], false);
  EXPECT_EQ(key, 0x04);
}

TEST(onKeyReleasedTest, SingleModToModMapping) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_SHIFT, 0}, {MOD_LEFT_CTRL, 0}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  uint8_t mod;
  uint8_t key = 0x04;

  // The source modifier is pressed;
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_LEFT_SHIFT;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key, 0x04);

  // The target modifier is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_LEFT_CTRL;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x04);

  // Another modifier is pressed
  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  mod = MOD_RIGHT_GUI;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod, &key);
  EXPECT_EQ(mod, MOD_RIGHT_GUI);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key, 0x04);
}

// TODO: Parameterize the order of keymaps
TEST(onKeyReleasedTest, ModKeyCombination) {
  const int keymapSize = 2;
  KeyMap keymaps[keymapSize] = {
    {{MOD_LEFT_ALT, 0x04}, {MOD_LEFT_CTRL, 0}},  // from mod-key combo
    {{0, 0x04}, {0, 0x05}},  // another keymap with same key without mod
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  // When the key is released
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  uint8_t mod1 = 0;
  uint8_t key1 = 0x04;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod1, &key1);
  EXPECT_EQ(mod1, 0);
  EXPECT_EQ(keyPressedFlags[0], false);  // If 0x04 key is released, the combination of alt-left and 0x04 key is also released.
  EXPECT_EQ(keyPressedFlags[1], false);
  EXPECT_EQ(key1, 0x05);

  // When the mod and key are released
  clearKeyPressedFlags(keyPressedFlags, keymapSize, true);
  uint8_t mod2 = MOD_LEFT_ALT;
  uint8_t key2 = 0x04;
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod2, &key2);
  EXPECT_EQ(mod2, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(keyPressedFlags[1], false);  // If the combination of left-alt and 0x04 key are released, the single 0x04 key is also released since onKeyReleased is triggered by releasing the key, but not mod.
  EXPECT_EQ(key2, 0);
}

TEST(onKeyPressedReleasedTest, CapsToCtrl) {
  const int keymapSize = 1;
  KeyMap keymaps[keymapSize] = {
    {{0, 0x39}, {MOD_LEFT_CTRL, 0}},
  };
  KeyPressedFlag keyPressedFlags[keymapSize];

  clearKeyPressedFlags(keyPressedFlags, keymapSize, false);
  // First, the source key is presse
  uint8_t mod1 = 0;
  uint8_t key1 = 0x39;  // CapsLock
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod1, &key1);
  EXPECT_EQ(mod1, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key1, 0x00);

  // Second, a printing key is pressed
  uint8_t mod2 = 0;
  uint8_t key2 = 0x04;  // 'a'
  onKeyPressed(keymaps, keymapSize, keyPressedFlags, &mod2, &key2);
  EXPECT_EQ(mod2, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key2, 0x04);

  // Then, release
  uint8_t mod3 = 0;
  uint8_t key3 = 0x04;  // 'a'
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod3, &key3);
  EXPECT_EQ(mod3, MOD_LEFT_CTRL);
  EXPECT_EQ(keyPressedFlags[0], true);
  EXPECT_EQ(key3, 0x04);

  uint8_t mod4 = 0;
  uint8_t key4 = 0x39;  // CapsLock
  onKeyReleased(keymaps, keymapSize, keyPressedFlags, &mod4, &key4);
  EXPECT_EQ(mod4, 0);
  EXPECT_EQ(keyPressedFlags[0], false);
  EXPECT_EQ(key4, 0x00);
}
