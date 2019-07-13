#ifndef BLUEPRINTTESTS_HPP
#define BLUEPRINTTESTS_HPP

#include "BlueprintTests.hpp"
#include "ConsumerAdapterTests.hpp"
#include "blueprint.hpp"
#include "device.hpp"
#include "informationModelExceptions.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(RefIdGenerationTests,
     AddDeviceGroup_DeviceRefIdIs1234_ResultIsNotNullPtr) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");

  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");

  EXPECT_NE(group, nullptr);
}

TEST(RefIdGenerationTests, AddDeviceGroup_DeviceRefIdIs1234_ResultIs1234_1) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");

  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");

  ASSERT_NE(group, nullptr);
  EXPECT_EQ(group->getReferenceId(), "1234.1");
}

TEST(RefIdGenerationTests,
     AddTwoDeviceGroups_DeviceRefIdIs1234_SecondGroupDeviceIdIs1234_2) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddDeviceGroup("TestGroup2", "A Second Test Group");

  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.2");

  ASSERT_NE(group, nullptr);
  EXPECT_EQ(group->getReferenceId(), "1234.2");
}

TEST(RefIdGenerationTests,
     AddDeviceSubGroup_DeviceRefIdIs1234_ResultIs1234_1_1) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",
                           ElementType::Group);

  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");
  DeviceElement *groupElement = device->findSubElement(group, "1234.1.1");

  ASSERT_NE(groupElement, nullptr);
  EXPECT_EQ(groupElement->getReferenceId(), "1234.1.1");
  EXPECT_EQ(groupElement->getElementName(), "TestSubGroup");
  EXPECT_EQ(groupElement->getElementDescription(), "A Test Subgroup");
}

TEST(RefIdGenerationTests,
     AddSecondDeviceSubGroup_DeviceRefIdIs1234_ResultIs1234_1_2) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup",
                           ElementType::Group);
  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");
  DeviceElement *groupElement =
      device->findSubElement((DeviceElement *)group, "1234.1.1");
  DeviceElement *groupElement2 =
      device->findSubElement((DeviceElement *)group, "1234.1.2");

  ASSERT_NE(groupElement, nullptr);
  EXPECT_EQ(groupElement->getReferenceId(), "1234.1.1");
  EXPECT_EQ(groupElement->getElementName(), "TestSubGroup");
  EXPECT_EQ(groupElement->getElementDescription(), "A Test Subgroup");

  ASSERT_NE(groupElement2, nullptr);
  EXPECT_EQ(groupElement2->getReferenceId(), "1234.1.2");
  EXPECT_EQ(groupElement2->getElementName(), "TestSubGroup2");
  EXPECT_EQ(groupElement2->getElementDescription(), "A second Test Subgroup");
}

TEST(RefIdGenerationTests,
     AddElementtoGroup_DeviceRefIdIs1234_ResultIs1234_1_2) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1", "TestElement", "An element to a group",
                           ElementType::Observable);
  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");
  DeviceElement *subElement =
      device->findSubElement((DeviceElement *)group, "1234.1.2");

  ASSERT_NE(subElement, nullptr);
  EXPECT_EQ(subElement->getReferenceId(), "1234.1.2");
  EXPECT_EQ(subElement->getElementName(), "TestElement");
  EXPECT_EQ(subElement->getElementDescription(), "An element to a group");
}

TEST(RefIdGenerationTests,
     AddElementtoGroup_DeviceRefIdIs1234_ResultIs1234_1_3_1) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1", "TestElement", "An element to a group",
                           ElementType::Observable);
  blueprint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1.3", "WriteableElement",
                           "A writeable Element to the second group.",
                           ElementType::Group);
  // Assert
  Device *device = blueprint->GetDevice();
  DeviceElementGroup *group = device->getDeviceElementGroup("1234.1");
  DeviceElement *subElement = device->findSubElement(group, "1234.1.3.1");

  ASSERT_NE(subElement, nullptr);
  EXPECT_EQ(subElement->getReferenceId(), "1234.1.3.1");
  EXPECT_EQ(subElement->getElementName(), "WriteableElement");
  EXPECT_EQ(subElement->getElementDescription(),
            "A writeable Element to the second group.");
}

TEST(RefIdGenerationTests, AddElementToAnotherElement_ThrowsException) {
  // Arrange
  Blueprint *blueprint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  blueprint->AddDeviceGroup("TestGroup", "A Test Group");
  blueprint->AddSubElement("1234.1", "TestSubGroup", "A Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1", "TestElement", "An element to a group",
                           ElementType::Observable);
  blueprint->AddSubElement("1234.1", "TestSubGroup2", "A second Test Subgroup",
                           ElementType::Group);
  blueprint->AddSubElement("1234.1.3", "WriteableElement",
                           "A writeable Element to the second group.",
                           ElementType::Writable);

  // Act and Assert
  EXPECT_THROW(
      {
        blueprint->AddSubElement("1234.1.3.1", "ObservableElement",
                                 "A observable element",
                                 ElementType::Observable);
      },
      ElementTypeMismatchException);
}
#endif // BLUEPRINTTESTS_HPP