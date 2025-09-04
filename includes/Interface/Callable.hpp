#ifndef __STAG_INFORMATION_MODEL_CALLABLE_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_HPP

#include "DataVariant.hpp"

#include <chrono>
#include <future>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Information_Model {
/**
 * @addtogroup ExecutableModeling Callable Modelling
 * @{
 */

struct ResultReturningNotSupported : public std::runtime_error {
  ResultReturningNotSupported()
      : std::runtime_error(
            "Callable does not support returning execution results") {}
};

struct MandatoryParameterHasNoValue : public std::invalid_argument {
  MandatoryParameterHasNoValue(uintmax_t param_id, DataType param_type)
      : std::invalid_argument("Mandatory parameter " +
            std::to_string(param_id) + ":" + toString(param_type) +
            " has no value set") {}
};

struct MandatoryParameterMissing : public std::invalid_argument {
  MandatoryParameterMissing(uintmax_t param_id, DataType param_type)
      : std::invalid_argument("Mandatory parameter " +
            std::to_string(param_id) + ":" + toString(param_type) +
            " is missing") {}
};

struct ParameterTypeMismatch : public std::invalid_argument {
  ParameterTypeMismatch(uintmax_t param_id, DataType expected, DataType given)
      : std::invalid_argument("Parameter " + std::to_string(param_id) + ":" +
            toString(expected) + " does not accept " + toString(given) +
            " type values") {}
};

struct ParameterDoesNotExist : public std::invalid_argument {
  explicit ParameterDoesNotExist(uintmax_t param_id)
      : std::invalid_argument(
            "No parameter exists at position " + std::to_string(param_id)) {}
};

struct ExecutorNotAvailable : public std::runtime_error {
  ExecutorNotAvailable()
      : std::runtime_error("Executor callback is no longer available") {}
};

struct CancellerNotAvailable : public std::runtime_error {
  CancellerNotAvailable()
      : std::runtime_error("Canceler callback is no longer available") {}
};

struct CallerNotFound : public std::runtime_error {
  CallerNotFound(uintmax_t call_id, const std::string& name)
      : std::runtime_error("No caller with id: " + std::to_string(call_id) +
            " for Callable " + name + " call exists") {}
};

struct CallerIDExists : public std::runtime_error {
  CallerIDExists(uintmax_t call_id, const std::string& name)
      : std::runtime_error("Caller with id: " + std::to_string(call_id) +
            " for Callable " + name + " was already dispatched") {}
};

struct CallAlreadyCanceled : public std::runtime_error {
  explicit CallAlreadyCanceled(uintmax_t call_id)
      : std::runtime_error("Caller with id: " + std::to_string(call_id) +
            " call is already canceled") {}
};

struct CallCanceled : public std::runtime_error {
  CallCanceled(uintmax_t call_id, const std::string& name)
      : std::runtime_error("Caller with id: " + std::to_string(call_id) +
            " canceled the execution call for Callable " + name) {}
};

struct CallTimedout : public std::runtime_error {
  explicit CallTimedout(const std::string& name)
      : std::runtime_error("Callable " + name + " call timed out") {}
};

/**
 * @brief Async response future wrapper for the Callable execution result
 *
 */
struct ResultFuture {
  /**
   * @brief Used to indicate that a given future has been consumed or canceled
   *
   *
   * CallClearer(call_id, false) - indicates that the future result has been
   * consumed CallClearer(call_id, true) - indicates that the given result
   * request has been canceled
   */
  using CallClearer = std::function<void(uintmax_t, bool)>;

  ResultFuture(uintmax_t caller,
      std::future<DataVariant>&& result,
      const CallClearer& clearer);

  ResultFuture(const ResultFuture&) = delete;

  ResultFuture(ResultFuture&&) = default;

  ~ResultFuture() = default;

  ResultFuture& operator=(const ResultFuture&) = delete;

  ResultFuture& operator=(ResultFuture&&) = default;

  /**
   * @brief Obtains the promised future result and removes the call_id from
   * active operations list
   *
   * @throws CallCanceled - if the requested operation was canceled by the
   * user
   * @throws std::runtime_error - if internal callback encountered an error.
   * It may have caused @ref Deregistration
   *
   * @return DataVariant
   */
  DataVariant get();

  template <class Rep, class Period>
  std::future_status wait_for(
      const std::chrono::duration<Rep, Period>& timeout_duration) const {
    return result_.wait_for(timeout_duration);
  }

  /**
   * @brief Tries to cancel the requested result. If result was already set,
   * suppresses the value, otherwise cancels the request.
   *
   * @throws std::runtime_error - if result was already delivered and contains
   * an exception
   * @throws CancellerNotAvailable - if assigned canceler is no longer available
   *
   */
  void cancel();

  uintmax_t callerID() const;

private:
  uintmax_t id_;
  std::future<DataVariant> result_;
  CallClearer clearer_;
};

/**
 * @brief An interface to a Callable.
 *
 * Models a single functionality for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addCallable()
 */
struct Callable {
  using Parameter = std::optional<DataVariant>;
  /**
   * @brief Indexed map of the modeled function parameters
   *
   * @param Key - parameter number
   * @param Value - parameter value
   */
  using Parameters = std::unordered_map<uintmax_t, Parameter>;

  struct ParameterType {
    DataType type;
    bool mandatory = false;
  };

  /**
   * @brief Indexed map of the modeled function parameter types
   *
   * @param Key - parameter number
   * @param Value - parameter type
   */
  using ParameterTypes = std::unordered_map<uintmax_t, ParameterType>;

  virtual ~Callable() = default;

