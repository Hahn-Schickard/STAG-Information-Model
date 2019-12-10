#include "DeviceElementGroup_MOCK.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AtLeast;

TEST(DeviceElemenGroupTest, CanGetSubelement) {
  MockDeviceElementGroup mock_device_element_group(
      "1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device_element_group, getSubelement).Times(AtLeast(1));
  mock_device_element_group.getSubelement("1234:0");
}

TEST(DeviceElemenGroupTest, CanGetSubelements) {
  MockDeviceElementGroup mock_device_element_group(
      "1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_device_element_group, getSubelements).Times(AtLeast(1));
  mock_device_element_group.getSubelements();
}