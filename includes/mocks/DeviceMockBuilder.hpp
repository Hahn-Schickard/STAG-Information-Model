#ifndef __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP

#include "../DeviceBuilderInterface.hpp"
#include "DeviceElementGroup_MOCK.hpp"
#include "Device_MOCK.hpp"

#include <gmock/gmock.h>
#include <optional>
#include <stdexcept>

namespace Information_Model {
namespace testing {

/**
 * @brief DeviceMockBuilder builds Device Mock instances.
 * This class should be used by Data Consumer Adapter implementations to build a
 * mock Device instance during unit and integration test stages
 *
 */
class DeviceMockBuilder : public DeviceBuilderInterface {
  MockDevicePtr device_;

public:
  DeviceMockBuilder() = default;

  void buildDeviceBase(const std::string &unique_id, const std::string &name,
                       const std::string &desc) {
    if (!device_) {
      device_ = std::make_shared<::testing::NiceMock<MockDevice>>(unique_id,
                                                                  name, desc);
    } else {
      throw std::runtime_error(
          "A device is already beeing built! Call "
          "DeviceMockBuilder::getResult() before starting to build a new one!");
    }
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
                                const std::string &desc, DataType data_type) {
    return addDeviceElement(std::string(), name, desc, ElementType::READABLE,
                            data_type, std::nullopt, std::nullopt);
  }

  std::string addReadableMetric(const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb) {
    return addDeviceElement(std::string(), name, desc, ElementType::READABLE,
                            data_type, read_cb, std::nullopt);
  }

  std::string addReadableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type,
                                ReadFunctor read_cb) {
    return addDeviceElement(group_refid, name, desc, ElementType::READABLE,
                            data_type, read_cb, std::nullopt);
  }

  std::string addReadableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type) {
    return addDeviceElement(group_refid, name, desc, ElementType::READABLE,
                            data_type, std::nullopt, std::nullopt);
  }

  std::string addWritableMetric(const std::string &name,
                                const std::string &desc, DataType data_type) {
    return addDeviceElement(std::string(), name, desc, ElementType::WRITABLE,
                            data_type, std::nullopt, std::nullopt);
  }

  std::string addWritableMetric(const std::string &name,
                                const std::string &desc, DataType data_type,
                                std::optional<ReadFunctor> read_cb,
                                WriteFunctor write_cb) {
    return addDeviceElement(std::string(), name, desc, ElementType::WRITABLE,
                            data_type, read_cb, write_cb);
  }

  std::string addWritableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type) {
    return addDeviceElement(group_refid, name, desc, ElementType::WRITABLE,
                            data_type, std::nullopt, std::nullopt);
  }

  std::string addWritableMetric(const std::string &group_refid,
                                const std::string &name,
                                const std::string &desc, DataType data_type,
                                std::optional<ReadFunctor> read_cb,
                                WriteFunctor write_cb) {
    return addDeviceElement(group_refid, name, desc, ElementType::WRITABLE,
                            data_type, read_cb, write_cb);
  }

  MockDeviceElementGroupPtr getGroupImplementation(const std::string &ref_id) {
    if (device_) {
      if (ref_id.empty()) {
        return std::static_pointer_cast<MockDeviceElementGroup>(
            device_->getDeviceElementGroup().base());
      } else {
        return std::static_pointer_cast<MockDeviceElementGroup>(
          std::get<NonemptyDeviceElementGroupPtr>(
            device_->getDeviceElementGroup()->getSubelement(ref_id)
              ->specific_interface)
            .base());
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
    case ElementType::WRITABLE: {
      ref_id =
          group->addWritableMetric(name, desc, data_type, read_cb, write_cb);
      break;
    }
    case ElementType::READABLE: {
      ref_id = group->addReadableMetric(name, desc, data_type, read_cb);
      break;
    }
    default: { break; }
    }
    return ref_id;
  }

  DevicePtr getResult() {
    if (device_) {
      return std::move(device_);
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }
};
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP