#ifndef __DEVICE_ELEMENT_GROUP_MOCK_HPP
#define __DEVICE_ELEMENT_GROUP_MOCK_HPP

#include "DeviceElementGroup.hpp"

#include "gmock/gmock.h"

namespace Information_Model {
  namespace testing {
    /**
   * @brief DeviceElementGroup mock , use for testing only! 
   * Based on Google Mocks framework
   * 
   */
    class MockDeviceElementGroup : public DeviceElementGroup {
     public:
      MockDeviceElementGroup(const std::string& REF_ID,
          const std::string& NAME,
          const std::string& DESC)
          : DeviceElementGroup(REF_ID, NAME, DESC) {}
      MOCK_METHOD(std::vector<std::shared_ptr<DeviceElement>>,
          getSubelements,
          (),
          (override));
      MOCK_METHOD(std::shared_ptr<DeviceElement>,
          getSubelement,
          (const std::string& REF_ID),
          (override));
    };
  }   // namespace testing
}   // namespace Information_Model

#endif   //__DEVICE_ELEMENT_GROUP_MOCK_HPP