#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Device_MOCK.hpp"


using ::testing::AtLeast;

TEST(DeviceTest, DeviceElementGroupIDIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementId();
  // Assert
  std::string expectedResult = "1234";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupIDIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementId();
  // Assert
  std::string expectedResult = "1235:";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementName();
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementName();
  // Assert
  std::string expectedResult = "TestDevice2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementDescription();
  // Assert
  std::string expectedResult =
      "A hardcoded deviceelement";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementDescription();
  // Assert
  std::string expectedResult =
      "A hardcoded deviceelement2.";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTest, CanGetDeviceElementGroup) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device, getDeviceElementGroup).Times(AtLeast(1));
  mock_device.getDeviceElementGroup();
}