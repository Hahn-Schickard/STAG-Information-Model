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

struct BuildInfo {
  std::string name;
  std::string description;
};

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
   * @brief Indexed future result that the Callble returns to the callers
   *
   */
  using AsyncExecuteFuture = std::pair<uintmax_t, std::future<DataVariant>>;

  /**
   * @brief Used by the Callble to asynchronously execute the modeled function
   *
   */
  using AsyncExecuteCallback =
      std::function<AsyncExecuteFuture(const Parameters&)>;

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
   * @brief Used by the Observable to start/stop sending notifications to
   * registered observers
   *
   */
  using IsObservingCallback = std::function<void(bool)>;

  virtual ~DeviceBuilder() = default;

  virtual void setDeviceInfo(
      const std::string& unique_id, const BuildInfo& element_info) = 0;

  virtual std::string addGroup(const BuildInfo& element_info) = 0;

  virtual std::string addGroup(
      const std::string& parent_id, const BuildInfo& element_info) = 0;

  virtual std::string addReadable(
      const BuildInfo& element_info, DataType, const ReadCallback& read_cb) = 0;

  virtual std::string addReadableMetric(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb) = 0;

  virtual std::string addWritable(const BuildInfo& element_info,
      DataType data_type,
      const WriteCallback& write_cb,
      const ReadCallback& read_cb = nullptr) = 0;

  virtual std::string addWritableMetric(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const WriteCallback& write_cb,
      const ReadCallback& read_cb = nullptr) = 0;

  virtual std::pair<std::string, NotifyCallback> addObservable(
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb,
      const IsObservingCallback& observe_cb) = 0;

  virtual std::pair<std::string, NotifyCallback> addObservable(
      const std::string& parent_id,
      const BuildInfo& element_info,
      DataType data_type,
      const ReadCallback& read_cb,
      const IsObservingCallback& observe_cb) = 0;

  virtual std::string addCallable(const BuildInfo& element_info,
      DataType data_type,
      const AsyncExecuteCallback& async_execute_cb,
      const CancelCallback& cancel_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  virtual std::string addCallable(const BuildInfo& element_info,
      const ExecuteCallback& execute_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  virtual std::string addCallable(const std::string& parent_id,
      const BuildInfo& element_info,
      const ExecuteCallback& execute_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  virtual std::string addCallable(const std::string& parent_id,
      const BuildInfo& element_info,
      DataType result_type,
      const ExecuteCallback& execute_cb,
      const AsyncExecuteCallback& async_execute_cb,
      const CancelCallback& cancel_cb,
      const ParameterTypes& parameter_types = {}) = 0;

  virtual std::unique_ptr<Device> result() = 0;
};

using DeviceBuilderPtr = std::shared_ptr<DeviceBuilder>;
} // namespace Information_Model

#endif //__STAG_INFORMATION_MODEL_DEVICE_BUILDER_HPP
