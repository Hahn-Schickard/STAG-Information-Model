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
 *
 * This class SHOULD be used by Data Consumer Adapter implementations to build a
 * mock/fake Device instance to test interactions with Information Model without
 * having access to real Device instances or Technology Adapter Implementations,
 * when a fake Device instance is required to test interactions with the Device
 * Model. For example, printing out the built Device instances in a system
 * integration test
 */
class DeviceMockBuilder : public DeviceBuilderInterface {
  std::unique_ptr<MockDevice> device_;

public:
  DeviceMockBuilder() = default;

  void buildDeviceBase(const std::string& unique_id,
      const std::string& name,
      const std::string& desc) override {
    if (!device_) {
      device_ = std::make_unique<::testing::NiceMock<MockDevice>>(
          unique_id, name, desc);
    } else {
      throw std::runtime_error(
          "A device is already beeing built! Call "
          "DeviceMockBuilder::getResult() before starting to build a new one!");
    }
  }

  std::string addDeviceElementGroup(
      const std::string& name, const std::string& desc) override {
    return addDeviceElement(
        std::string(), name, desc, ElementType::GROUP, DataType::UNKNOWN);
  }

  std::string addDeviceElementGroup(const std::string& group_refid,
      const std::string& name,
      const std::string& desc) override {
    return addDeviceElement(
        group_refid, name, desc, ElementType::GROUP, DataType::UNKNOWN);
  }

  std::string addReadableMetric(
      const std::string& name, const std::string& desc, DataType data_type) {
    return addDeviceElement(
        std::string(), name, desc, ElementType::READABLE, data_type);
  }

  std::string addReadableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      ReadFunctor read_cb) override {
    return addDeviceElement(
        std::string(), name, desc, ElementType::READABLE, data_type, read_cb);
  }

  std::string addReadableMetric(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      ReadFunctor read_cb) override {
    return addDeviceElement(
        group_refid, name, desc, ElementType::READABLE, data_type, read_cb);
  }

  std::string addReadableMetric(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(
        group_refid, name, desc, ElementType::READABLE, data_type);
  }

  std::string addWritableMetric(
      const std::string& name, const std::string& desc, DataType data_type) {
    return addDeviceElement(
        std::string(), name, desc, ElementType::WRITABLE, data_type);
  }

  std::string addWritableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      std::optional<ReadFunctor> read_cb,
      WriteFunctor write_cb) override {
    return addDeviceElement(std::string(),
        name,
        desc,
        ElementType::WRITABLE,
        data_type,
        read_cb,
        write_cb);
  }

  std::string addWritableMetric(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(
        group_refid, name, desc, ElementType::WRITABLE, data_type);
  }

  std::string addWritableMetric(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      std::optional<ReadFunctor> read_cb,
      WriteFunctor write_cb) override {
    return addDeviceElement(group_refid,
        name,
        desc,
        ElementType::WRITABLE,
        data_type,
        read_cb,
        write_cb);
  }

  MockDeviceElementGroupPtr getGroupImplementation(const std::string& ref_id) {
    if (device_) {
      if (ref_id.empty()) {
        return std::static_pointer_cast<MockDeviceElementGroup>(
            device_->getDeviceElementGroup().base());
      } else {
        return std::static_pointer_cast<MockDeviceElementGroup>(
            std::get<NonemptyDeviceElementGroupPtr>(
                device_->getDeviceElementGroup()
                    ->getSubelement(ref_id)
                    ->specific_interface)
                .base());
      }
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }

  std::string addDeviceElement(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      ElementType type = ElementType::GROUP,
      DataType data_type = DataType::UNKNOWN,
      std::optional<ReadFunctor> read_cb = std::nullopt,
      std::optional<WriteFunctor> write_cb = std::nullopt,
      std::optional<ExecuteFunctor> execute_cb = std::nullopt) override {
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
    case ElementType::FUNCTION: {
      // @TODO: implement function support
      __attribute__((unused)) auto suppress = execute_cb;
      break;
    }
    default: { break; }
    }
    return ref_id;
  }

  UniqueDevicePtr getResult() override {
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