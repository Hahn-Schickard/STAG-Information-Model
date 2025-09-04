#include "CallableMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

CallableMock::CallableMock(DataType result_type,
    const Callable::ParameterTypes& supported_params,
    const Executor::Response& default_response)
    : result_type_(result_type), supported_params_(supported_params),
      default_response_(default_response),
      executor_(makeExecutor(
          result_type_, supported_params_, default_response_, 100ms)) {
  setExecutor();
}

ExecutorPtr CallableMock::getExecutor() const { return executor_; }

void CallableMock::changeExecutor(const ExecutorPtr& executor) {
  if (executor_) {
    executor_->cancelAll();
  }

  executor_ = executor;

  setExecutor();
}

void CallableMock::useDefaultExecutor() {
  executor_ =
      makeExecutor(result_type_, supported_params_, default_response_, 100ms);
  setExecutor();
}

void CallableMock::setExecutor() {
  if (executor_) {
    ON_CALL(*this, resultType)
        .WillByDefault(Invoke(executor_.get(), &Executor::resultType));
    ON_CALL(*this, parameterTypes)
        .WillByDefault(Invoke(executor_.get(), &Executor::parameterTypes));
    ON_CALL(*this, execute)
        .WillByDefault([this](const Callable::Parameters& params) {
          executor_->execute(params);
        });
    ON_CALL(*this, call(_)).WillByDefault([this](uintmax_t timeout) {
      auto result_future = executor_->asyncCall(Callable::Parameters{});
      auto status = result_future.wait_for(chrono::milliseconds(timeout));
      if (status == future_status::ready) {
        return result_future.get();
      } else {
        throw CallTimedout("MockFunction");
      }
    });
    ON_CALL(*this, call(_, _))
        .WillByDefault([this](const Callable::Parameters& params,
                           uintmax_t timeout) {
          auto result_future = executor_->asyncCall(params);
          auto status = result_future.wait_for(chrono::milliseconds(timeout));
          if (status == future_status::ready) {
            return result_future.get();
          } else {
            throw CallTimedout("MockFunction");
          }
        });
    ON_CALL(*this, asyncCall)
        .WillByDefault(bind(&Executor::asyncCall, executor_, placeholders::_1));
  } else {
    ON_CALL(*this, resultType).WillByDefault(Return(result_type_));
    ON_CALL(*this, parameterTypes).WillByDefault(Return(supported_params_));
    ON_CALL(*this, execute).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, call(_)).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, call(_, _)).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, asyncCall).WillByDefault(Throw(ExecutorNotAvailable()));
  }
}

} // namespace Information_Model::testing