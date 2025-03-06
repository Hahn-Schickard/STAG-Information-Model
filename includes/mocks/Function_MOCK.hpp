#ifndef __INFORMATION_MODEL_FUNCTION_MOCK_HPP_
#define __INFORMATION_MODEL_FUNCTION_MOCK_HPP_

#include "../Function.hpp"

#include "gmock/gmock.h"
#include <chrono>
#include <future>

namespace Information_Model::testing {
/**
 * @addtogroup ExecutableModeling Function Modelling
 * @{
 */
/**
 * @brief Function mock implementation with default fake method implementations
 * and ability to add custom fake functionality
 *
 * @attention
 * Use only for testing
 *
 */
struct MockFunction : public Function {
  using ExecutorResult = std::pair<uintmax_t, std::future<DataVariant>>;
  using Executor = std::function<ExecutorResult(Function::Parameters)>;
  using Canceler = std::function<void(uintmax_t)>;

  MockFunction() : MockFunction(DataType::None) {}

  explicit MockFunction(const Function::ParameterTypes& supported_params)
      : MockFunction(DataType::None, supported_params, std::nullopt) {}

  explicit MockFunction(DataType result_type)
      : MockFunction(result_type, Function::ParameterTypes(), std::nullopt) {}

  MockFunction(
      DataType result_type, const std::optional<DataVariant>& result_value)
      : MockFunction(result_type, Function::ParameterTypes(), result_value) {}

  MockFunction(
      DataType result_type, const Function::ParameterTypes& supported_params)
      : MockFunction(result_type, supported_params, setVariant(result_type)) {}

  MockFunction(DataType result_type,
      const Function::ParameterTypes& supported_params,
      const std::optional<DataVariant>& result_value)
      : Function(result_type, supported_params), result_type_(result_type),
        supported_params_(supported_params), result_value_(result_value) {
    if (!result_value_.has_value()) {
      ON_CALL(*this, call)
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
      ON_CALL(*this, asyncCall)
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
      ON_CALL(*this, cancelAsyncCall)
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
      ON_CALL(*this, cancelAllAsyncCalls)
          .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
    } else {
      ON_CALL(*this, call)
          .WillByDefault(::testing::Return(result_value.value()));
      ON_CALL(*this, asyncCall)
          .WillByDefault([this](const Function::Parameters& /*params*/) {
            return allocateAsyncCall();
          });
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
    }
  }

  ~MockFunction() { ::testing::Mock::VerifyAndClear(this); }

  MOCK_METHOD(
      void, execute, (const Function::Parameters& /*parameters*/), (override));
  MOCK_METHOD(DataVariant,
      call,
      (const Function::Parameters& /*parameters*/, uintmax_t /*timeout*/),
      (override));
  MOCK_METHOD(Function::ResultFuture,
      asyncCall,
      (const Function::Parameters& /*parameters*/),
      (override));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t /*call_id*/), (override));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (override));

  void clearCall(uintmax_t call_id) { result_promises_.erase(call_id); }

  Function::ResultFuture allocateAsyncCall() {
    auto call_id = result_promises_.size();
    auto promise = std::promise<DataVariant>();
    auto result_future = ResultFuture(promise.get_future(),
        call_id,
        std::bind(&MockFunction::clearCall, this, std::placeholders::_1));
    result_promises_.emplace(call_id, std::move(promise));
    return result_future;
  }

  void respond(uintmax_t call_id, const DataVariant& value) {
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

  void respond(uintmax_t call_id, const std::exception_ptr& exception) {
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

  void respondToAll(const DataVariant& value) {
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

  void respondToAll(const std::exception_ptr& exception) {
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

  /**
   * @brief Set a custom Executor to invoke custom callback
   *
   * @param executor - custom execution callback
   * @param canceler - custom cancel execution callback
   */
  void delegateToFake(
      const Executor& executor, const Canceler& canceler = nullptr) {
    respondToAll(std::make_exception_ptr(
        std::logic_error("Assigned a new external execution handler")));
    if (executor) {
      executor_ = executor;
      canceler_ = canceler;
      ON_CALL(*this, execute)
          .WillByDefault([this](const Function::Parameters& params) {
            auto result = executor_(params);
          });

      if (canceler_) {
        ON_CALL(*this, call)
            .WillByDefault([this](const Function::Parameters& params,
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
        ON_CALL(*this, asyncCall)
            .WillByDefault([this](const Function::Parameters& params) {
              auto [caller_id, result_future] = executor_(params);
              return ResultFuture(std::move(result_future),
                  caller_id,
                  std::bind(
                      &MockFunction::clearCall, this, std::placeholders::_1));
            });
        ON_CALL(*this, cancelAsyncCall).WillByDefault(canceler_);
      } else {
        ON_CALL(*this, call)
            .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
        ON_CALL(*this, asyncCall)
            .WillByDefault(::testing::Throw(ResultReturningNotSupported()));
      }
    } else {
      throw std::invalid_argument("Executor callable MUST NOT be empty");
    }
  }

  /**
   * @brief Use internal responder mechanism instead of default value returner
   * or custom Executor
   *
   * Use MockFunction::respond() or MockFunction::respondToAll() to set the
   * response
   *
   * This will cause the Function::call() to throw FunctionCallTimedout, if no
   * response has been set before the call timeout has occurred,
   */
  void delegateToFake() {
    ON_CALL(*this, call)
        .WillByDefault(
            [this](const Parameters& /*parameters*/, uintmax_t timeout) {
              auto result = allocateAsyncCall();
              auto status = result.wait_for(std::chrono::milliseconds(timeout));
              if (status == std::future_status::ready) {
                return result.get();
              } else {
                cancelAsyncCall(result.call_id);
                throw FunctionCallTimedout("MockFunction");
              }
            });
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

using MockFunctionPtr = std::shared_ptr<MockFunction>;
using NonemptyMockFunctionPtr = Nonempty::Pointer<MockFunctionPtr>;
/** @}*/
} // namespace Information_Model::testing

#endif //__INFORMATION_MODEL_FUNCTION_MOCK_HPP_