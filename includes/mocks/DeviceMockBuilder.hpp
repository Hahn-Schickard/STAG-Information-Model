#ifndef __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP
#define __INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP

#include "../DeviceBuilderInterface.hpp"
#include "DeviceElementGroup_MOCK.hpp"
#include "Device_MOCK.hpp"
#include "Function_MOCK.hpp"
#include "Metric_MOCK.hpp"
#include "ObservableMetric_MOCK.hpp"
#include "WritableMetric_MOCK.hpp"

#include <gmock/gmock.h>
#include <optional>
#include <stdexcept>

namespace Information_Model {
namespace testing {
/**
 * @brief DeviceMockBuilder builds Device Mock instances.
 *
 * This class SHOULD be used by Data Consumer Adapter implementations to build
 * a mock/fake Device instance to test interactions with Information Model
 * without having access to real Device instances.
 *
 * This class MAY be used in Technology Adapter Implementations,
 * when a fake Device instance is required to test interactions with the Device
 * Model. For example, printing out the built Device instances in a system
 * integration test.
 *
 * @attention
 * Use for testing only
 */
struct DeviceMockBuilder : public DeviceBuilderInterface {
  // import interface overloads
  using DeviceBuilderInterface::addDeviceElementGroup;
  using DeviceBuilderInterface::addFunction;
  using DeviceBuilderInterface::addObservableMetric;
  using DeviceBuilderInterface::addReadableMetric;
  using DeviceBuilderInterface::addWritableMetric;

  DeviceMockBuilder() = default;

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  void buildDeviceBase(const std::string& unique_id,
      const std::string& name,
      const std::string& desc) override {
    if (!device_) {
      device_ = std::make_unique<::testing::NiceMock<MockDevice>>(
          unique_id, name, desc);
    } else {
      throw std::runtime_error( //
          "A device is already beeing built! Call "
          "DeviceMockBuilder::getResult() before starting to build a new one!");
    }
  }
  /** @}*/

  /**
   * @addtogroup GroupModeling Device Element Group Modelling
   * @{
   */
  std::string addDeviceElementGroup(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc) override {
    return addDeviceElement(group_ref_id, name, desc, Functionality())
        ->getElementId();
  }
  /** @}*/

  /**
   * @addtogroup ReadableModeling Metric Modelling
   * @{
   */
  std::string addReadableMetric(
      const std::string& name, const std::string& desc, DataType data_type) {
    return addDeviceElement(std::string(),
        name,
        desc,
        buildDefaultFunctionality(ElementType::READABLE, data_type))
        ->getElementId();
  }

  std::string addReadableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(ElementType::READABLE, data_type))
        ->getElementId();
  }

  std::string addReadableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      Reader read_cb) override {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(data_type, read_cb))
        ->getElementId();
  }
  /** @}*/

  /**
   * @addtogroup WritableModeling Writable Metric Modelling
   * @{
   */
  std::string addWritableMetric(
      const std::string& name, const std::string& desc, DataType data_type) {
    return addDeviceElement(std::string(),
        name,
        desc,
        buildDefaultFunctionality(ElementType::WRITABLE, data_type))
        ->getElementId();
  }

  std::string addWritableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(ElementType::WRITABLE, data_type))
        ->getElementId();
  }

  std::string addWritableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      Writer write_cb,
      Reader read_cb) override {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(data_type, read_cb, write_cb))
        ->getElementId();
  }
  /** @}*/

  /**
   * @addtogroup ObservableModeling Observable Metric Modelling
   * @{
   */
  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& name, const std::string& desc, DataType data_type) {
    return addObservableMetric(std::string(), name, desc, data_type);
  }

  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    auto element = addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(data_type, ObserveInitializer()));
    auto observable =
        std::get<NonemptyObservableMetricPtr>(element->functionality).base();
    return std::make_pair(element->getElementId(),
        std::bind(
            &ObservableMetric::observed, observable, std::placeholders::_1));
  }

  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      ObserveInitializer observe_cb) {
    return addObservableMetric(
        std::string(), name, desc, data_type, observe_cb);
  }

  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      ObserveInitializer observe_cb) {
    auto element = addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(data_type, observe_cb));
    auto observable =
        std::get<NonemptyObservableMetricPtr>(element->functionality).base();
    return std::make_pair(element->getElementId(),
        std::bind(
            &ObservableMetric::observed, observable, std::placeholders::_1));
  }

  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      Reader read_cb,
      ObserveInitializer observe_cb) override {
    auto element = addDeviceElement(group_ref_id,
        name,
        desc,
        Functionality(data_type, read_cb, observe_cb));
    auto observable =
        std::get<NonemptyObservableMetricPtr>(element->functionality).base();
    return std::make_pair(element->getElementId(),
        std::bind(
            &ObservableMetric::observed, observable, std::placeholders::_1));
  }
  /** @}*/

  /**
   * @addtogroup ExecutableModeling Function Modelling
   * @{
   */
  std::string addFunction(const std::string& name, const std::string& desc) {
    return addDeviceElement(std::string(),
        name,
        desc,
        buildDefaultFunctionality(ElementType::FUNCTION))
        ->getElementId();
  }

  std::string addFunction(
      const std::string& name, const std::string& desc, DataType result_type) {
    return addDeviceElement(std::string(),
        name,
        desc,
        Functionality(result_type, Function::ParameterTypes{}))
        ->getElementId();
  }

  std::string addFunction(const std::string& name,
      const std::string& desc,
      DataType result_type,
      Function::ParameterTypes supported_params) {
    return addDeviceElement(
        std::string(), name, desc, Functionality(result_type, supported_params))
        ->getElementId();
  }

  std::string addFunction(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType result_type,
      Function::ParameterTypes supported_params) {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(result_type, supported_params))
        ->getElementId();
  }

  std::string addFunction(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType result_type,
      Executor execute_cb,
      Canceler cancel_cb,
      Function::ParameterTypes supported_params) override {
    return addDeviceElement(group_ref_id,
        name,
        desc,
        Functionality(result_type, execute_cb, cancel_cb, supported_params))
        ->getElementId();
  }
  /** @}*/

  /**
   * @addtogroup GroupModeling Device Element Group Modelling
   * @{
   */
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
  /** @}*/

  /**
   * @brief Create any DeviceElement instance in a generic way
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param functionality
   * @return std::string
   */
  DeviceElementPtr addDeviceElement(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      const Functionality& functionality) {
    auto group = getGroupImplementation(group_ref_id);
    auto ref_id = group->generateReferenceID();
    DeviceElementPtr element;
    if (functionality.type() != ElementType::GROUP) {
      element = makeDeviceElement(
          ref_id, name, desc, buildSpecificInterface(functionality));
    } else {
      auto sub_group =
          std::make_shared<::testing::NiceMock<MockDeviceElementGroup>>(ref_id);
      element = makeDeviceElement(
          ref_id, name, desc, NonemptyDeviceElementGroupPtr(sub_group));
    }
    group->addDeviceElement(NonemptyDeviceElementPtr(element));

    return element;
  }

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  UniqueDevicePtr getResult() override {
    if (device_) {
      return std::move(device_);
    } else {
      throw std::runtime_error("Device base was not built!");
    }
  }
  /** @}*/

