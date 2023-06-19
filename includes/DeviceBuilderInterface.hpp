#ifndef __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP
#define __INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP

#include "DataVariant.hpp"
#include "Device.hpp"
#include "Function.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace Information_Model {
using UniqueDevicePtr = std::unique_ptr<Device>;

/**
 * @brief This Interface is used by Technology Adapter implementations to build
 * a device within the Information Model.
 *
 * Used by Technology Adapter implementations to build a device within the
 * Information Model.
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and
 * SHOULD be acquired via TechnologyAdapter::getDeviceBuilder() method.
 *
 * To test interactions with this interface, please set
 * testing::DeviceBuilderMock class as a DeviceBuilderPtr when creating
 * a new TechnologyAdapter instance by calling
 * TechnologyAdapter::setInterfaces() method.
 */
struct DeviceBuilderInterface {
  using Reader = std::function<DataVariant()>;
  using Writer = std::function<void(DataVariant)>;
  using ExecutorResult = std::pair<uintmax_t, std::future<DataVariant>>;
  using Executor = std::function<ExecutorResult(Function::Parameters)>;
  using Canceler = std::function<void(uintmax_t)>;

  virtual ~DeviceBuilderInterface() = default;

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  /**
   * @brief Create a base element for the device. This method MUST be called
   * before any add* method is called.
   *
   * This method MUST be called first when creating a new device, because this
   * method initializes the base implementation as well as assigns the device
   * ID, which will be used as a seed for DeviceElement ID generation
   *
   * @param unique_id
   * @param name
   * @param desc
   */
  virtual void buildDeviceBase(const std::string& /*unique_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::buildDeviceBase");
  }
  /** @}*/

  /**
   * @addtogroup GroupModeling Device Element Group Modelling
   * @{
   */
  /**
   * @brief Adds a group element to the device root level DeviceElementGroup.
   *
   * This method creates a new DeviceElementGroup instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created DeviceElementGroup. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(
      const std::string& name, const std::string& desc) {
    return addDeviceElementGroup(std::string(), name, desc);
  }

  /**
   * @brief Adds a group element to another DeviceElementGroup. The parent group
   * element MUST exist.
   *
   * This method creates a new DeviceElementGroup instance, adds it to the
   * specified DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created DeviceElementGroup.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string will be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If the specified parent DeviceElementGroup is a subgroup itself, the
   * returned string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID.SUBPARENT_ELEMENT_ID.ELEMENT_ID
   * @endcode
   * A similar format will be used in the nth level of subgroups as well.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @return std::string
   */
  virtual std::string addDeviceElementGroup(const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/) {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::addDeviceElementGroup");
  }
  /** @}*/

  /**
   * @addtogroup ReadableModeling Metric Modelling
   * @{
   */
  /**
   * @brief Adds a readable metric with a given data type to the device root
   * level DeviceElementGroup.
   *
   * This method creates a new Metric instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Metric. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  std::string addReadableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      Reader read_cb) {
    return addReadableMetric(std::string(), name, desc, data_type, read_cb);
  }

  /**
   * @brief Adds a readable metric with a given data type to another
   * DeviceElementGroup. The parent group element MUST exist.
   *
   * This method creates a new Metric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Metric.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID.SUBPARENT_ELEMENT_ID.ELEMENT_ID
   * @endcode
   * A similar format will be used nth level of subgroup as well.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @return std::string
   */
  virtual std::string addReadableMetric(const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/,
      DataType /*data_type*/,
      Reader /*read_cb*/) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addReadableMetric for subgroup");
  }
  /** @}*/

