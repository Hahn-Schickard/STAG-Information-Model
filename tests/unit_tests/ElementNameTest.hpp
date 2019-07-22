#ifndef ELEMENTNAMETEST_HPP
#define ELEMENTNAMETEST_HPP

#include "DeviceElementBuilder.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "DeviceImpl.hpp"
#include "Device.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

TEST(ElementNameTests, DeviceNameIsCorrect) {
  // Arrange
  Device *element =
      new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementNameTests, DeviceNameIsNotCorrect) {
  // Arrange
  Device *element =
      new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementNameTests, DeviceElementNameIsCorrect) {
  // Arrange
  DeviceElement *element = new DeviceElementBuilder(
      "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementNameTests, DeviceElementNameIsNotCorrect) {
  // Arrange
  DeviceElement *element = new DeviceElementBuilder(
      "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementNameTests, DeviceElementGroupNameIsCorrect) {
  // Arrange
  DeviceElementGroup *element = new DeviceElementGroupImpl(
      "1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementNameTests, DeviceElementGroupNameIsNotCorrect) {
  // Arrange
  DeviceElementGroup *element = new DeviceElementGroupImpl(
      "1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementName();
  // Assert
  std::string expectedResult = "TestDevice2";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

#endif