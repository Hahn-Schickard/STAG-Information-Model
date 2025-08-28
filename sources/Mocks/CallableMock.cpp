#include "CallableMock.hpp"

#include <Stoppable/Task.hpp>

#include <thread>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct ExecutorImpl : public Executor {
  ExecutorImpl(const ExecutorImpl&) = delete;

  ExecutorImpl& operator()(const ExecutorImpl&) = delete;

  ~ExecutorImpl() override = default;

  ExecutorImpl(chrono::milliseconds response_delay) : delay_(response_delay) {}

  void execute(const Callable::Parameters& /*params*/) final {
    if (delay_.count() > 0) {
      this_thread::sleep_for((delay_));
    }
  }

  ResultFuture assignCall(const Callable::Parameters& /*params*/) final {
    auto execute_lock = lock_guard(execute_mx_);
    auto call_id = result_promises_.size();
    auto result_promise = promise<DataVariant>();
    ResultFuture result_future{call_id,
        result_promise.get_future(),
        bind(&Executor::cancel, this, placeholders::_1)};
    result_promises_.emplace(call_id, move(result_promise));
    return result_future;
  }

  void autoRespond(const optional<DataVariant>& value) final {
    if (value) {
      if (!responder_) {
        responder_ = make_shared<Stoppable::Task>(
            [this]() { respondToAll(response_value_.value()); },
            [](const exception_ptr&) {
              // @todo decide how to handle thrown exceptions
            });
        responder_->start();
      }
    } else {
      if (!responder_) {
        responder_->stop();
      }
    }
  }

  void respond(uintmax_t call_id, const DataVariant& value) final {
    if (delay_.count() > 0) {
      this_thread::sleep_for(delay_);
    }
    auto it = result_promises_.find(call_id);
    if (it != result_promises_.end()) {
      it->second.set_value(value);
      auto clear_lock = lock_guard(erase_mx_);
      it = result_promises_.erase(it);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respond(uintmax_t call_id, const exception_ptr& exception) final {
    if (delay_.count() > 0) {
      this_thread::sleep_for(delay_);
    }
    auto it = result_promises_.find(call_id);
    if (it != result_promises_.end()) {
      it->second.set_exception(exception);
      auto clear_lock = lock_guard(erase_mx_);
      it = result_promises_.erase(it);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respondToAll(const DataVariant& value) final {
    for (auto& result_promise : result_promises_) {
      result_promise.second.set_value(value);
    }
    auto clear_lock = lock_guard(erase_mx_);
    result_promises_.clear();
  }

  void respondToAll(const exception_ptr& exception) final {
    for (auto& result_promise : result_promises_) {
      result_promise.second.set_exception(exception);
    }
    auto clear_lock = lock_guard(erase_mx_);
    result_promises_.clear();
  }

  void cancel(uintmax_t call_id) final {
    auto it = result_promises_.find(call_id);
    if (it != result_promises_.end()) {
      it->second.set_exception(
          make_exception_ptr(CallCanceled(call_id, "ExternalExecutor")));
      auto clear_lock = lock_guard(erase_mx_);
      it = result_promises_.erase(it);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void cancelAll() final {
    auto clear_lock = lock_guard(erase_mx_);
    // NOLINTNEXTLINE(modernize-loop-convert)
    for (auto it = result_promises_.begin(); it != result_promises_.end();
        it++) {
      // ResultFuture is non copyable, must access it via itators
      it->second.set_exception(
          make_exception_ptr(CallCanceled(it->first, "MockCallable")));
    }
    result_promises_.clear();
  }

private:
  mutex execute_mx_; // @todo: check if both mutexes are needed and used
                     // correctly
  mutex erase_mx_;
  chrono::milliseconds delay_;
  Stoppable::TaskPtr responder_;
  optional<DataVariant> response_value_;
  unordered_map<uintmax_t, promise<DataVariant>> result_promises_;
};

CallableMock::CallableMock() {
  // @todo decide on default behaviour
}

CallableMock::CallableMock(DataType result_type)
    : CallableMock(result_type, Callable::ParameterTypes()) {}

CallableMock::CallableMock(const Callable::ParameterTypes& supported_params)
    : CallableMock(DataType::None, supported_params) {}

CallableMock::CallableMock(
    DataType result_type, const Callable::ParameterTypes& supported_params)
    : result_type_(result_type), supported_params_(supported_params) {
  useDefaultExecutor();
}

ExecutorPtr CallableMock::getExecutor() { return executor_; }

void CallableMock::changeExecutor(const ExecutorPtr& executor,
    DataType result_type,
    const Callable::ParameterTypes& supported_params) {
  if (executor_) {
    executor_->respondToAll(make_exception_ptr(
        logic_error("Assigned a new external execution handler")));
  }

  executor_ = executor;
  result_type_ = result_type;
  supported_params_ = supported_params;

  setExecutor();
}

void CallableMock::useDefaultExecutor(chrono::milliseconds response_delay) {
  executor_ = make_shared<ExecutorImpl>(response_delay);
  setExecutor();
}

void CallableMock::setExecutor() {
  ON_CALL(*this, resultType).WillByDefault(Return(result_type_));
  ON_CALL(*this, parameterTypes).WillByDefault(Return(supported_params_));

  if (executor_) {
    ON_CALL(*this, execute)
        .WillByDefault([this](const Callable::Parameters& params) {
          executor_->execute(params);
        });
    if (result_type_ != DataType::None) {
      ON_CALL(*this, call(_))
          .WillByDefault([this](uintmax_t timeout) -> DataVariant {
            auto result_future = executor_->assignCall(Callable::Parameters{});
            auto status = result_future.wait_for(chrono::milliseconds(timeout));
            if (status == future_status::ready) {
              return result_future.get();
            } else {
              throw CallTimedout("MockFunction");
            }
          });
      ON_CALL(*this, call(_, _))
          .WillByDefault([this](const Callable::Parameters& params,
                             uintmax_t timeout) -> DataVariant {
            auto result_future = executor_->assignCall(params);
            auto status = result_future.wait_for(chrono::milliseconds(timeout));
            if (status == future_status::ready) {
              return result_future.get();
            } else {
              throw CallTimedout("MockFunction");
            }
          });
      ON_CALL(*this, asyncCall)
          .WillByDefault(
              bind(&Executor::assignCall, executor_, placeholders::_1));
    } else {
      ON_CALL(*this, call(_))
          .WillByDefault(Throw(ResultReturningNotSupported()));
      ON_CALL(*this, call(_, _))
          .WillByDefault(Throw(ResultReturningNotSupported()));
      ON_CALL(*this, asyncCall)
          .WillByDefault(Throw(ResultReturningNotSupported()));
    }
  } else {
    ON_CALL(*this, execute).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, call(_)).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, call(_, _)).WillByDefault(Throw(ExecutorNotAvailable()));
    ON_CALL(*this, asyncCall).WillByDefault(Throw(ExecutorNotAvailable()));
  }
}

} // namespace Information_Model::testing