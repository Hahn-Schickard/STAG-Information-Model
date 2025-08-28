#ifndef __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#define __STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP
#include "Callable.hpp"

#include <gmock/gmock.h>

namespace Information_Model::testing {

struct Executor {
  virtual ~Executor() = default;

  virtual void execute(const Callable::Parameters& params) = 0;

  virtual ResultFuture assignCall(const Callable::Parameters& params) = 0;

  // nullopt disables autoresponse mechanism
  virtual void autoRespond(const std::optional<DataVariant>& value) = 0;

  virtual void respond(uintmax_t call_id, const DataVariant& value) = 0;

  virtual void respond(
      uintmax_t call_id, const std::exception_ptr& exception) = 0;

  virtual void respondToAll(const DataVariant& value) = 0;

  virtual void respondToAll(const std::exception_ptr& exception) = 0;

  virtual void cancel(uintmax_t call_id) = 0;

  virtual void cancelAll() = 0;
};

using ExecutorPtr = std::shared_ptr<Executor>;

struct CallableMock : public Callable {
  CallableMock();

  explicit CallableMock(DataType result_type);

  explicit CallableMock(const Callable::ParameterTypes& supported_params);

  CallableMock(
      DataType result_type, const Callable::ParameterTypes& supported_params);

  ~CallableMock() override = default;

  MOCK_METHOD(void, execute, (const Parameters&), (const final));
  MOCK_METHOD(DataVariant, call, (uintmax_t), (const final));
  MOCK_METHOD(DataVariant, call, (const Parameters&, uintmax_t), (const final));
  MOCK_METHOD(ResultFuture, asyncCall, (const Parameters&), (const final));
  MOCK_METHOD(void, cancelAsyncCall, (uintmax_t), (const final));
  MOCK_METHOD(void, cancelAllAsyncCalls, (), (const final));
  MOCK_METHOD(DataType, resultType, (), (const final));
  MOCK_METHOD(ParameterTypes, parameterTypes, (), (const final));

  ExecutorPtr getExecutor();

  void changeExecutor(const ExecutorPtr& executor,
      DataType result_type,
      const Callable::ParameterTypes& supported_params);

  void useDefaultExecutor(
      std::chrono::milliseconds response_delay = std::chrono::milliseconds(
          100));

private:
  void setExecutor();

  DataType result_type_;
  Callable::ParameterTypes supported_params_;
  ExecutorPtr executor_;
};

using CallableMockPtr = std::shared_ptr<CallableMock>;
} // namespace Information_Model::testing
#endif //__STAG_INFORMATION_MODEL_CALLABLE_MOCK_HPP