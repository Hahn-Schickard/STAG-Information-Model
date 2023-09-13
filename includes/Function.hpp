#ifndef __INFORMATION_MODEL_FUNCTION_HPP
#define __INFORMATION_MODEL_FUNCTION_HPP

#include "DataVariant.hpp"

#include <future>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
/**
 * @addtogroup ExecutableModeling Function Modelling
 * @{
 */

struct ResultReturningNotSupported : public std::runtime_error {
  ResultReturningNotSupported()
      : std::runtime_error(
            "Function does not support returning execution results") {}
};

struct CallerNotFound : public std::runtime_error {
  CallerNotFound(uintmax_t call_id, const std::string& name)
      : std::runtime_error("No caller with id: " + std::to_string(call_id) +
            " for Function " + name + " call exists") {}
};

struct CallerIDExists : public std::runtime_error {
  CallerIDExists(uintmax_t call_id, const std::string& name)
      : std::runtime_error("Caller with id: " + std::to_string(call_id) +
            " for Function " + name + " was already dispatched") {}
};

struct CallCanceled : public std::runtime_error {
  CallCanceled(uintmax_t call_id, const std::string& name)
      : std::runtime_error("Caller with id: " + std::to_string(call_id) +
            " canceled the execution call for Function " + name) {}
};

struct FunctionCallTimedout : public std::runtime_error {
  FunctionCallTimedout(const std::string& name)
      : std::runtime_error("Function " + name + " call timedout") {}
};

/**
 * @brief An interface to a Function.
 *
 * Models a single functionality for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addFunction()
 */

struct Function {
  using Parameter = std::optional<DataVariant>;
  /**
   * @brief Indexed map of the modeled function parameters
   *
   * @param Key - parameter number in the function
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
   * @param Key - parameter number in the function
   * @param Value - parameter type
   */
  using ParameterTypes = std::unordered_map<uintmax_t, ParameterType>;

  /**
   * @brief Async response future wrapper
   *
   */
  struct ResultFuture : std::future<DataVariant> {
    using CallClearer = std::function<void(uintmax_t)>;

    ResultFuture(std::future<DataVariant>&& future_result,
        uintmax_t caller,
        CallClearer clearer)
        : std::future<DataVariant>(std::move(future_result)), call_id(caller),
          clear_caller(clearer) {}

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
    DataVariant get() {
      auto result = std::future<DataVariant>::get();
      clear_caller(call_id);
      return result;
    }

    const uintmax_t call_id; // NOLINT(readability-identifier-naming)

  private:
    const CallClearer clear_caller; // NOLINT(readability-identifier-naming)
  };

  virtual ~Function() = default;

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
  virtual void execute(const Parameters& /*parameters*/ = Parameters()) {
    throw std::logic_error(
        "Called based implementation of Function::execute()");
  }

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
   * @throws FunctionCallTimedout - if execution call has timeout
   * @throws CallerIDExists - if internal callback returned a caller id
   * that is already assigned
   * @throws std::runtime_error - if internal callback encountered an
   * error. May cause @ref Deregistration
   *
   * @param timeout - number of miliseconds until a timeout occurs
   * @return DataVariant
   */
  DataVariant call(uintmax_t timeout = 100) {
    return call(Parameters{}, timeout);
  }

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
   * @throws FunctionCallTimedout - if execution call has timeout
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
      const Parameters& /*parameters*/, uintmax_t /*timeout*/ = 100) {
    throw ResultReturningNotSupported();
  }

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
      const Parameters& /*parameters*/ = Parameters()) {
    throw ResultReturningNotSupported();
  }

  /**
   * @brief Cancels a given asynchronous call
   *
   * The linked result future (the second Function::ResultFuture parameter) will
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
  virtual void cancelAsyncCall(uintmax_t /*call_id*/) {
    throw ResultReturningNotSupported();
  }

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
  virtual void cancelAllAsyncCalls() { throw ResultReturningNotSupported(); }

  bool operator==(const Function& other) const noexcept {
    return (result_type == other.result_type) &&
        (parameters == other.parameters);
  }

  bool operator!=(const Function& other) const noexcept {
    return !operator==(other);
  }

  const DataType result_type; // NOLINT(readability-identifier-naming)
  const ParameterTypes parameters; // NOLINT(readability-identifier-naming)

protected:
  Function(DataType type, const ParameterTypes& supported_parameters)
      : result_type(type), parameters(supported_parameters) {}
}; // namespace Information_Model

/**
 * @brief Helper function to expand an existing parameter map with a given
 * Function::Parameter
 *
 * @throws std::range_error - if Function::Parameter value is already assigned
 * at a given parameter number
 *
 * @param map
 * @param param_number
 * @param param
 */
inline void addParameter(Function::Parameters& map,
    uintmax_t param_number,
    const Function::Parameter& param) {
  if (!map.try_emplace(param_number, param).second) {
    auto existing = map[param_number].value_or(DataVariant());
    auto param_value = param.value_or(DataVariant());
    std::string error_msg = "Could not add " +
        toString(toDataType(param_value)) + "(" + toString(param_value) + ")" +
        " as parameter number " + std::to_string(param_number) +
        " because parameter " + toString(toDataType(existing)) + "(" +
        toString(existing) + ") is already assigned to that number";
    throw std::range_error(error_msg);
  }
}

/**
 * @brief Helper function to expand an existing parameter map with supported
 * parameter types only
 *
 * @throws std::invalid_argument - if parameter type or number is not supported
 * @throws std::range_error - if Function::Parameter value is already assigned
 * at a given parameter number
 *
 * @param map
 * @param supported_types
 * @param param_number
 * @param param
 */
inline void addSupportedParameter(Function::Parameters& map,
    const Function::ParameterTypes& supported_types,
    uintmax_t param_number,
    const Function::Parameter& param) {
  auto supported_types_iter = supported_types.find(param_number);
  if (supported_types_iter != supported_types.end()) {
    auto supported_type = supported_types_iter->second;
    if (param.has_value()) {
      auto param_value = param.value();
      if (matchVariantType(param_value, supported_type.first)) {
        addParameter(map, param_number, param);
      } else {
        std::string error_msg = "Parameter number " +
            std::to_string(param_number) + " " +
            toString(supported_type.first) + " does not support " +
            toString(toDataType(param_value)) + "(" + toString(param_value) +
            ") value";
        throw std::invalid_argument(error_msg);
      }
    } else if (supported_type.second) {
      addParameter(map, param_number, std::nullopt);
    } else {
      std::string error_msg = "Parameter number " +
          std::to_string(param_number) + " " + toString(supported_type.first) +
          " does not support empty values";
      throw std::invalid_argument(error_msg);
    }
  } else {
    std::string error_msg = "Parameter number " + std::to_string(param_number) +
        " is not supported";
    throw std::invalid_argument(error_msg);
  }
}

inline std::string toString(Function::ParameterTypes params) {
  return std::accumulate(std::next(params.begin()),
      params.end(),
      toString(params[0].first),
      [](std::string& result,
          std::pair<uintmax_t, Function::ParameterType> parameter) {
        return result + ", " + toString(parameter.second.first);
      });
}

using FunctionPtr = std::shared_ptr<Function>;
using NonemptyFunctionPtr = NonemptyPointer::NonemptyPtr<FunctionPtr>;
/** @}*/
} // namespace Information_Model
#endif //__INFORMATION_MODEL_FUNCTION_HPP