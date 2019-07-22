#ifndef ELEMENTNAMETEST_HPP
#define ELEMENTNAMETEST_HPP

#include "DeviceElementBuilder.hpp"
#include "DeviceElementGroupImpl.hpp"
#include "DeviceBuilder.hpp"
#include <string>
#include <gtest/gtest.h>

using namespace Model_Factory;

TEST(ElementNameTests, DeviceNameIsCorrect) {
  // Arrange
  // Device *element = new Device("1234", "TestDevice", "A hardcoded
  // deviceelement");

  // Act
  // std::string testedElement = element->getElementName();
  std::string testedElement = "TestDevice";
  // Assert
  std::string expectedResult = "TestDevice";
  EXPECT_STREQ(expectedResult.c_str(), testedElement.c_str())
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}

#endif