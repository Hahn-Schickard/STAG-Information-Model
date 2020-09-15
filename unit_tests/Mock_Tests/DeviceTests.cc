#include "Device_MOCK.hpp"

#include "gtest/gtest.h"

#include <memory>

using namespace std;
using namespace Information_Model;
using namespace Information_Model::testing;
using ::testing::AtLeast;

TEST(DeviceTests, canCallgetDeviceElementGroup) {
  MockDevice test_device("1234", "Test Device", "Lorem Ipsum");
  EXPECT_CALL(test_device, getDeviceElementGroup).Times(AtLeast(1));
  test_device.getDeviceElementGroup();
}
