#ifndef ELEMENTDESCRIPTIONTEST_HPP
#define ELEMENTDESCRIPTIONTEST_HPP

#include "DeviceElementBuilder.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "DeviceImpl.hpp"
#include "Device.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;
using namespace Information_Model;

TEST(ElementDescriptionTests, DeviceDescriptionIsCorrect) {
  // Arrange
  Device *element =
      new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A hardcoded deviceelement";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementDescriptionTests, DeviceDescriptionIsNotCorrect) {
  // Arrange
  Device *element =
      new DeviceImpl("1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A new hardcoded deviceelement";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementDescriptionTests, DeviceElementDescriptionIsCorrect) {
  // Arrange
  DeviceElement *element = new DeviceElementBuilder(
      "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A hardcoded deviceelement";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementDescriptionTests, DeviceElementDescriptionIsNotCorrect) {
  // Arrange
  DeviceElement *element = new DeviceElementBuilder(
      "1234", "TestDevice", "A hardcoded deviceelement", ElementType::Group);

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A new hardcoded deviceelement";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementDescriptionTests, DeviceElementGroupDescriptionIsCorrect) {
  // Arrange
  DeviceElementGroup *element = new DeviceElementGroupImpl(
      "1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A hardcoded deviceelement";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

TEST(ElementDescriptionTests, DeviceElementGroupDescriptionIsNotCorrect) {
  // Arrange
  DeviceElementGroup *element = new DeviceElementGroupImpl(
      "1234", "TestDevice", "A hardcoded deviceelement");

  // Act
  std::string testedElement = element->getElementDescription();
  // Assert
  std::string expectedResult = "A new hardcoded deviceelement";
  EXPECT_STRNE(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

#endif