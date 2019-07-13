#ifndef CONSUMERADAPTERTESTS_HPP
#define CONSUMERADAPTERTESTS_HPP

#include "ConsumerAdapterTests.hpp"
#include "blueprint.hpp"
#include "device.hpp"
#include "gtest/gtest.h"
#include <string>
using namespace std;

TEST(ConsumerAdapter, GetDevice_ValidRefId_ReturnsDevice) {
  // Arrange
  Blueprint *bluePrint = new Blueprint("1234", "TestDevice",
                                       "A hardcoded device for testing only");

  // Act
  auto device = bluePrint->GetDevice();
  string refId = device->getReferenceId();

  // Assert
  ASSERT_NE(device, nullptr);
  EXPECT_EQ("1234", refId);
  EXPECT_EQ("TestDevice", device->getElementName());
  EXPECT_EQ("A hardcoded device for testing only",
            device->getElementDescription());
}

TEST(ConsumerAdapter, GetDeviceElementGroup_NoParameter_ReturnsFirstGroup) {
  // Arrange
  Blueprint *bluePrint = new Blueprint();
  Device *device = bluePrint->GetDevice();

  // Act
  DeviceElementGroup *group = device->getDeviceElementGroup();

  // Assert
  ASSERT_NE(group, nullptr);
  EXPECT_EQ("1234.1", group->getReferenceId());
  EXPECT_EQ("TestElementGroup", group->getElementName());
  EXPECT_EQ("A hardcoded device element group", group->getElementDescription());
}

TEST(ConsumerAdapter, GetDeviceElementGroups_ReturnsOneGroup) {
  // Arrange
  Blueprint *bluePrint = new Blueprint();
  Device *device = bluePrint->GetDevice();

  // Act
  vector<DeviceElementGroup> groups = device->getDeviceElementGroups();

  // Assert

  EXPECT_EQ(1, groups.size());

  delete bluePrint;
}

#endif // CONSUMERADAPTERTESTS_HPP