protected:
  Functionality buildDefaultFunctionality(
      ElementType type, DataType data_type) {
    switch (type) {
    case ElementType::READABLE: {
      return Functionality(data_type, Reader());
    }
    case ElementType::WRITABLE: {
      return Functionality(data_type, Reader(), Writer());
    }
    case ElementType::FUNCTION: {
      return Functionality(data_type, Executor(), Canceler(), {});
    }
    case ElementType::OBSERVABLE: {
      return Functionality(data_type, Reader(), ObserveInitializer());
    }
    case ElementType::GROUP: {
      return Functionality();
    }
    default: {
      std::string error_msg =
          "ElementType " + toString(type) + " is not supported";
      throw std::invalid_argument(error_msg);
    }
    }
  }

  Functionality buildDefaultFunctionality(
      DataType data_type, ObserveInitializer observe_cb) {
    return Functionality(data_type, Reader(), observe_cb);
  }

  Functionality buildDefaultFunctionality(ElementType type) {
    return buildDefaultFunctionality(type, DataType::BOOLEAN);
  }

  DeviceElement::SpecificInterface buildSpecificInterface(
      const Functionality& functionality) {
    switch (functionality.type()) {
    case ElementType::READABLE: {
      auto read = functionality.getRead();
      auto readable = std::make_shared<::testing::NiceMock<MockMetric>>(
          functionality.data_type);
      if (read.callback) {
        readable->delegateToFake(read.callback);
      } else {
        readable->delegateToFake();
      }

      return NonemptyMetricPtr(readable);
    }
    case ElementType::WRITABLE: {
      auto write = functionality.getWrite();
      auto writable = std::make_shared<::testing::NiceMock<MockWritableMetric>>(
          functionality.data_type);
      if (write.callback) {
        if (write.read_part.callback) {
          writable->delegateToFake(write.callback, write.read_part.callback);
        } else {
          writable->delegateToFake(write.callback);
        }
      } else {
        writable->delegateToFake();
      }

      return NonemptyWritableMetricPtr(writable);
    }
    case ElementType::OBSERVABLE: {
      auto observe = functionality.getObserve();
      auto observable =
          std::make_shared<::testing::NiceMock<MockObservableMetric>>(
              functionality.data_type, observe.callback);
      if (observe.read_part.callback) {
        observable->delegateToFake(observe.read_part.callback);
      } else {
        observable->delegateToFake();
      }

      return NonemptyObservableMetricPtr(observable);
    }
    case ElementType::FUNCTION: {
      auto execute = functionality.getExecute();
      auto executable = std::make_shared<::testing::NiceMock<MockFunction>>(
          functionality.data_type, execute.supported_params);
      if (execute.call && execute.cancel) {
        executable->delegateToFake(execute.call, execute.cancel);
      }

      return NonemptyFunctionPtr(executable);
    }
    default: {
      std::string error_msg = "ElementType " + toString(functionality.type()) +
          " is not supported by generalized SpecificInterface builder";
      throw std::invalid_argument(error_msg);
    }
    }
  }

  std::unique_ptr<MockDevice> device_;
};

using DeviceMockBuilderPtr = std::shared_ptr<DeviceMockBuilder>;
using NonemptyDeviceMockBuilderPtr = Nonempty::Pointer<DeviceMockBuilderPtr>;
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP