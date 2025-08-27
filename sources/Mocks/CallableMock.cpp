#include "CallableMock.hpp"

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

CallableMock::CallableMock(const Callable::ParameterTypes& supported_params)
    : CallableMock(DataType::None, supported_params, nullopt) {}

CallableMock::CallableMock(DataType result_type)
    : CallableMock(result_type, Callable::ParameterTypes(), nullopt) {}

CallableMock::CallableMock(
    DataType result_type, const optional<DataVariant>& result_value)
    : CallableMock(result_type, Callable::ParameterTypes(), result_value) {}

CallableMock::CallableMock(
    DataType result_type, const Callable::ParameterTypes& supported_params)
    : CallableMock(result_type, supported_params, setVariant(result_type)) {}

CallableMock::CallableMock(DataType result_type,
    const Callable::ParameterTypes& supported_params,
    const optional<DataVariant>& result_value)
    : result_type_(result_type), supported_params_(supported_params),
      result_value_(result_value) {
  if (!result_value_.has_value()) {
    ON_CALL(*this, call(_)).WillByDefault(Throw(ResultReturningNotSupported()));
    ON_CALL(*this, call(_, _))
        .WillByDefault(Throw(ResultReturningNotSupported()));
    ON_CALL(*this, asyncCall)
        .WillByDefault(Throw(ResultReturningNotSupported()));
    ON_CALL(*this, cancelAsyncCall)
        .WillByDefault(Throw(ResultReturningNotSupported()));
    ON_CALL(*this, cancelAllAsyncCalls)
        .WillByDefault(Throw(ResultReturningNotSupported()));
  } else {
    ON_CALL(*this, call(_)).WillByDefault(Return(result_value.value()));
    ON_CALL(*this, call(_, _)).WillByDefault(Return(result_value.value()));
    ON_CALL(*this, asyncCall)
        .WillByDefault([this](const Callable::Parameters& /*params*/) {
          return allocateAsyncCall();
        });
    ON_CALL(*this, cancelAsyncCall).WillByDefault([this](uintmax_t call_id) {
      auto iter = result_promises_.find(call_id);
      if (iter != result_promises_.end()) {
        iter->second.set_exception(
            make_exception_ptr(CallCanceled(call_id, "MockFunction")));
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

void CallableMock::respond(uintmax_t call_id, const DataVariant& value) {
  if (executor_) {
    throw logic_error("Execution calls are handled by an external function");
  }

  auto it = result_promises_.find(call_id);
  if (it != result_promises_.end()) {
    it->second.set_value(value);
    it = result_promises_.erase(it);
  } else {
    throw CallerNotFound(call_id, "MockCallable");
  }
}

void CallableMock::respond(uintmax_t call_id, const exception_ptr& exception) {
  if (executor_) {
    throw logic_error("Execution calls are handled by an external function");
  }

  auto it = result_promises_.find(call_id);
  if (it != result_promises_.end()) {
    it->second.set_exception(exception);
    it = result_promises_.erase(it);
  } else {
    throw CallerNotFound(call_id, "MockCallable");
  }
}

void CallableMock::respondToAll(const DataVariant& value) {
  if (executor_) {
    throw logic_error("Execution calls are handled by an external function");
  }

  for (auto it = result_promises_.begin(); it != result_promises_.end(); it++) {
    it->second.set_value(value);
  }
  result_promises_.clear();
}

void CallableMock::respondToAll(const exception_ptr& exception) {
  if (executor_) {
    throw logic_error("Execution calls are handled by an external function");
  }

  for (auto it = result_promises_.begin(); it != result_promises_.end(); it++) {
    it->second.set_exception(exception);
  }
  result_promises_.clear();
}

void CallableMock::clearCall(uintmax_t call_id) {
  result_promises_.erase(call_id);
}

void CallableMock::cancelAllCalls() {
  if (executor_) {
    throw logic_error("Execution calls are handled by an external function");
  }

  for (auto it = result_promises_.begin(); it != result_promises_.end(); it++) {
    it->second.set_exception(
        make_exception_ptr(CallCanceled(it->first, "MockCallable")));
  }
  result_promises_.clear();
}

ResultFuture CallableMock::allocateAsyncCall() {
  auto call_id = result_promises_.size();
  auto result_promise = promise<DataVariant>();
  ResultFuture result_future{call_id,
      result_promise.get_future(),
      bind(&CallableMock::clearCall, this, placeholders::_1)};
  result_promises_.emplace(call_id, move(result_promise));
  return result_future;
}

} // namespace Information_Model::testing