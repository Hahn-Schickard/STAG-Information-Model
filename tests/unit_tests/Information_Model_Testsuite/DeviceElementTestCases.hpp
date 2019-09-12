#ifndef __DEVICEELEMENT_TEST_CASES_HPP
#define __DEVICEELEMENT_TEST_CASES_HPP

#include "gtest/gtest.h"

#include "DeviceElement.hpp"

TEST(DeviceElementTest, DeviceElementTypeUndefIsCorrect) {

  // Arrange
  Information_Model::DeviceElement *element =
      new Information_Model::DeviceElement(
          "1234", "TestDevice", "A hardcoded deviceelement",
          Information_Model::ElementType::Undefined);
  // Act
  Information_Model::ElementType testedElement = element->getElementType();
  // Assert
  Information_Model::ElementType expectedResult =
      Information_Model::ElementType::Undefined;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << std::endl
      << "provided: " << testedElement << std::endl;
}
#endif //__DEVICEELEMENT_TEST_CASES_HPP