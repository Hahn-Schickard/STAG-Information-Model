#ifndef __DEVICE_ELEMENT_GROUP_MOCK_HPP
#define __DEVICE_ELEMENT_GROUP_MOCK_HPP

#include "DeviceElementGroup.hpp"

#include "gmock/gmock.h"

class MockDeviceElementGroup : public Information_Model::DeviceElementGroup {
 public:
  MockDeviceElementGroup(const std::string REF_ID,
      const std::string NAME,
      const std::string DESC)
      : Information_Model::DeviceElementGroup(REF_ID, NAME, DESC) {}
  MOCK_METHOD(std::vector<std::shared_ptr<DeviceElement>>,
      getSubelements,
      (),
      (override));
  MOCK_METHOD(std::shared_ptr<Information_Model::DeviceElement>,
      getSubelement,
      (const std::string REF_ID),
      (override));
};

#endif   //__DEVICE_ELEMENT_GROUP_MOCK_HPP