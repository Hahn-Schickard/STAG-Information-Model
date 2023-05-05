#ifndef __INFORMATION_MODEL_FUNCTION_MOCK_HPP_
#define __INFORMATION_MODEL_FUNCTION_MOCK_HPP_

#include "../Function.hpp"

#include "gmock/gmock.h"
#include <chrono>

namespace Information_Model {

/**
 * @addtogroup FunctionModeling Function Modelling
 * @{
 */
struct MockFunction : public Function {
  MockFunction() : MockFunction(DataType::UNKNOWN) {}

  MockFunction(Function::ParameterTypes supported_params)
      : MockFunction(DataType::UNKNOWN, supported_params, std::nullopt) {}

  MockFunction(DataType result_type)
      : MockFunction(result_type, Function::ParameterTypes(), std::nullopt) {}

  MockFunction(DataType result_type, std::optional<DataVariant> result_value)
      : MockFunction(result_type, Function::ParameterTypes(), result_value) {}

  MockFunction(DataType result_type,
      Function::ParameterTypes supported_params,
      std::optional<DataVariant> result_value)
      : Function(), result_type_(result_type),
        supported_params_(supported_params), result_value_(result_value) {
    if (!result_value_.has_value()) {
      ON_CALL(*this, call)
          .WillByDefault(::testing::Throw(std::runtime_error(
              "Function does not support returning results")));
      ON_CALL(*this, asyncCall)
          .WillByDefault(::testing::Throw(std::runtime_error(
              "Function does not support returning results")));
    } else {
      auto async_call =
          [this](
              Function::Parameters /*parameters*/) -> Function::ResultFuture {
        auto call_id = result_promises_.size();
        auto promise = std::promise<DataVariant>();
        auto result_future = std::make_pair(call_id, promise.get_future());
        result_promises_.emplace(call_id, std::move(promise));
        return std::move(result_future);
      };
      ON_CALL(*this, call)
          .WillByDefault([this, async_call](
                             uintmax_t timeout, Function::Parameters params) {
            auto result = async_call(params);
            auto status =
                result.second.wait_for(std::chrono::milliseconds(timeout));
            if (status == std::future_status::ready) {
              return result.second.get();
            } else {
              cancelAsyncCall(result.first);
              return DataVariant();
            }
          });
      ON_CALL(*this, asyncCall).WillByDefault(async_call);
      ON_CALL(*this, cancelAsyncCall).WillByDefault([this](uintmax_t call_id) {
        auto iter = result_promises_.find(call_id);
        if (iter != result_promises_.end()) {
          iter->second.set_exception(std::make_exception_ptr(
              std::runtime_error("Asynchronous was cancelled")));
          iter = result_promises_.erase(
              iter); // @TODO: is this safe? Can we erase the promise before
                     // std::future<>::get() was called? Is this erased
                     // correctly?
        } else {
          std::string error_msg = "No asynchronous call with id " +
              std::to_string(call_id) + " exists";
          throw std::runtime_error(error_msg);
        }
      });
    }
    ON_CALL(*this, getResultDataType)
        .WillByDefault(::testing::Return(result_type_));
    ON_CALL(*this, getSupportedParameterTypes)
        .WillByDefault(::testing::Return(supported_params_));
  }

  ~MockFunction() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(void, execute, (Function::Parameters /*parameters*/), (override));
  MOCK_METHOD(DataVariant,
      call,
      (uintmax_t /*timeout*/, Function::Parameters /*parameters*/),
      (override));
  MOCK_METHOD(Function::ResultFuture,
      asyncCall,
      (Function::Parameters /*parameters*/),
      (override));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t /*call_id*/), (override));
  MOCK_METHOD(DataType, getResultDataType, (), (override));
  MOCK_METHOD(
      Function::ParameterTypes, getSupportedParameterTypes, (), (override));

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

private:
  DataType result_type_;
  Function::ParameterTypes supported_params_;
  std::optional<DataVariant> result_value_;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
};

namespace testing {
using MockFunctionPtr = std::shared_ptr<MockFunction>;
/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_FUNCTION_MOCK_HPP_