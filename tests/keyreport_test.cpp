#include "gtest/gtest.h"

#include "../keyreport.h"

using namespace std;
using namespace keyreport;

TEST(keyreport, setKeyPressed_and_setKeyReleased) {
  KeyReport keyReport;

  releaseAllKey(&keyReport);
  for (int i=0; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0);
  }

  setKeyPressed(&keyReport, 0, 0x40);
  ASSERT_EQ(keyReport.keys[0], 0x40);
  for (int i=1; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0x00);
  }
  ASSERT_EQ(keyReport.modifiers, 0x00);

  // Same key again
  setKeyPressed(&keyReport, 0, 0x40);
  ASSERT_EQ(keyReport.keys[0], 0x40);
  for (int i=1; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0x00);
  }
  ASSERT_EQ(keyReport.modifiers, 0x00);

  // Another key
  setKeyPressed(&keyReport, 0, 0x41);
  ASSERT_EQ(keyReport.keys[0], 0x40);
  ASSERT_EQ(keyReport.keys[1], 0x41);
  for (int i=2; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0x00);
  }
  ASSERT_EQ(keyReport.modifiers, 0x00);

  setKeyReleased(&keyReport, 0, 0x40);
  ASSERT_EQ(keyReport.keys[0], 0x00);
  ASSERT_EQ(keyReport.keys[1], 0x41);
  for (int i=2; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0x00);
  }
  ASSERT_EQ(keyReport.modifiers, 0x00);

  setKeyReleased(&keyReport, 0, 0x41);
  for (int i=0; i<KEY_REPORT_KEYS_NUM; i++) {
    ASSERT_EQ(keyReport.keys[i], 0x00);
  }
  ASSERT_EQ(keyReport.modifiers, 0x00);
}
