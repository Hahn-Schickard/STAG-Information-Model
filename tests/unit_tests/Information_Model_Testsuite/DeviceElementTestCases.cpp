#include "DeviceElement_FAKE.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;

TEST(DeviceElementTest, DeviceElementTypeUndefIsCorrect) {
  // Arrange
  DeviceElement* element = new FakeDeviceElement("1234",
      "TestDevice",
      "A hardcoded deviceelement",
      ElementType::Undefined);
  // Act
  ElementType tested_element = element->getElementType();
  // Assert
  ElementType expecte_result = ElementType::Undefined;
  EXPECT_EQ(expecte_result, tested_element)
      << "expected: " << expecte_result << std::endl
      << "provided: " << tested_element << std::endl;
}