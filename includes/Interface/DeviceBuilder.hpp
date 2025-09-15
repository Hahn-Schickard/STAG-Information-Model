#ifndef __STAG_INFORMATION_MODEL_DEVICE_BUILDER_HPP
#define __STAG_INFORMATION_MODEL_DEVICE_BUILDER_HPP

#include "Callable.hpp"
#include "DataVariant.hpp"
#include "Device.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace Information_Model {
/**
 * @addtogroup DeviceModeling Device Modelling
 * @{
 */

/**
 * @brief Helper struct to store optional element information
 *
 */
struct BuildInfo {
  std::string name{};
  std::string description{};
};

struct GroupEmpty : public std::logic_error {
  explicit GroupEmpty(const std::string& device_id)
      : std::logic_error("Device " + device_id + " root group is empty") {}

  GroupEmpty(const std::string& device_id, const std::string& group_id)
      : std::logic_error(
            "Device " + device_id + " group " + group_id + " is empty") {}
};

struct DeviceInfoNotSet : public std::logic_error {
  DeviceInfoNotSet() : std::logic_error("Device Information was never built") {}
};

struct DeviceBuildInProgress : public std::logic_error {
  DeviceBuildInProgress() : std::logic_error("Device is already being built") {}
};

/**
 * @brief Device Builder interface used by Technology Adapter implementations to
 * build a device within the Information Model
 *
 */
struct DeviceBuilder {
  /**
   * @brief Used by the Readable to get the latest value
   *
   */
  using ReadCallback = std::function<DataVariant()>;

  /**
   * @brief Used by the Writable to set a user given value
   *
   */
  using WriteCallback = std::function<void(const DataVariant&)>;

  /**
   * @brief Used by the Callable to execute the modeled functionality without
   * returning a result
   *
   */
  using ExecuteCallback = std::function<void(const Parameters&)>;

  /**
   * @brief Used by the Callble to asynchronously execute the modeled function
   *
   */
  using AsyncExecuteCallback = std::function<ResultFuture(const Parameters&)>;

  /**
   * @brief Used by the Callable to cancel a previous ExecuteCallback call
   *
   */
  using CancelCallback = std::function<void(uintmax_t)>;

  /**
   * @brief Used by the Observable implementer to send notifications to
   * registered observers
   *
   */
  using NotifyCallback = std::function<void(const DataVariant&)>;

  /**
   * @brief Used by the Observable to indicate when the implementation
   * should start/stop sending notifications to the registered observers
   *
   */
  using IsObservingCallback = std::function<void(bool)>;

  virtual ~DeviceBuilder() = default;

  /**
   * @brief Create the base device with the specified id. This device will then
   * store all of the built elements.
   *
   * @attention This method MUST be called before any add*() or result() method
   * calls.
   *
   * @param unique_id
   * @param element_info
   */
  virtual void setDeviceInfo(
      const std::string& unique_id, const BuildInfo& element_info) = 0;

  /**
   * @brief Creates a new Group element and adds it to the root Group
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   *
   * @param element_info
   * @return std::string - the ID of the built Group Element
   */
  virtual std::string addGroup(const BuildInfo& element_info) = 0;

  /**
   * @brief Creates a new Group element and adds it to a given parent Group, if
   * it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if given parent id points to a group that
   * does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @return std::string - the ID of the built Group Element
   */
  virtual std::string addGroup(
      const std::string& parent_id, const BuildInfo& element_info) = 0;

  /**
   * @brief Creates a Readable element and adds it to the root Group
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ReadCallback is null
   *
   * @param element_info
   * @param data_type
   * @param read_cb
   * @return std::string - the ID of the built Readable Element
   */
  virtual std::string addReadable(const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb) = 0;

  /**
   * @brief Creates a Readable element and adds it to a given parent Group, if
   * it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ReadCallback is null
   *  - given parent id points to a group that does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @param data_type
   * @param read_cb
   * @return std::string - the ID of the built Readable Element
   */
  virtual std::string addReadable(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb) = 0;

  /**
   * @brief Creates a Writable element and adds it to the root Group
   * If ReadCallback is given as null, creates a write only Writable element
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given WriteCallback is null
   *
   * @param element_info
   * @param data_type
   * @param write_cb
   * @param read_cb
   * @return std::string - the ID of the built Writable Element
   */
  virtual std::string addWritable(const BuildInfo& element_info,
      DataType data_type,
      const WriteCallback& write_cb,
      const ReadCallback& read_cb = nullptr) = 0;

