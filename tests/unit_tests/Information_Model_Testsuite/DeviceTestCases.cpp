#include "Device_MOCK.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AtLeast;

TEST(DeviceTest, DeviceElementGroupIDIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementRefId();
  // Assert
  std::string expected_result = "1234";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, DeviceElementGroupIDIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementRefId();
  // Assert
  std::string expected_result = "1235:";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementName();
  // Assert
  std::string expected_result = "TestDevice";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, DeviceElementGroupNameIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementName();
  // Assert
  std::string expected_result = "TestDevice2";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementDescription();
  // Assert
  std::string expected_result = "A hardcoded deviceelement";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, DeviceElementGroupDescriptionIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementDescription();
  // Assert
  std::string expected_result = "A hardcoded deviceelement2.";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

TEST(DeviceTest, CanGetDeviceElementGroup) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device, getDeviceElementGroup).Times(AtLeast(1));
  mock_device.getDeviceElementGroup();
}