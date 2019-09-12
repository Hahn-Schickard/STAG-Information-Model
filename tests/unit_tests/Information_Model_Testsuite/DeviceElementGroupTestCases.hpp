#ifndef __DEVICEELEMENTGROUP_TEST_CASES_HPP
#define __DEVICEELEMENTGROUP_TEST_CASES_HPP

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DeviceElementGroupMock.hpp"


using ::testing::AtLeast;

TEST(DeviceElemenGroupTest, CanGetSubelement) {
  MockDeviceElementGroup mock_deviceelementgroup("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_deviceelementgroup, getSubelement).Times(AtLeast(1));
  mock_deviceelementgroup.getSubelement("1234:0");
}

TEST(DeviceElemenGroupTest, CanGetSubelements) {
  MockDeviceElementGroup mock_deviceelementgroup("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_deviceelementgroup, getSubelements).Times(AtLeast(1));
  mock_deviceelementgroup.getSubelements();
}

TEST(DeviceElemenGroupTest, CanGetNumericElementId) {
  MockDeviceElementGroup mock_deviceelementgroup("1234", "TestDevice", "A hardcoded deviceelement");
  EXPECT_CALL(mock_deviceelementgroup, getNumericElementId).Times(AtLeast(1));
  mock_deviceelementgroup.getNumericElementId();
}
#endif //__DEVICEELEMENTGROUP_TEST_CASES_HPP