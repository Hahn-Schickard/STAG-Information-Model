#ifndef __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP

#include "DeviceBuilderInterface.hpp"
#include "DeviceElementGroup_MOCK.hpp"
#include "Device_MOCK.hpp"

#include <gmock/gmock.h>
#include <optional>
#include <stdexcept>

namespace Information_Model {
namespace testing {
template <class T> T setCallback(std::optional<T> optional_value) {
  if (optional_value.has_value()) {
    return optional_value.value();
  } else {
    return T();
  }
}

/**
 * @brief DeviceMockBuilder builds Device Mock instances.
 * This class should be used by Data Consumer Adapter implementations to build a
 * mock Device instance during unit and integration test stages
 *
 */
class DeviceMockBuilder : public DeviceBuilderInterface {
  MockDevicePtr device_;

public:
  void buildDeviceBase(const std::string &unique_id, const std::string &name,
                       const std::string &desc) {
    device_ = std::make_shared<MockDevice>(unique_id, name, desc);
  }

  std::string addDeviceElementGroup(const std::string &name,
                                    const std::string &desc) {
    return addDeviceElement(std::string(), name, desc, ElementType::GROUP,
                            DataType::UNKNOWN, std::nullopt, std::nullopt);
  }

  std::string addDeviceElementGroup(const std::string &group_refid,
                                    const std::string &name,
                                    const std::string &desc) {
    return addDeviceElement(group_refid, name, desc, ElementType::GROUP,
                            DataType::UNKNOWN, std::nullopt, std::nullopt);
  }

  std::string addReadableMetric(const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb) {
    return addDeviceElement(std::string(), name, desc, ElementType::READABLE,
                            data_type, move(read_cb), std::nullopt);
  }

  std::string addReadableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb) {
    return addDeviceElement(group_refid, name, desc, ElementType::READABLE,
                            data_type, move(read_cb), std::nullopt);
  }

  std::string addWritableMetric(const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb, WriteFunctor write_cb) {
    return addDeviceElement(std::string(), name, desc, ElementType::WRITABLE,
                            data_type, move(read_cb), move(write_cb));
  }

  std::string addWritableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb, WriteFunctor write_cb) {
    return addDeviceElement(group_refid, name, desc, ElementType::WRITABLE,
                            data_type, move(read_cb), move(write_cb));
  }

  MockDeviceElementGroupPtr getGroupImplementation(const std::string &ref_id) {
    if (device_) {
      if (ref_id.empty()) {
        return std::static_pointer_cast<MockDeviceElementGroup>(
            device_->getDeviceElementGroup());
      } else {
        return std::static_pointer_cast<MockDeviceElementGroup>(
            device_->getDeviceElementGroup()->getSubelement(ref_id));
      }
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }

  std::string addDeviceElement(const std::string &group_refid,
                               const std::string &name, const std::string &desc,
                               ElementType type, DataType data_type,
                               std::optional<ReadFunctor> read_cb,
                               std::optional<WriteFunctor> write_cb) {
    std::string ref_id("");

    auto group = getGroupImplementation(group_refid);

    switch (type) {
    case ElementType::GROUP: {
      ref_id = group->addSubgroup(name, desc);
      break;
    };
    case ElementType::OBSERVABLE:
    case ElementType::WRITABLE: {
      ref_id = group->addWritableMetric(name, desc, data_type,
                                        setCallback<ReadFunctor>(read_cb),
                                        setCallback<WriteFunctor>(write_cb));
      break;
    }
    case ElementType::READABLE: {
      ref_id = group->addReadableMetric(name, desc, data_type,
                                        setCallback<ReadFunctor>(read_cb));
      break;
    }
    case ElementType::FUNCTION:
    default: { break; }
    }
    return ref_id;
  }

  DevicePtr getResult() {
    if (device_) {
      return move(device_);
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }
};
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP