#include "Device_MOCK.hpp"

#include "gtest/gtest.h"

#include <memory>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;
using ::testing::AtLeast;

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupIDIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementId();
  // Assert
  std::string expected_result = "1234";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupIDIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementId();
  // Assert
  std::string expected_result = "1235:";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupNameIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementName();
  // Assert
  std::string expected_result = "TestDevice";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupNameIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementName();
  // Assert
  std::string expected_result = "TestDevice2";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupDescriptionIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementDescription();
  // Assert
  std::string expected_result = "A hardcoded deviceelement";
  EXPECT_STREQ(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, DeviceElementGroupDescriptionIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string tested_element = mock_device.getElementDescription();
  // Assert
  std::string expected_result = "A hardcoded deviceelement2.";
  EXPECT_STRNE(expected_result.c_str(), tested_element.c_str())
      << "expected: " << expected_result << std::endl
      << "provided: " << tested_element << std::endl;
}

// NOLINTNEXTLINE
TEST(DeviceTests, canGetDeviceElementGroup) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device, getDeviceElementGroup).Times(AtLeast(1));
  mock_device.getDeviceElementGroup();
}
