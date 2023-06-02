#ifndef __INFORMATION_MODEL_FUNCTION_MOCK_HPP_
#define __INFORMATION_MODEL_FUNCTION_MOCK_HPP_

#include "../Function.hpp"

#include "gmock/gmock.h"
#include <chrono>
#include <future>

namespace Information_Model {

/**
 * @addtogroup ExecutableModeling Function Modelling
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

  MockFunction(DataType result_type, Function::ParameterTypes supported_params)
      : MockFunction(result_type, supported_params, setVariant(result_type)) {}

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
      ON_CALL(*this, call)
          .WillByDefault([this](Parameters /*parameters*/, uintmax_t timeout) {
            auto result = allocateAsyncCall();
            auto status =
                result.second.wait_for(std::chrono::milliseconds(timeout));
            if (status == std::future_status::ready) {
              return result.second.get();
            } else {
              cancelAsyncCall(result.first);
              throw FunctionCallTimedout("MockFunction");
            }
          });
      ON_CALL(*this, asyncCall)
          .WillByDefault([this](Function::Parameters /*params*/) {
            return allocateAsyncCall();
          });
    }
    ON_CALL(*this, cancelAsyncCall).WillByDefault([this](uintmax_t call_id) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_exception(
            std::make_exception_ptr(CallCanceled(call_id, "MockFunction")));
        iter = result_promises_.erase(iter);
      } else {
        throw CallerNotFound(call_id, "MockFunction");
      }
    });
    ON_CALL(*this, cancelAllAsyncCalls).WillByDefault([this]() {
      cancelAllCalls();
    });
    ON_CALL(*this, getResultDataType)
        .WillByDefault(::testing::Return(result_type_));
    ON_CALL(*this, getSupportedParameterTypes)
        .WillByDefault(::testing::Return(supported_params_));
  }

  ~MockFunction() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(void, execute, (Function::Parameters /*parameters*/), (override));
  MOCK_METHOD(DataVariant,
      call,
      (Function::Parameters /*parameters*/, uintmax_t /*timeout*/),
      (override));
  MOCK_METHOD(Function::ResultFuture,
      asyncCall,
      (Function::Parameters /*parameters*/),
      (override));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t /*call_id*/), (override));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (override));
  MOCK_METHOD(DataType, getResultDataType, (), (const, override));
  MOCK_METHOD(Function::ParameterTypes,
      getSupportedParameterTypes,
      (),
      (const, override));

  Function::ResultFuture allocateAsyncCall() {
    auto call_id = result_promises_.size();
    auto promise = std::promise<DataVariant>();
    auto result_future = std::make_pair(call_id, promise.get_future());
    result_promises_.emplace(call_id, std::move(promise));
    return std::move(result_future);
  }

  void respond(uintmax_t call_id, DataVariant value) {
    if (!executor_) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_value(value);
        iter = result_promises_.erase(iter);
      } else {
        throw CallerNotFound(call_id, "MockFunction");
      }
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respond(uintmax_t call_id, std::exception_ptr exception) {
    if (!executor_) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_exception(exception);
        iter = result_promises_.erase(iter);
      } else {
        throw CallerNotFound(call_id, "MockFunction");
      }
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respondToAll(DataVariant value) {
    if (!executor_) {
      for (auto iter = result_promises_.begin(); iter != result_promises_.end();
           iter++) {
        iter->second.set_value(value);
      }
      result_promises_.clear();
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void respondToAll(std::exception_ptr exception) {
    if (!executor_) {
      for (auto iter = result_promises_.begin(); iter != result_promises_.end();
           iter++) {
        iter->second.set_exception(exception);
      }
      result_promises_.clear();
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void cancelAllCalls() {
    if (!executor_) {
      for (auto iter = result_promises_.begin(); iter != result_promises_.end();
           iter++) {
        iter->second.set_exception(
            std::make_exception_ptr(CallCanceled(iter->first, "MockFunction")));
      }
      result_promises_.clear();
    } else {
      throw std::logic_error(
          "Execution calls are handled by an external function");
    }
  }

  void delegateToFake(Executor executor, Canceler canceler) {
    respondToAll(std::make_exception_ptr(
        std::logic_error("Assigned a new external execution handler")));
    if (executor && canceler) {
      executor_ = executor;
      canceler_ = canceler;
      ON_CALL(*this, call)
          .WillByDefault([this](Function::Parameters params,
                             uintmax_t timeout) -> DataVariant {
            auto result_future =
                std::async(std::launch::async, executor_, params);
            auto status =
                result_future.wait_for(std::chrono::milliseconds(timeout));
            if (status == std::future_status::ready) {
              return result_future.get().second.get();
            } else {
              throw FunctionCallTimedout("MockFunction");
            }
          });
      ON_CALL(*this, asyncCall).WillByDefault(executor_);
      ON_CALL(*this, cancelAsyncCall).WillByDefault(canceler_);
    } else {
      throw std::invalid_argument(
          "Both Executor and Canceler functors must be nonempty. Empty "
          "functors are not allowed, because Executor and Canceler functors "
          "are required to properly handle memory allocation for result "
          "futures");
    }
  }

  bool clearExpectations() { return ::testing::Mock::VerifyAndClear(this); }

private:
  DataType result_type_;
  Function::ParameterTypes supported_params_;
  std::optional<DataVariant> result_value_;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
  Executor executor_;
  Canceler canceler_;
};

namespace testing {
using MockFunctionPtr = std::shared_ptr<MockFunction>;
/** @}*/
} // namespace testing
} // namespace Information_Model

#endif //__INFORMATION_MODEL_FUNCTION_MOCK_HPP_