#ifndef _FAKE_DEVICE_ELEMENT_HPP
#define _FAKE_DEVICE_ELEMENT_HPP

#include "DeviceElement.hpp"

#include <string>

namespace Information_Model {
  namespace testing {
    /**
   * @brief Device Element fake implementation. Use only for testing!
   * 
   */
    class FakeDeviceElement : public DeviceElement {
     public:
      FakeDeviceElement(const std::string REF_ID,
          const std::string NAME,
          const std::string DESC,
          ElementType type)
          : DeviceElement(REF_ID, NAME, DESC, type) {}
    };
  }   // namespace testing
}   // namespace Information_Model

#endif   //_FAKE_DEVICE_ELEMENT_HPP