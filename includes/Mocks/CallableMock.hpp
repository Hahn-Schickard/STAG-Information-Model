#ifndef __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#include "Callable.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct CallableMock : virtual public Callable {
  CallableMock() = default;

  ~CallableMock() override = default;

  MOCK_METHOD(void, execute, (const Parameters&), (const final));
  MOCK_METHOD(DataVariant, call, (uintmax_t), (const final));
  MOCK_METHOD(DataVariant, call, (const Parameters&, uintmax_t), (const final));
  MOCK_METHOD(ResultFuture, asyncCall, (const Parameters&), (const final));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t), (const final));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (const final));
  MOCK_METHOD(DataType, resultType, (), (const final));
  MOCK_METHOD(ParameterTypes, parameterTypes, (), (const final));
};

using CallableMockPtr = std::shared_ptr<CallableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP