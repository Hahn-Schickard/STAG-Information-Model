#ifndef __STAG_INFORMATION_MODEL_CALLABLE_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_HPP

#include "DataVariant.hpp"

#include <future>
#include <memory>
#include <numeric>
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
  using CallClearer = std::function<void(uintmax_t)>;

  ResultFuture(uintmax_t caller,
      std::future<DataVariant>&& future_result,
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

  uintmax_t callerID() const;

private:
  uintmax_t call_id_;
  std::future<DataVariant> future_result_;
  CallClearer clear_caller_;
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

  /**
   * @brief Contains parameter type information
   *
   * @param First - parameter data type
   * @param Second - indicates if this parameter is optional
   *
   */
  using ParameterType = std::pair<DataType, bool>;

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
  virtual ResultFuture asyncCall(
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

/**
 * @brief Helper function to expand an existing parameter map with a given
 * Callable::Parameter
 *
 * @throws std::range_error - if Callable::Parameter value is already assigned
 * at a given parameter number
 *
 * @param map
 * @param param_number
 * @param param
 */
void addParameter(Callable::Parameters& map,
    uintmax_t param_number,
    const Callable::Parameter& param);

/**
 * @brief Helper function to expand an existing parameter map with supported
 * parameter types only
 *
 * @throws std::invalid_argument - if parameter type or number is not supported
 * @throws std::range_error - if Callable::Parameter value is already assigned
 * at a given parameter number
 *
 * @param map
 * @param supported_types
 * @param param_number
 * @param param
 */
void addSupportedParameter(Callable::Parameters& map,
    const Callable::ParameterTypes& supported_types,
    uintmax_t param_number,
    const Callable::Parameter& param);

std::string toString(Callable::ParameterTypes params);

using CallablePtr = std::shared_ptr<Callable>;
/** @}*/
} // namespace Information_Model
#endif //__STAG_INFORMATION_MODEL_CALLABLE_HPP