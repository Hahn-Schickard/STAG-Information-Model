#ifndef DEVICETEST_HPP
#define DEVICETEST_HPP

#include "DeviceImpl.hpp"
#include "Device.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

class DeviceTest : public ::testing::Test
{
public:
  Device *device;
  DeviceElementGroup *element;

  DeviceTest() {}

  void SetUp()
  {
    device = new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");
    DeviceImpl *tmp_device = dynamic_cast<DeviceImpl *>(device);
    tmp_device->addDeviceElementGroup(
        "TestGroup", "This is a synthetic test for device element group.");
    element = device->getDeviceElementGroup();
  }

  void TearDown() { delete device; }
};

TEST_F(DeviceTest, DeviceElementGroupIDIsCorrect)
{
  // Act
  std::string testedElement = element->getElementRefId();
  // Assert
  std::string expectedResult = "1234:";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST_F(DeviceTest, DeviceElementGroupIDIsNotCorrect)
{
  // Act
  std::string testedElement = element->getElementRefId();
  // Assert
  std::string expectedResult = "1235:";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST_F(DeviceTest, DeviceElementGroupNameIsCorrect)
{
  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestGroup";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST_F(DeviceTest, DeviceElementGroupNameIsNotCorrect)
{
  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestGroup2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST_F(DeviceTest, DeviceElementGroupDescriptionIsCorrect)
{
  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "This is a synthetic test for device element group.";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST_F(DeviceTest, DeviceElementGroupDescriptionIsNotCorrect)
{
  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "This is a synthetic test for device element group2.";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

#endif