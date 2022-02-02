#ifndef __INFORMATION_MODEL_DEVICE_MOCK_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_HPP

#include "../Device.hpp"
#include "DeviceElementGroup_MOCK.hpp"

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
  NonemptyDeviceElementGroupPtr base_group_;

public:
  MockDevice(const std::string &ref_id, const std::string &name,
             const std::string &desc)
      : Device(ref_id, name, desc),
        base_group_(
            std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(
                ref_id + ":")) {
    ON_CALL(*this, getDeviceElementGroup)
        .WillByDefault(
            [this]() -> NonemptyDeviceElementGroupPtr {
              return NonemptyDeviceElementGroupPtr(base_group_);
            });

    ON_CALL(*this, getDeviceElement)
        .WillByDefault([this](const std::string &ref_id) -> DeviceElementPtr {
          return base_group_->getSubelement(ref_id);
        });
  }

  MOCK_METHOD(
    NonemptyDeviceElementGroupPtr, getDeviceElementGroup, (), (override));

  MOCK_METHOD(DeviceElementPtr, getDeviceElement,
              (const std::string & /*ref_id*/), (override));
};

using MockDevicePtr = std::shared_ptr<MockDevice>;
} // namespace testing
} // namespace Information_Model
#endif //__INFORMATION_MODEL_DEVICE_MOCK_HPP
