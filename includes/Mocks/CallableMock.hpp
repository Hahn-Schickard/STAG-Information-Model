#ifndef __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#include "Callable.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct CallableMock : public Callable {
  using ExecutorResult = std::pair<uintmax_t, std::future<DataVariant>>;
  using Executor = std::function<ExecutorResult(Callable::Parameters)>;
  using Canceler = std::function<void(uintmax_t)>;

  CallableMock() = default;

  explicit CallableMock(const Callable::ParameterTypes& supported_params);

  explicit CallableMock(DataType result_type);

  CallableMock(
      DataType result_type, const std::optional<DataVariant>& result_value);

  CallableMock(
      DataType result_type, const Callable::ParameterTypes& supported_params);

  CallableMock(DataType result_type,
      const Callable::ParameterTypes& supported_params,
      const std::optional<DataVariant>& result_value);

  ~CallableMock() override = default;

  MOCK_METHOD(void, execute, (const Parameters&), (const final));
  MOCK_METHOD(DataVariant, call, (uintmax_t), (const final));
  MOCK_METHOD(DataVariant, call, (const Parameters&, uintmax_t), (const final));
  MOCK_METHOD(ResultFuture, asyncCall, (const Parameters&), (const final));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t), (const final));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (const final));
  MOCK_METHOD(DataType, resultType, (), (const final));
  MOCK_METHOD(ParameterTypes, parameterTypes, (), (const final));

  void respond(uintmax_t call_id, const DataVariant& value);

  void respond(uintmax_t call_id, const std::exception_ptr& exception);

  void respondToAll(const DataVariant& value);

  void respondToAll(const std::exception_ptr& exception);

  void clearCall(uintmax_t call_id);

  void cancelAllCalls();

private:
  ResultFuture allocateAsyncCall();

  DataType result_type_;
  Callable::ParameterTypes supported_params_;
  std::optional<DataVariant> result_value_;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
  Executor executor_;
  Canceler canceler_;
};

using CallableMockPtr = std::shared_ptr<CallableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP