#ifndef __DEVICE_TEST_CASES_HPP
#define __DEVICE_TEST_CASES_HPP

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "device_mock.hpp"

#include <stdexcept>

using ::testing::AtLeast;

class DeviceTest : public ::testing::Test {
public:
  /*void SetUp() override {
    mock_device = MockDevice("1234", "TestDevice", "A hardcoded deviceelement");
    // device = new DeviceImpl("1234", "TestDevice", "A hardcoded
    // deviceelement");
    // DeviceImpl *tmp_device = dynamic_cast<DeviceImpl *>(device);
    // tmp_device->addDeviceElementGroup(
    //    "TestGroup", "This is a synthetic test for device element group.");
    // element = device->getDeviceElementGroup();
    // test_device =
    //    new MockDevice("1234", "TestDevice", "A hardcoded deviceelement");
  }*/
};

TEST(DeviceTest, DeviceElementGroupIDIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementRefId();
  // Assert
  std::string expectedResult = "1234:";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

/*TEST(DeviceTest, DeviceElementGroupIDIsNotCorrect) {
  // Act
  std::string testedElement = element->getElementRefId();
  // Assert
  std::string expectedResult = "1235:";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsCorrect) {
  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestGroup";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsNotCorrect) {
  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestGroup2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsCorrect) {
  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult =
      "This is a synthetic test for device element group.";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsNotCorrect) {
  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult =
      "This is a synthetic test for device element group2.";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}*/

TEST(DeviceTest, CanGetDeviceElementGroup) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device, getDeviceElementGroup).Times(AtLeast(1));
  mock_device.getDeviceElementGroup();
}
#endif //__DEVICE_TEST_CASES_HPP