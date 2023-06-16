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
  std::string addDeviceElementGroup(
      const std::string& name, const std::string& desc) override {
    return addDeviceElement(std::string(), name, desc, Functionality());
  }

  std::string addDeviceElementGroup(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc) override {
    return addDeviceElement(group_ref_id, name, desc, Functionality());
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
        buildDefaultFunctionality(ElementType::READABLE, data_type));
  }

  std::string addReadableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(ElementType::READABLE, data_type));
  }

  std::string addReadableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      Reader read_cb) override {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(data_type, read_cb));
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
        buildDefaultFunctionality(ElementType::WRITABLE, data_type));
  }

  std::string addWritableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type) {
    return addDeviceElement(group_ref_id,
        name,
        desc,
        buildDefaultFunctionality(ElementType::WRITABLE, data_type));
  }

  std::string addWritableMetric(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      Writer write_cb,
      Reader read_cb) override {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(data_type, read_cb, write_cb));
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
        buildDefaultFunctionality(ElementType::FUNCTION));
  }

  std::string addFunction(
      const std::string& name, const std::string& desc, DataType result_type) {
    return addDeviceElement(
        std::string(), name, desc, Functionality(result_type));
  }

  std::string addFunction(const std::string& name,
      const std::string& desc,
      DataType result_type,
      Function::ParameterTypes supported_params) {
    return addDeviceElement(std::string(),
        name,
        desc,
        Functionality(result_type, supported_params));
  }

  std::string addFunction(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      DataType result_type,
      Function::ParameterTypes supported_params) {
    return addDeviceElement(
        group_ref_id, name, desc, Functionality(result_type, supported_params));
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
        Functionality(result_type, execute_cb, cancel_cb, supported_params));
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
   * @brief Groups information required by SpecificInterface construction
   *
   */
  struct Functionality {
    /**
     * @brief Group information for Metric SpecificInterface instances
     *
     */
    struct Read {
      Read() = default;
      Read(Reader read_cb) : callback(read_cb) {}

      const Reader callback; // NOLINT(readability-identifier-naming)
    };

    /**
     * @brief Group information for WritableMetric SpecificInterface instances
     *
     */
    struct Write {
      Write() = default;
      Write(Writer write_cb) : callback(write_cb) {}
      Write(Reader read_cb, Writer write_cb)
          : read_part(read_cb), callback(write_cb) {}

      const Read read_part; // NOLINT(readability-identifier-naming)
      const Writer callback; // NOLINT(readability-identifier-naming)
    };

    /**
     * @brief Group information for Function SpecificInterface instances
     *
     */
    struct Execute {
      Execute() = default;
      Execute(Function::ParameterTypes supported_parameters)
          : supported_params(supported_parameters) {}
      Execute(Executor execute_cb, Canceler cancel_cb)
          : call(execute_cb), cancel(cancel_cb) {}
      Execute(Executor execute_cb,
          Canceler cancel_cb,
          Function::ParameterTypes supported_parameters)
          : call(execute_cb), cancel(cancel_cb),
            supported_params(supported_parameters) {}

      const Executor call; // NOLINT(readability-identifier-naming)
      const Canceler cancel; // NOLINT(readability-identifier-naming)
      const Function::ParameterTypes
          supported_params; // NOLINT(readability-identifier-naming)
    };

    using Group = std::monostate;
    using Interface = std::variant<Group, Read, Write, Execute>;

    /**
     * @brief Creates basic information for a DeviceElementGroupMock
     *
     */
    Functionality() : data_type(DataType::NONE), interface(Group()) {}

    /**
     * @brief Creates basic information for a simple MetricMock with default
     * read capability based on a given DataType
     *
     * @param type
     */
    Functionality(DataType type) : data_type(type), interface(Read()) {}

    /**
     * @brief Creates basic information for a MetricMock with given DataType and
     * read capability
     *
     * @param type
     * @param read_cb
     */
    Functionality(DataType type, Reader read_cb)
        : data_type(type), interface(Read(read_cb)) {}

    /**
     * @brief Creates basic information for a WritableMetricMock with given
     * DataType and write capability
     *
     * @attention
     * Set Writer to nullptr, if faking write functionality is not
     * required
     *
     * @param type
     * @param write_cb
     */
    Functionality(DataType type, Writer write_cb)
        : data_type(type), interface(Write(write_cb)) {}

    /**
     * @brief Creates basic information for a WritableMetricMock with given
     * DataType and read and write capabilities
     *
     * @attention
     * Set Reader to nullptr, if default read capability is sufficient for
     * your use case
     *
     * @attention
     * Set Writer to nullptr, if faking write functionality is not
     * required
     *
     * @param type
     * @param read_cb
     * @param write_cb
     */
    Functionality(DataType type, Reader read_cb, Writer write_cb)
        : data_type(type), interface(Write(read_cb, write_cb)) {}

    /**
     * @brief Creates basic information for a simple FunctionMock with given
     * result DataType and supported function parameters
     *
     * @attention
     * Set Function::ParameterTypes to {} in case function parameters are not
     * used
     *
     * @param result_type
     * @param supported_params
     */
    Functionality(
        DataType result_type, Function::ParameterTypes supported_params)
        : data_type(result_type), interface(Execute(supported_params)) {}

    /**
     * @brief Creates basic information for a FunctionMock with given result
     * DataType, executor and cancel functions
     *
     * @attention
     * Nullptr values are not allowed.
     *
     * @param result_type
     * @param execute_cb
     * @param cancel_cb
     */
    Functionality(DataType result_type, Executor execute_cb, Canceler cancel_cb)
        : data_type(result_type), interface(Execute(execute_cb, cancel_cb)) {}

    /**
     * @brief Creates basic information for a FunctionMock with given result
     * DataType, executor cancel functions and supported function parameters
     *
     * @attention
     * Nullptr values are not allowed.
     *
     * @attention
     * Set Function::ParameterTypes to {} in case function parameters are not
     * used
     *
     * @param result_type
     * @param execute_cb
     * @param cancel_cb
     * @param supported_params
     */
    Functionality(DataType result_type,
        Executor execute_cb,
        Canceler cancel_cb,
        Function::ParameterTypes supported_params)
        : data_type(result_type),
          interface(Execute(execute_cb, cancel_cb, supported_params)) {}

    ElementType type() const {
      if (std::holds_alternative<Read>(interface)) {
        return ElementType::READABLE;
      } else if (std::holds_alternative<Write>(interface)) {
        return ElementType::WRITABLE;
      } else if (std::holds_alternative<Execute>(interface)) {
        return ElementType::FUNCTION;
      } else {
        return ElementType::GROUP;
      }
    }

    Read getRead() const { return std::get<Read>(interface); }
    Write getWrite() const { return std::get<Write>(interface); }
    Execute getExecute() const { return std::get<Execute>(interface); }

    const DataType data_type; // NOLINT(readability-identifier-naming)
    const Interface interface; // NOLINT(readability-identifier-naming)
  };

  /**
   * @brief Create any DeviceElement instance in a generic way
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param functionality
   * @return std::string
   */
  std::string addDeviceElement(const std::string& group_ref_id,
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

    return ref_id;
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
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_MOCK_BUILDER_HPP