#ifndef DEVICETEST_HPP
#define DEVICETEST_HPP

#include "DeviceImpl.hpp"
#include "Device.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

class DeviceTest : public ::testing::Test {
public:
  Device *device;
  DeviceElementGroup *element;

  DeviceTest() {}

  void SetUp() {
    device = new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");
    element = device->getDeviceElementGroup();
  }

  void TearDown() {
    int i = 0; // delete device;
  }
};

TEST_F(DeviceTest, DeviceElementGroupIDIsCorrect) {
  // Act
  std::string testedElement = element->getElementRefId();
  // Assert
  std::string expectedResult = "1234";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

#endif