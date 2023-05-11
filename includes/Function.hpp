#ifndef __INFORMATION_MODEL_FUNCTION_HPP
#define __INFORMATION_MODEL_FUNCTION_HPP

#include "DataVariant.hpp"

#include <future>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Nonempty_Pointer/NonemptyPtr.hpp"

namespace Information_Model {
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
 * @addtogroup FunctionModeling Function Modelling
 * @{
 */
/**
 * @brief An interface to a Function.
 *
 * Models a single functionality for various sensors/actors
 *
 * @attention
 * This interface is implemented in Information Model Manager Project and is
 * built via DeviceBuilderInterface::addFunction() or
 * DeviceBuilderInterface::addDeviceElement() with type set to
 * ElementType::FUNCTION
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
   * @brief Async response buffer
   *
   * @param First - async call id
   * @param Second - result buffer
   */
  using ResultFuture = std::pair<uintmax_t, std::future<DataVariant>>;

  /**
   * @brief Executes the modeled functionality without waiting for the execution
   * result
   *
   * @throws std::runtime_error - if base implementation was called
   *
   * @param parameters
   */
  virtual void execute(Parameters /*parameters*/ = Parameters()) {
    throw std::runtime_error(
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
   *
   * @param parameters
   * @param timeout - number of miliseconds until a timeout occurs
   * @return DataVariant
   */
  virtual DataVariant call(
      uintmax_t /*timeout*/ = 100, Parameters /*parameters*/ = Parameters()) {
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
   *
   * @param parameters
   * @return ResultFuture
   */
  virtual ResultFuture asyncCall(Parameters /*parameters*/ = Parameters()) {
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
   * @throws std::runtime_error - if base implementation was called
   *
   * @param call_id - obtained from the first ResultFuture parameter
   */
  virtual void cancelAsyncCall(uintmax_t /*call_id*/) {
    throw std::runtime_error(
        "Called based implementation of Function::cancelAsyncCall()");
  }

  /**
   * @brief Cancels all asynchronous call executions
   *
   * All result promises, that have not been fulfilled, will be canceled. All
   * linked result futures will throw an exception to indicate that it was
   * canceled
   *
   * @throws std::runtime_error - if base implementation was called
   */
  virtual void cancelAllAsyncCalls() {
    throw std::runtime_error(
        "Called based implementation of Function::cancelAllAsyncCalls()");
  }

  /**
   * @brief Returns the supported result type
   *
   * If the modeled Function does not support returning a result, this method
   * will return DataType::UNKNOWN
   *
   * @throws std::runtime_error - if base implementation was called
   *
   * @return DataType
   */
  virtual DataType getResultDataType() {
    throw std::runtime_error(
        "Called based implementation of Function::getResultDataType()");
  }

  /**
   * @brief Returns an index map of supported Parameter types
   *
   * @throws std::runtime_error - if base implementation was called
   *
   * @return ParameterTypes
   */
  virtual ParameterTypes getSupportedParameterTypes() {
    throw std::runtime_error("Called based implementation of "
                             "Function::getSupportedParameterTypes()");
  }

protected:
  Function() = default;
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
    Function::Parameter param) {
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
    Function::Parameter param) {
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

using FunctionPtr = std::shared_ptr<Function>;
using NonemptyFunctionPtr = NonemptyPointer::NonemptyPtr<FunctionPtr>;

/** @}*/
} // namespace Information_Model
#endif //__INFORMATION_MODEL_FUNCTION_HPP