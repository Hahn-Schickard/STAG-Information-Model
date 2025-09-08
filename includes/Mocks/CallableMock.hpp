#ifndef __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP

#include "ExecutorMock.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct CallableMock : public Callable {
  CallableMock() = default;

  explicit CallableMock(DataType result_type,
      const ParameterTypes& supported_params = {},
      const Executor::Response& default_response = std::make_exception_ptr(
          std::logic_error("Default response exception")));

  ~CallableMock() override = default;

  MOCK_METHOD(void, execute, (const Parameters& parameters), (const final));
  MOCK_METHOD(DataVariant, call, (uintmax_t timeout), (const final));
  MOCK_METHOD(DataVariant,
      call,
      (const Parameters& parameters, uintmax_t timeout),
      (const final));
  MOCK_METHOD(
      ResultFuture, asyncCall, (const Parameters& parameters), (const final));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t), (const final));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (const final));
  MOCK_METHOD(DataType, resultType, (), (const final));
  MOCK_METHOD(ParameterTypes, parameterTypes, (), (const final));

  ExecutorPtr getExecutor() const;

  void changeExecutor(const ExecutorPtr& executor);
  void useDefaultExecutor();

private:
  void setExecutor();

  DataType result_type_;
  ParameterTypes supported_params_;
  Executor::Response default_response_;
  ExecutorPtr executor_;
};

using CallableMockPtr = std::shared_ptr<CallableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP