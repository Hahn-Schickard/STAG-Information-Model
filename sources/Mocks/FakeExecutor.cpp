#include "FakeExecutor.hpp"

#include <Stoppable/Task.hpp>
#include <Variant_Visitor/Visitor.hpp>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>

namespace Information_Model::testing {
using namespace std;

struct FakeExecutor : public Executor {
  FakeExecutor() = default;

  FakeExecutor(DataType result_type,
      const ParameterTypes& supported,
      const Executor::Response& default_response,
      chrono::nanoseconds response_delay)
      : result_type_(result_type), supported_params_(supported),
        default_response_(default_response), delay_(response_delay),
        task_(make_shared<Stoppable::Task>(
            bind(&FakeExecutor::respondOnce, this), [](const exception_ptr&) {
              // @todo decide how to handle exceptions
            })) {}

  ~FakeExecutor() override { cancelAll(); }

  shared_ptr<uintmax_t> assignID() {
    unique_lock lock(id_mx_);
    auto id = make_shared<uintmax_t>(0);
    while (!ids_.emplace(*id, id).second) {
      (*id)++;
    }
    return id;
  }

  void freeIDs() {
    unique_lock lock(id_mx_);
    for (auto it = ids_.begin(); it != ids_.end();) {
      if (it->second.expired()) {
        it = ids_.erase(it);
      } else {
        ++it;
      }
    }
  }

  void delayCall() const {
    if (delay_.count() > 0) {
      this_thread::sleep_for(delay_);
    }
  }

  void execute(const Parameters& params) final {
    delayCall();
    checkParameters(params, supported_params_);
  }

  ResultFuture asyncCall(const Parameters& params) final {
    if (result_type_ == DataType::None) {
      throw ResultReturningNotSupported();
    }
    auto call_id = assignID();
    promise<DataVariant> result_promise{};
    try {
      checkParameters(params, supported_params_);
    } catch (...) {
      result_promise.set_exception(current_exception());
      return ResultFuture(call_id, result_promise.get_future());
    }
    ResultFuture result_future(call_id, result_promise.get_future());
    result_promises_.try_emplace(*call_id, move(result_promise));
    {
      lock_guard lock(dispatch_mx_);
      to_be_dispatched_.push(*call_id);
      queue_not_empty_.notify_one();
    }
    return result_future;
  }

  void respond(uintmax_t call_id, const Response& response) final {
    delayCall();
    auto it = result_promises_.find(call_id);
    if (it != result_promises_.end()) {
      Variant_Visitor::match(
          response,
          [&it](const DataVariant& value) { it->second.set_value(value); },
          [&it](const exception_ptr& exception) {
            it->second.set_exception(exception);
          });
      result_promises_.erase(it);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void cancel(uintmax_t call_id) final {
    if (auto it = result_promises_.find(call_id);
        it != result_promises_.end()) {
      it->second.set_exception(
          make_exception_ptr(CallCanceled(call_id, "MockCallable")));
      it = result_promises_.erase(it);
    }
  }

  void cancelAll() final {
    for (auto& [promise_id, result_promise] : result_promises_) {
      result_promise.set_exception(
          make_exception_ptr(CallCanceled(promise_id, "MockCallable")));
    }
    result_promises_.clear();
  }

  DataType resultType() const final { return result_type_; }

  ParameterTypes parameterTypes() const final { return supported_params_; }

  void checkType(const Response& response) const {
    if (result_type_ == DataType::None &&
        holds_alternative<DataVariant>(response)) {
      throw invalid_argument("Can not set DataVariant response for executor. "
                             "Executor does not support returning values");
    }
  }

  void queueResponse(const Response& response) final {
    checkType(response);
    responses_queue_.push(response);
  }

  void queueResponse(uintmax_t call_id, const Response& response) final {
    checkType(response);
    responses_map_.try_emplace(call_id, response);
  }

  void respondOnce() final {
    unique_lock lock(dispatch_mx_);
    if (queue_not_empty_.wait_for(
            lock, 100us, [this]() { return !to_be_dispatched_.empty(); })) {
      auto dispatch_id = to_be_dispatched_.front();

      if (auto it = responses_map_.find(dispatch_id);
          it != responses_map_.end()) {
        respond(dispatch_id, it->second);
        it = responses_map_.erase(it);
      } else if (!responses_queue_.empty()) {
        auto response = responses_queue_.front();
        responses_queue_.pop();
        respond(dispatch_id, response);
      } else {
        respond(dispatch_id, default_response_);
      }
      to_be_dispatched_.pop();
    }
    freeIDs();
  }

  void start() final { task_->start(); }

  void stop() final { task_->stop(); }

private:
  DataType result_type_ = DataType::None;
  ParameterTypes supported_params_;
  Executor::Response default_response_;
  chrono::nanoseconds delay_;
  Stoppable::TaskPtr task_;
  unordered_map<uintmax_t, weak_ptr<uintmax_t>> ids_;
  mutex id_mx_;
  queue<Response> responses_queue_;
  unordered_map<uintmax_t, Response> responses_map_;
  mutex dispatch_mx_;
  condition_variable queue_not_empty_;
  queue<uintmax_t> to_be_dispatched_;
  unordered_map<uintmax_t, promise<DataVariant>> result_promises_;
};

ExecutorPtr makeExecutor(DataType result_type,
    const ParameterTypes& supported_params,
    const Executor::Response& default_response,
    chrono::nanoseconds delay) {
  return make_shared<FakeExecutor>(
      result_type, supported_params, default_response, delay);
}
} // namespace Information_Model::testing
