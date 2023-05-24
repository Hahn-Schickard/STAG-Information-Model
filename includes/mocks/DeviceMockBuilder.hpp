#ifndef __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP

#include "../DeviceBuilderInterface.hpp"
#include "DeviceElementGroup_MOCK.hpp"
#include "Device_MOCK.hpp"
#include "Function_MOCK.hpp"
#include "Metric_MOCK.hpp"
#include "WritableMetric_MOCK.hpp"

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
struct DeviceMockBuilder : public DeviceBuilderInterface {
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
                    ->functionality)
                .base());
      }
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }

  DeviceElementPtr buildDeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      ElementType type,
      DataType data_type,
      std::optional<ReadFunctor> read_cb,
      std::optional<WriteFunctor> write_cb,
      std::optional<Function::ParameterTypes> supported_params,
      std::optional<ExecuteFunctor> execute_cb,
      std::optional<CancelFunctor> cancel_cb) {
    switch (type) {
    case ElementType::GROUP: {
      auto group =
          std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);

      return makeDeviceElement(
          ref_id, name, desc, NonemptyDeviceElementGroupPtr(group));
    }
    case ElementType::WRITABLE: {
      auto writable =
          std::make_shared<::testing::NiceMock<MockWritableMetric>>(data_type);
      if (read_cb.has_value()) {
        if (write_cb.has_value()) {
          writable->delegateToFake(read_cb.value(), write_cb.value());
        } else {
          writable->delegateToFake(read_cb.value());
        }
      } else {
        writable->delegateToFake();
      }

      return makeDeviceElement(
          ref_id, name, desc, NonemptyWritableMetricPtr(writable));
    }
    case ElementType::READABLE: {
      auto readable =
          std::make_shared<::testing::NiceMock<MockMetric>>(data_type);
      if (read_cb.has_value()) {
        readable->delegateToFake(read_cb.value());
      } else {
        readable->delegateToFake();
      }

      return makeDeviceElement(ref_id, name, desc, NonemptyMetricPtr(readable));
    }
    case ElementType::FUNCTION: {
      auto executable = std::make_shared<::testing::NiceMock<MockFunction>>(
          data_type, supported_params.value_or(Function::ParameterTypes()));
      if (execute_cb.has_value() && cancel_cb.has_value()) {
        executable->delegateToFake(execute_cb.value(), cancel_cb.value());
      }

      return makeDeviceElement(
          ref_id, name, desc, NonemptyFunctionPtr(executable));
    }
    default: {
      throw std::invalid_argument("Requested to build unsupported ElementType");
    }
    }
  }

  std::string addDeviceElement(const std::string& group_refid,
      const std::string& name,
      const std::string& desc,
      ElementType type = ElementType::GROUP,
      DataType data_type = DataType::UNKNOWN,
      std::optional<ReadFunctor> read_cb = std::nullopt,
      std::optional<WriteFunctor> write_cb = std::nullopt,
      std::optional<Function::ParameterTypes> supported_params = std::nullopt,
      std::optional<ExecuteFunctor> execute_cb = std::nullopt,
      std::optional<CancelFunctor> cancel_cb = std::nullopt) override {
    auto group = getGroupImplementation(group_refid);
    auto new_id = group->generateReferenceID();
    auto element = buildDeviceElement(new_id,
        name,
        desc,
        type,
        data_type,
        read_cb,
        write_cb,
        supported_params,
        execute_cb,
        cancel_cb);
    group->addDeviceElement(NonemptyDeviceElementPtr(element));

    return new_id;
  }

  UniqueDevicePtr getResult() override {
    if (device_) {
      return std::move(device_);
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }

private:
  std::unique_ptr<MockDevice> device_;
};
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP