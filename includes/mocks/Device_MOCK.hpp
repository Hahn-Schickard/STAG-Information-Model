#ifndef __DEVICE_MOCK_HPP
#define __DEVICE_MOCK_HPP

#include "Device.hpp"

#include <gmock/gmock.h>

namespace Information_Model {
/**
 * @brief elements within this namespace are suppose to be only used by test
 * cases.
 *
 */
namespace testing {
/**
 * @brief Device class mock, use for testing only!
 *
 */
class MockDevice : public Device {
public:
  MockDevice(const std::string &REF_ID, const std::string &NAME,
             const std::string &DESC)
      : Device(REF_ID, NAME, DESC) {}

  MOCK_METHOD0(getDeviceElementGroup, std::shared_ptr<DeviceElementGroup>());

  MOCK_METHOD1(getDeviceElement,
               std::shared_ptr<DeviceElement>(const std::string &ref_id));
};
} // namespace testing
} // namespace Information_Model
#endif //__DEVICE_MOCK_HPP
