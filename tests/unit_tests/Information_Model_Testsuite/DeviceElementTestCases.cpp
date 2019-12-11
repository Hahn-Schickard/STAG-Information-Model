#include "DeviceElement_FAKE.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;

TEST(DeviceElementTest, DeviceElementTypeUndefIsCorrect) {
  // Arrange
  DeviceElement* element = new FakeDeviceElement("1234",
      "TestDevice",
      "A hardcoded deviceelement",
      ElementType::UNDEFINED);
  // Act
  ElementType tested_element = element->getElementType();
  // Assert
  ElementType expecte_result = ElementType::UNDEFINED;
  EXPECT_EQ(expecte_result, tested_element)
      << "expected: " << expecte_result << endl
      << "provided: " << tested_element << endl;
}