  /**
   * @brief Executes the modeled functionality without waiting for the execution
   * result
   *
   * @throws std::logic_error - if base implementation was called
   *
   * @attention May cause @ref Deregistration
   *
   * @param parameters
   */
  virtual void execute(const Parameters& parameters = Parameters()) const = 0;
  /**
   * @brief Calls the modeled functionality and waits to return the execution
   * result
   *
   * Blocks until the execution result is available or a timeout occurs
   *
   * If execution call timesout, the request will be canceled and an exception
   * will be thrown
   *
   * @throws ResultReturningNotSupported - if modeled functionality does not
   * support returning execution result
   * @throws CallTimedout - if execution call has timeout
   * @throws CallerIDExists - if internal callback returned a caller id
   * that is already assigned
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @param timeout - number of miliseconds until a timeout occurs
   * @return DataVariant
   */
  virtual DataVariant call(uintmax_t timeout = 100) const = 0;

  /**
   * @brief Calls the modeled functionality and waits to return the execution
   * result
   *
   * Blocks until the execution result is available or a timeout occurs
   *
   * If execution call timesout, the request will be canceled and an exception
   * will be thrown
   *
   * @throws ResultReturningNotSupported - if modeled functionality does not
   * support returning execution result
   * @throws CallTimedout - if execution call has timeout
   * @throws CallerIDExists - if internal callback returned a caller id
   * that is already assigned
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @param parameters
   * @param timeout - number of miliseconds until a timeout occurs
   * @return DataVariant
   */
  virtual DataVariant call(
      const Parameters& parameters, uintmax_t timeout = 100) const = 0;

  /**
   * @brief Calls the modeled functionality and allocates a future for the
   * execution result
   *
   * Blocks until the execution call is dispatched
   *
   * @throws ResultReturningNotSupported- if modeled functionality does not
   * support returning execution result
   * @throws CallerIDExists - if internal callback returned a caller id
   * that is already assigned
   *
   * @attention May cause @ref Deregistration
   *
   * @param parameters
   * @return ResultFuture
   */
  [[nodiscard]] virtual ResultFuture asyncCall(
      const Parameters& parameters = Parameters()) const = 0;

  /**
   * @brief Cancels a given asynchronous call
   *
   * The linked result future (the second Callable::ResultFuture parameter) will
   * throw an exception to indicate that it was canceled
   *
   * @throws CallerNotFound - if the given call_id does not indicate a
   * previous asynchronous call
   * @throws ResultReturningNotSupported- if modeled functionality does not
   * support returning execution result
   * @throws std::runtime_error - if internal cancellation mechanism encountered
   * an error
   *
   * @param call_id - obtained from the first ResultFuture parameter
   */
  virtual void cancelAsyncCall(uintmax_t call_id) const = 0;

  /**
   * @brief Cancels all asynchronous call executions
   *
   * All result promises, that have not been fulfilled, will be canceled. All
   * linked result futures will throw an exception to indicate that it was
   * canceled
   *
   * @throws ResultReturningNotSupported- if modeled functionality does not
   * support returning execution result
   * @throws std::runtime_error - if internal cancellation mechanism encountered
   * an error
   */
  virtual void cancelAllAsyncCalls() const = 0;

  virtual DataType resultType() const = 0;

  virtual ParameterTypes parameterTypes() const = 0;
}; // namespace Information_Model

bool operator==(
    const Callable::ParameterType& lhs, const Callable::ParameterType& rhs);

bool operator!=(
    const Callable::ParameterType& lhs, const Callable::ParameterType& rhs);

/**
 * @brief Helper function to expand an existing parameter map with supported
 * parameter types
 *
 * @throws ParameterDoesNotExist - if no supported parameter type is defined at
 * a given position
 * @throws ParameterTypeMismatch - if given parameter does not match the
 * supported parameter type
 * @throws MandatoryParameterHasNoValue - if given parameter is marked as
 * mandatory, but has no value
 *
 * @param map - target container, if operation succeeded, the given container
 * will be larger by one element
 * @param supported_types - validation container, obtained from
 * Callable::parameterTypes();
 * @param position - parameter position
 * @param parameter - assigned parameter value
 * @param strict_assign - if true overrides existing parameter values, otherwise
 * keeps the old value
 */
void addSupportedParameter(Callable::Parameters& map,
    const Callable::ParameterTypes& supported_types,
    uintmax_t position,
    const Callable::Parameter& param,
    bool strict_assign = false);

/**
 * @brief Checks if a given container has all the mandatory parameters set and
 * if set parameters have correct values
 *
 * @throws MandatoryParameterMissing - if given container does not have one of
 * the required parameters
 * @throws ParameterTypeMismatch - if given parameter does not match the
 * supported parameter type
 * @throws MandatoryParameterHasNoValue - if given parameter is marked as
 * mandatory, but has no value
 *
 * @param input_parameters - input container
 * @param supported_types - validation container, obtained from
 * Callable::parameterTypes();
 */
void checkParameters(const Callable::Parameters& input_parameters,
    const Callable::ParameterTypes& supported_types);

/**
 * @brief Converts a given Callable::ParameterTypes container to a human
 * readable string
 *
 * @param supported_types
 * @return std::string - returns "{}" if empty, otherwise "{{...},...}"
 */
std::string toString(const Callable::ParameterTypes& supported_types);

/**
 * @brief Converts a given Callable::Parameters container to a human
 * readable string
 *
 * @param parameters
 * @return std::string - returns "{}" if empty, otherwise "{{...},...}"
 */
std::string toString(const Callable::Parameters& parameters);

using CallablePtr = std::shared_ptr<Callable>;
/** @}*/
} // namespace Information_Model
#endif //__STAG_INFORMATION_MODEL_CALLABLE_HPP