#include <iostream>

#include "keyreport.h"

using namespace std;
using namespace keyreport;

int run_tests() {  // TODO: Refactoring by some unit testing framework
  KeyReport keyReport;
  releaseAllKey(&keyReport);

  for (int i=0; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }

  setKeyPressed(&keyReport, 0, 0x40);
  if (keyReport.keys[0] != 0x40) {
    cout << "keyReport.keys[0] != 0x40";
    return 1;
  }
  for (int i=1; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }
  if (keyReport.modifiers != 0) {
    cout << "keyReport.modifiers != 0";
    return 1;
  }

  // Same key again
  setKeyPressed(&keyReport, 0, 0x40);
  if (keyReport.keys[0] != 0x40) {
    cout << "keyReport.keys[0] != 0x40";
    return 1;
  }
  for (int i=1; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }
  if (keyReport.modifiers != 0) {
    cout << "keyReport.modifiers != 0";
    return 1;
  }

  // Another key
  setKeyPressed(&keyReport, 0, 0x41);
  if (keyReport.keys[0] != 0x40) {
    cout << "keyReport.keys[0] != 0x40";
    return 1;
  }
  if (keyReport.keys[1] != 0x41) {
    cout << "keyReport.keys[0] != 0x41";
    return 1;
  }
  for (int i=2; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }
  if (keyReport.modifiers != 0) {
    cout << "keyReport.modifiers != 0";
    return 1;
  }

  setKeyReleased(&keyReport, 0, 0x40);
  if (keyReport.keys[0] != 0) {
    cout << "keyReport.keys[0] != 0";
    return 1;
  }
  if (keyReport.keys[1] != 0x41) {
    cout << "keyReport.keys[0] != 0x41";
    return 1;
  }
  for (int i=2; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }
  if (keyReport.modifiers != 0) {
    cout << "keyReport.modifiers != 0";
    return 1;
  }

  setKeyReleased(&keyReport, 0, 0x41);
  for (int i=0; i<KEY_REPORT_KEYS_NUM; i++) {
    if (keyReport.keys[i] != 0) {
      cout << "keyReport.keys[" << i << "] != 0";
      return 1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  return run_tests();
}
