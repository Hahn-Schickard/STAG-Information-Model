#ifndef __INFORMATION_MODEL_FUNCTION_MOCK_HPP_
#define __INFORMATION_MODEL_FUNCTION_MOCK_HPP_

#include "../Function.hpp"

#include "gmock/gmock.h"
#include <chrono>
#include <future>

namespace Information_Model {

/**
 * @addtogroup FunctionModeling Function Modelling
 * @{
 */
struct MockFunction : public Function {
  using Executor = std::function<Function::ResultFuture(Function::Parameters)>;
  using Canceler = std::function<void(uintmax_t)>;

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
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
      ON_CALL(*this, asyncCall)
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
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
              throw FunctionCallTimedout("MockFunction");
            }
          });
      ON_CALL(*this, asyncCall).WillByDefault(async_call);
      ON_CALL(*this, cancelAsyncCall).WillByDefault([this](uintmax_t call_id) {
        auto iter = result_promises_.find(call_id);
        if (iter != result_promises_.end()) {
          iter->second.set_exception(
              std::make_exception_ptr(CallCanceled(call_id, "MockFunction")));
          iter = result_promises_.erase(
              iter); // @TODO: is this safe? Can we erase the promise before
                     // std::future<>::get() was called? Is this erased
                     // correctly?
        } else {
          throw CallerNotFound(call_id, "MockFunction");
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

  void respond(uintmax_t call_id, DataVariant value) {
    if (!executor_.has_value()) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_value(value);
        iter = result_promises_.erase(iter); // @TODO: same as previous todo
      } else {
        throw CallerNotFound(call_id, "MockFunction");
      }
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respond(uintmax_t call_id, const std::exception& exception) {
    if (!executor_.has_value()) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_exception(std::make_exception_ptr(exception));
        iter = result_promises_.erase(iter); // @TODO: same as previous todo
      } else {
        throw CallerNotFound(call_id, "MockFunction");
      }
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respondToAll(DataVariant value) {
    if (!executor_.has_value()) {
      for (auto iter = result_promises_.begin(); iter != result_promises_.end();
           iter++) {
        iter->second.set_value(value);
      }
      result_promises_.clear(); // @TODO: same as previous todo
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respondToAll(const std::exception& exception) {
    if (!executor_.has_value()) {
      for (auto iter = result_promises_.begin(); iter != result_promises_.end();
           iter++) {
        iter->second.set_exception(std::make_exception_ptr(exception));
      }
      result_promises_.clear(); // @TODO: same as previous todo
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void delegateToFake(Executor executor, Canceler canceler) {
    respondToAll(std::logic_error("Assigned a new external execution handler"));
    executor_ = executor;
    canceler_ = canceler;
    ON_CALL(*this, call)
        .WillByDefault([this](uintmax_t timeout,
                           Function::Parameters params) -> DataVariant {
          auto result_future =
              std::async(std::launch::async, executor_.value(), params);
          auto status =
              result_future.wait_for(std::chrono::milliseconds(timeout));
          if (status == std::future_status::ready) {
            return result_future.get().second.get();
          } else {
            throw FunctionCallTimedout("MockFunction");
          }
        });
    ON_CALL(*this, asyncCall).WillByDefault(executor_.value());
    ON_CALL(*this, cancelAsyncCall).WillByDefault(canceler_.value());
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

private:
  DataType result_type_;
  Function::ParameterTypes supported_params_;
  std::optional<DataVariant> result_value_;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
  std::optional<Executor> executor_;
  std::optional<Canceler> canceler_;
};

namespace testing {
using MockFunctionPtr = std::shared_ptr<MockFunction>;
/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_FUNCTION_MOCK_HPP_