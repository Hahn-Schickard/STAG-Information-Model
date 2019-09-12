#include "gtest/gtest.h"
#include "device_test_cases.hpp"
#include "deviceelement_test_cases.hpp"
#include "deviceelementgroup_test_cases.hpp"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
