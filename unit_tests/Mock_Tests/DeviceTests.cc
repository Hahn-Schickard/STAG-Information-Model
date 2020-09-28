#include "Device_MOCK.hpp"

#include "gtest/gtest.h"

#include <memory>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;
using ::testing::AtLeast;

TEST(DeviceTests, DeviceElementGroupIDIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementId();
  // Assert
  std::string expectedResult = "1234";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, DeviceElementGroupIDIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementId();
  // Assert
  std::string expectedResult = "1235:";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, DeviceElementGroupNameIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementName();
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, DeviceElementGroupNameIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementName();
  // Assert
  std::string expectedResult = "TestDevice2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, DeviceElementGroupDescriptionIsCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementDescription();
  // Assert
  std::string expectedResult = "A hardcoded deviceelement";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, DeviceElementGroupDescriptionIsNotCorrect) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  // Act
  std::string testedElement = mock_device.getElementDescription();
  // Assert
  std::string expectedResult = "A hardcoded deviceelement2.";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(DeviceTests, canGetDeviceElementGroup) {
  MockDevice mock_device("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device, getDeviceElementGroup).Times(AtLeast(1));
  mock_device.getDeviceElementGroup();
}

TEST(DeviceTestss, canCallgetDeviceElementGroup) {
  MockDevice test_device("1234", "Test Device", "Lorem Ipsum");
  EXPECT_CALL(test_device, getDeviceElementGroup).Times(AtLeast(1));
  test_device.getDeviceElementGroup();
}