  /**
   * @brief Creates a Writable element and adds it to a given parent Group, if
   * it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given WriteCallback is null
   *  - given parent id points to a group that does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @param data_type
   * @param write_cb
   * @param read_cb
   * @return std::string - the ID of the built Writable Element
   */
  virtual std::string addWritable(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const WriteCallback& write_cb,
      const ReadCallback& read_cb = nullptr) = 0;

  /**
   * @brief Creates an Observable element and adds it to the root Group
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ReadCallback is null
   *  - given IsObservingCallback is null
   *
   * @param element_info
   * @param data_type
   * @param read_cb
   * @param observe_cb
   * @return std::pair<std::string, NotifyCallback>
   *  - std::string - the ID of the built Observable Element
   *  - NotifyCallback - callback function to dispatch new value notifications
   */
  virtual std::pair<std::string, NotifyCallback> addObservable(
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb,
      const IsObservingCallback& observe_cb) = 0;

  /**
   * @brief Creates an Observable element and adds it to a given parent Group,
   * if it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ReadCallback is null
   *  - given IsObservingCallback is null
   *  - given parent id points to a group that does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @param data_type
   * @param read_cb
   * @param observe_cb
   * @return std::pair<std::string, NotifyCallback>
   *  - std::string - the ID of the built Observable Element
   *  - NotifyCallback - callback function to dispatch new value notifications
   */
  virtual std::pair<std::string, NotifyCallback> addObservable(
      const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb,
      const IsObservingCallback& observe_cb) = 0;

  /**
   * @brief Creates a Callable element that returns no value and adds it to the
   * root Group
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if given ExecuteCallback is null
   *
   * @param element_info
   * @param execute_cb
   * @param parameter_types
   * @return std::string - the ID of the built Callable Element
   */
  virtual std::string addCallable(const BuildInfo& element_info,
      const ExecuteCallback& execute_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  /**
   * @brief Creates a Callable element that returns no value and adds it to a
   * given parent Group, if it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   * - given ExecuteCallback is null
   * - given parent id points to a group that does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @param execute_cb
   * @param parameter_types
   * @return std::string - the ID of the built Callable Element
   */
  virtual std::string addCallable(const std::string& parent_id,
      const BuildInfo& element_info,
      const ExecuteCallback& execute_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  /**
   * @brief Creates a Callable element that returns a given result value and
   * adds it to the root Group
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ExecuteCallback is null
   *  - given AsyncExecuteCallback is null
   *  - given CancelCallback is null
   *
   * @param element_info
   * @param result_type
   * @param execute_cb
   * @param async_execute_cb
   * @param cancel_cb
   * @param parameter_types
   * @return std::string - the ID of the built Callable Element
   */
  virtual std::string addCallable(const BuildInfo& element_info,
      DataType result_type,
      const ExecuteCallback& execute_cb,
      const AsyncExecuteCallback& async_execute_cb,
      const CancelCallback& cancel_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  /**
   * @brief  Creates a Callable element that returns a given result value and
   * adds it to a given parent Group, if it exists
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws std::invalid_argument - if
   *  - given DataType is None or Unknown
   *  - given ExecuteCallback is null
   *  - given AsyncExecuteCallback is null
   *  - given CancelCallback is null
   *  - given parent id points to a group that does not exit
   *
   * @param parent_id - result of any addGroup() method call
   * @param element_info
   * @param result_type
   * @param execute_cb
   * @param async_execute_cb
   * @param cancel_cb
   * @param parameter_types
   * @return std::string - the ID of the built Callable Element
   */
  virtual std::string addCallable(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType result_type,
      const ExecuteCallback& execute_cb,
      const AsyncExecuteCallback& async_execute_cb,
      const CancelCallback& cancel_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  /**
   * @brief Verifies that the device was built correctly and moves the built
   * device instance to the caller, thus reseting the builder for a fresh build
   *
   * @throws DeviceInfoNotSet - DeviceBuilder::setDeviceInfo() was not called
   * @throws GroupEmpty - one of the built groups is empty
   *
   * @return std::unique_ptr<Device>
   */
  virtual std::unique_ptr<Device> result() = 0;
};

using DeviceBuilderPtr = std::shared_ptr<DeviceBuilder>;
/** @}*/
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DEVICE_BUILDER_HPP
