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
 * It is based on the Builder Pattern and ensures that no part of the device
 * under construction is used before said device is fully built.
 *
 * To obtain a fully built device instance use getResult() method.
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
  using ObservedValue = std::function<void(DataVariant&&)>;
  using ObserveInitializer = std::function<void(bool)>;

  virtual ~DeviceBuilderInterface() = default;

  /**
   * @addtogroup DeviceModeling Device Modelling
   * @{
   */
  /**
   * @brief Create a base element for the device. This method MUST be called
   * before any add* method is called or after getResult() call.
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
    throw std::logic_error("Called base implementation of "
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
  std::string addDeviceElementGroup(
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
    throw std::logic_error("Called base implementation of "
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
      const Reader& read_cb) {
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
      const Reader& /*read_cb*/) {
    throw std::logic_error(
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
      const Writer& write_cb,
      const Reader& read_cb = nullptr) {
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
      const Writer& /*write_cb*/,
      const Reader& /*read_cb*/ = nullptr) {
    throw std::logic_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addWritableMetric for subgroup");
  }
  /** @}*/

  /**
   * @addtogroup ObservableModeling Observable Metric Modelling
   * @{
   */
  /**
   * @brief Adds an observable metric with a given return type to the device
   * root level DeviceElementGroup.
   *
   * This method creates a new Observable Metric instance, adds it to the root
   * DeviceElementGroup of the the currently built device and returns a
   * std::pair.
   *
   * The first element of the std::pair will be
   * a string based on the unique id, provided by the user with the previously
   * called buildDeviceBase() method. The format of this string will be as
   * follows:
   * @code {cpp}
   * DEVICE_UNIQUE_ID:ELEMENT_ID
   * @endcode
   *
   * The second element of the std::tuple will be the
   * Event_Model::EventSourceInterface::notify() callback function which is used
   * to notify any observers when new data is available.
   *
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param initialized_cb - observation (de)activation callback function, used
   * to inform the observable data source implementation when to start/stop
   * observation
   * @return std::pair<std::string, ObservedValue>
   */
  std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& name,
      const std::string& desc,
      DataType data_type,
      const Reader& read_cb,
      const ObserveInitializer& initialized_cb) {
    return addObservableMetric(
        std::string(), name, desc, data_type, read_cb, initialized_cb);
  }

  /**
   * @brief Adds an observable metric with a given return type to another
   * DeviceElementGroup. The parent group element MUST exist.
   *
   * This method creates a new Observable Metric instance, adds it to the
   * specified DeviceElementGroup of the the currently built device and returns
   * a std::pair.
   *
   * The requested group_ref_id argument is obtained from the previous
   * addDeviceElementGroup() call.
   *
   * The first element of the std::pair will be a string based on
   * the unique id, provided by the user
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
   * The second element of the std::tuple will be the
   * Event_Model::EventSourceInterface::notify() callback function which is used
   * to notify any observers when new data is available.
   *
   * @param group_ref_id
   * @param name
   * @param desc
   * @param data_type
   * @param read_cb
   * @param initialized_cb - observation (de)activation callback function, used
   * to inform the observable data source implementation when to start/stop
   * observation.
   * @return std::pair<std::string, ObservedValue>
   */
  virtual std::pair<std::string, ObservedValue> addObservableMetric(
      const std::string& /*group_ref_id*/,
      const std::string& /*name*/,
      const std::string& /*desc*/,
      DataType /*data_type*/,
      const Reader& /*read_cb*/,
      const ObserveInitializer& /*initialized_cb*/) {
    throw std::logic_error(
        "Called base implementation of "
        "DeviceBuilderInterface::addObservableMetric for subgroup");
  }
  /** @}*/

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
      const Executor& execute_cb,
      const Canceler& cancel_cb,
      const Function::ParameterTypes& supported_params = {}) {
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
      const Executor& execute_cb,
      const Function::ParameterTypes& supported_params = {}) {
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
      const Executor& execute_cb,
      const Function::ParameterTypes& supported_params = {}) {
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
      const Executor& /*execute_cb*/,
      const Canceler& /*cancel_cb*/,
      const Function::ParameterTypes& /*supported_params*/ = {}) {
    throw std::logic_error("Called base implementation of "
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
   * @attention
   * If this method is successful, it also resets the internal device
   * pointer to allow new instances creation. To create a new instance
   * use buildDeviceBase() method.
   *
   * If the device creation was not properly finished, throws an exception.
   *
   * @return UniqueDevicePtr
   */
  virtual UniqueDevicePtr getResult() {
    throw std::logic_error("Called base implementation of "
                           "DeviceBuilderInterface::getResult");
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
      explicit Read(const Reader& read_cb) : callback(read_cb) {}

      const Reader callback; // NOLINT(readability-identifier-naming)
    };

    /**
     * @brief Group information for WritableMetric SpecificInterface instances
     *
     */
    struct Write {
      Write() = default;
      explicit Write(const Writer& write_cb) : callback(write_cb) {}
      Write(const Reader& read_cb, const Writer& write_cb)
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
      explicit Execute(const Function::ParameterTypes& supported_parameters)
          : supported_params(supported_parameters) {}
      Execute(const Executor& execute_cb, const Canceler& cancel_cb)
          : call(execute_cb), cancel(cancel_cb) {}
      Execute(const Executor& execute_cb,
          const Canceler& cancel_cb,
          const Function::ParameterTypes& supported_parameters)
          : call(execute_cb), cancel(cancel_cb),
            supported_params(supported_parameters) {}

      const Executor call; // NOLINT(readability-identifier-naming)
      const Canceler cancel; // NOLINT(readability-identifier-naming)
      const Function::ParameterTypes
          supported_params; // NOLINT(readability-identifier-naming)
    };

    struct Observe {
      Observe() = default;
      Observe(const Reader& read_cb, const ObserveInitializer& observe_cb)
          : read_part(read_cb), callback(observe_cb) {}

      const Read read_part; // NOLINT(readability-identifier-naming)
      const ObserveInitializer
          callback; // NOLINT(readability-identifier-naming)
    };

    using Group = std::monostate;
    using Interface = std::variant<Group, Read, Write, Observe, Execute>;

    /**
     * @brief Creates basic information for a DeviceElementGroupMock
     *
     */
    Functionality() = default;

    /**
     * @brief Creates basic information for a simple MetricMock with default
     * read capability based on a given DataType
     *
     * @param type
     */
    explicit Functionality(DataType type)
        : data_type(type), interface(Read()) {}

    /**
     * @brief Creates basic information for a MetricMock with given DataType and
     * read capability
     *
     * @param type
     * @param read_cb
     */
    Functionality(DataType type, const Reader& read_cb)
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
    Functionality(DataType type, const Writer& write_cb)
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
    Functionality(DataType type, const Reader& read_cb, const Writer& write_cb)
        : data_type(type), interface(Write(read_cb, write_cb)) {}

    Functionality(DataType type,
        const Reader& read_cb,
        const ObserveInitializer& observe_cb)
        : data_type(type), interface(Observe(read_cb, observe_cb)) {}

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
        DataType result_type, const Function::ParameterTypes& supported_params)
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
    Functionality(DataType result_type,
        const Executor& execute_cb,
        const Canceler& cancel_cb)
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
        const Executor& execute_cb,
        const Canceler& cancel_cb,
        const Function::ParameterTypes& supported_params)
        : data_type(result_type),
          interface(Execute(execute_cb, cancel_cb, supported_params)) {}

    ElementType type() const {
      if (std::holds_alternative<Read>(interface)) {
        return ElementType::READABLE;
      } else if (std::holds_alternative<Write>(interface)) {
        return ElementType::WRITABLE;
      } else if (std::holds_alternative<Observe>(interface)) {
        return ElementType::OBSERVABLE;
      } else if (std::holds_alternative<Execute>(interface)) {
        return ElementType::FUNCTION;
      } else {
        return ElementType::GROUP;
      }
    }

    Read getRead() const { return std::get<Read>(interface); }
    Write getWrite() const { return std::get<Write>(interface); }
    Observe getObserve() const { return std::get<Observe>(interface); }
    Execute getExecute() const { return std::get<Execute>(interface); }

    const DataType data_type =
        DataType::NONE; // NOLINT(readability-identifier-naming)
    const Interface interface =
        Group(); // NOLINT(readability-identifier-naming)
  };

protected:
  /**
   * @addtogroup ElementModeling Device Element Modelling
   * @{
   */
  DeviceElementPtr makeDeviceElement(const std::string& ref_id,
      const std::string& name,
      const std::string& desc,
      DeviceElement::SpecificInterface&& interface) const {
    auto obj = DeviceElement(ref_id, name, desc, std::move(interface));
    // We can only call DeviceElement() from within DeviceBuilderInterface
    // class due to access the specifier. Thus we must first create the
    // object locally and then pass it to std::make_shared<>() function.
    return std::make_shared<DeviceElement>(std::move(obj));
  }
  /** @}*/
};

using DeviceBuilderInterfacePtr = std::shared_ptr<DeviceBuilderInterface>;
using NonemptyDeviceBuilderInterfacePtr =
    Nonempty::Pointer<DeviceBuilderInterfacePtr>;
} // namespace Information_Model

#endif //__INFORMATION_MODEL_DEVICE_BUILDER_INTERFACE_HPP
