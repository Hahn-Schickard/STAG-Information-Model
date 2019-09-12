#include "gtest/gtest.h"
#include "DeviceTestCases.hpp"
#include "DeviceElementTestCases.hpp"
#include "DeviceElementGroupTestCases.hpp"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