  /**
   * @addtogroup WritableModeling Writable Metric Modelling
   * @{
   */
  /**
   * @brief Adds a writable metric with a given data type to the device root
   * level DeviceElementGroup.
   *
   * This method creates a new WritableMetric instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created WritableMetric. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  std::string addWritableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type,
      Writer write_cb,
      Reader read_cb = nullptr) {
    return addWritableMetric(
        std::string(), name, desc, data_type, write_cb, read_cb);
  }

  /**
   * @brief Adds a writable metric with a given data type to another
   * DeviceElementGroup. The parent group element MUST exist.
   *
   * This method creates a new WritableMetric instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created WritableMetric.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID.SUBPARENT_ELEMENT_ID.ELEMENT_ID
   * @endcode
   * A similar format will be used nth level of subgroup as well.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param type
   * @param data_type
   * @param read_cb
   * @param write_cb
   * @return std::string
   */
  virtual std::string addWritableMetric(const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/,
      DataType /*data_type*/,
      Writer /*write_cb*/,
      Reader /*read_cb*/ = nullptr) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addWritableMetric for subgroup");
  }
  /** @}*/

  /**
   * @todo Decide on how to best create a binding to an Observable
   *
   */

  std::string addObservableMetric(const std::string& name,
      const std::string& desc,
      DataType data_type /* @todo: Observer binding mechanism */) {
    return addObservableMetric(std::string(), name, desc, data_type);
  }

  /**
   * @todo Decide on how to best create a binding to an Observable
   *
   */
  virtual std::string addObservableMetric(const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/,
      DataType /*data_type*/ /* @todo: Observer binding mechanism */) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addObservableMetric for subgroup");
  }

  /**
   * @addtogroup ExecutableModeling Function Modelling
   * @{
   */
  /**
   * @brief Adds an executable function with a given return type, execute and
   * cancel execution callable objects to the device root level
   * DeviceElementGroup.
   *
   * This method creates a new Function instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @param result_type
   * @param supported_params
   * @param execute_cb
   * @param cancel_cb
   * @return std::string
   */
  std::string addFunction(const std::string& name,
      const std::string& desc,
      DataType result_type,
      Executor execute_cb,
      Canceler cancel_cb,
      Function::ParameterTypes supported_params = {}) {
    return addFunction(std::string(),
        name,
        desc,
        result_type,
        execute_cb,
        cancel_cb,
        supported_params);
  }

  /**
   * @brief Adds an executable function with a given execute callable object to
   * the device root level DeviceElementGroup. Sets the built functions return
   * type to DataType::NONE
   *
   * This method creates a new Function instance, adds it to the root
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function. The returned
   * string will be based on the unique id, provided by the user with the
   * previously called buildDeviceBase() method. The format of this string will
   * be as follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * @param name
   * @param desc
   * @param execute_cb
   * @param supported_params
   * @return std::string
   */
  std::string addFunction(const std::string& name,
      const std::string& desc,
      Executor execute_cb,
      Function::ParameterTypes supported_params = {}) {
    return addFunction(std::string(),
        name,
        desc,
        DataType::NONE,
        execute_cb,
        nullptr,
        supported_params);
  }

  /**
   * @brief Adds an executable function with a given execute callable object to
   * to another DeviceElementGroup. The parent group element MUST exist. Sets
   * the built functions return type to DataType::NONE
   *
   * This method creates a new Function instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID.SUBPARENT_ELEMENT_ID.ELEMENT_ID
   * @endcode
   * A similar format will be used nth level of subgroup as well.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param execute_cb
   * @param supported_params
   * @return std::string
   */
  std::string addFunction(const std::string& group_ref_id,
      const std::string& name,
      const std::string& desc,
      Executor execute_cb,
      Function::ParameterTypes supported_params = {}) {
    return addFunction(group_ref_id,
        name,
        desc,
        DataType::NONE,
        execute_cb,
        nullptr,
        supported_params);
  }

  /**
   * @brief Adds a function to another DeviceElementGroup. The parent group
   * element MUST exist.
   *
   * This method creates a new Function instance, adds it to the specified
   * DeviceElementGroup of the currently built device and returns the
   * DeviceElement ID of the newly created Function.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The returned string will be based on the unique id, provided by the user
   * with the previously called buildDeviceBase() method. The format of this
   * string the be as follows: DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID:ELEMENT_ID
   *
   * If specified parent DeviceElementGroup is a subgroup itself, the returned
   * string will represent it as a new ID element as such:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:PARENT_ELEMENT_ID.SUBPARENT_ELEMENT_ID.ELEMENT_ID
   * @endcode
   * A similar format will be used nth level of subgroup as well.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param type
   * @param result_type
   * @param execute_cb
   * @param supported_params
   * @param cancel_cb
   * @return std::string
   */
  virtual std::string addFunction(const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/,
      DataType /*result_type*/,
      Executor /*execute_cb*/,
      Canceler /*cancel_cb*/,
      Function::ParameterTypes /*supported_params*/ = {}) {
    throw std::runtime_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addFunction for subgroup");
  }
  /** @}*/

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  /**
   * @brief Checks if the built result is valid and returns it.
   *
   * If the device creation was not properly finished, throws an exception.
   *
   * @return UniqueDevicePtr
   */
  virtual UniqueDevicePtr getResult() {
    throw std::runtime_error("Called base implementation of "
                             "DeviceBuilderInterface::getResult");
  }
  /** @}*/

protected:
  /**
   * @addtogroup ElementModeling Device Element Modelling
   * @{
   */
  DeviceElementPtr makeDeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      DeviceElement::SpecificInterface&& interface) {
    auto obj = DeviceElement(ref_id, name, desc, std::move(interface));
    // We can only call DeviceElement() from within DeviceBuilderInterface
    // class due to access the specifier. Thus we must first create the
    // object locally and then pass it to std::make_shared<>() function.
    return std::make_shared<DeviceElement>(std::move(obj));
  }
  /** @}*/
};

using DeviceBuilderInterfacePtr = std::shared_ptr<DeviceBuilderInterface>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP
