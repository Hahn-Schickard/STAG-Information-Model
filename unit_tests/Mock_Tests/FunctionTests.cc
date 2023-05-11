#include "Function_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>
#include <thread>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct FunctionExpectations {
  const std::string name_;
  const DataType result_type_;
  const Function::ParameterTypes supported_params_;
  std::optional<DataVariant> result_value_;

  FunctionExpectations(const std::string& name)
      : FunctionExpectations(
            name, DataType::UNKNOWN, Function::ParameterTypes(), std::nullopt) {
  }

  FunctionExpectations(
      const std::string& name, Function::ParameterTypes supported_params)
      : FunctionExpectations(
            name, DataType::UNKNOWN, supported_params, std::nullopt) {}

  FunctionExpectations(const std::string& name, DataType result_type)
      : FunctionExpectations(name,
            result_type,
            Function::ParameterTypes(),
            setVariant(result_type)) {}

  FunctionExpectations(
      const std::string& name, DataType result_type, DataVariant result_value)
      : FunctionExpectations(
            name, result_type, Function::ParameterTypes(), result_value) {}

  FunctionExpectations(const std::string& name,
      DataType result_type,
      Function::ParameterTypes supported_params,
      std::optional<DataVariant> result_value)
      : name_(name), result_type_(result_type),
        supported_params_(supported_params), result_value_(result_value) {}
};

using FunctionExpectationsPtr = shared_ptr<FunctionExpectations>;

class FunctionParametrizedTests
    : public ::testing::TestWithParam<FunctionExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<FunctionExpectations>(GetParam());
    function_mock = make_shared<MockFunction>(expectations->result_type_,
        expectations->supported_params_,
        expectations->result_value_);
    function = function_mock;
  }

  FunctionExpectationsPtr expectations;
  MockFunctionPtr function_mock;
  FunctionPtr function;
};

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canExecute) {
  EXPECT_CALL(*function_mock.get(), execute(::testing::_)).Times(AtLeast(1));
  ASSERT_NO_THROW(function->execute());
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canCall) {
  EXPECT_CALL(*function_mock.get(), call(::testing::_, ::testing::_))
      .Times(AtLeast(1));
  try {
    auto resul_future = std::async(
        std::launch::async, [this]() { return function->call(1000); });
    function_mock->respondToAll(
        expectations->result_value_.value_or(DataVariant()));
    auto result = resul_future.get();
    EXPECT_EQ(expectations->result_value_, result);
  } catch (exception& ex) {
    if (expectations->result_type_ != DataType::UNKNOWN) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canCallTimesOut) {
  EXPECT_CALL(*function_mock.get(), call(::testing::_, ::testing::_))
      .Times(AtLeast(1));
  if (expectations->result_type_ != DataType::UNKNOWN) {
    auto resul_future =
        std::async(std::launch::async, [this]() { return function->call(1); });
    std::this_thread::sleep_for(10ms);
    EXPECT_THROW(resul_future.get(), FunctionCallTimedout);
  } else {
    EXPECT_THROW(function->call(), ResultReturningNotSupported);
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canAsyncCall) {
  EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_)).Times(AtLeast(1));
  try {
    function->asyncCall();
  } catch (const exception& ex) {
    if (expectations->result_type_ != DataType::UNKNOWN) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canCancelAsyncCall) {
  EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_)).Times(AtLeast(1));
  try {
    auto future_result = function->asyncCall();
    EXPECT_CALL(*function_mock.get(), cancelAsyncCall(::testing::_))
        .Times(AtLeast(1));
    function->cancelAsyncCall(future_result.first);
    EXPECT_THROW(future_result.second.get(), CallCanceled);
  } catch (const exception& ex) {
    if (expectations->result_type_ != DataType::UNKNOWN) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, throwsCallerNotFoundOnCancelAsyncCall) {
  EXPECT_CALL(*function_mock.get(), cancelAsyncCall(::testing::_))
      .Times(AtLeast(1));
  EXPECT_THROW(function->cancelAsyncCall(202020202), CallerNotFound);
}

struct Executor {
  Executor(const Executor&) = delete;

  Executor() = default;
  /**
   * @brief Create a new Executor Functor with an execution delay
   *
   * @param response_delay - number of miliseconds to wait before responding
   */
  Executor(uintmax_t response_delay) : response_delay_(response_delay) {}

  Executor& operator()(const Executor&) = delete;

  Function::ResultFuture execute(Function::Parameters /*params*/) {
    auto call_id = result_promises_.size();
    auto promise = std::promise<DataVariant>();
    auto result_future = std::make_pair(call_id, promise.get_future());
    result_promises_.emplace(call_id, std::move(promise));
    return std::move(result_future);
  }

  void cancel(uintmax_t call_id) {
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_exception(
          std::make_exception_ptr(CallCanceled(call_id, "ExternalExecutor")));
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respond(uintmax_t call_id, DataVariant value = DataVariant()) {
    if (response_delay_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(response_delay_));
    }
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_value(value);
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respond(uintmax_t call_id, std::exception_ptr exception) {
    if (response_delay_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(response_delay_));
    }
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_exception(exception);
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respondToAll(DataVariant value) {
    for (auto iter = result_promises_.begin(); iter != result_promises_.end();
         iter++) {
      iter->second.set_value(value);
    }
    result_promises_.clear();
  }

  void respondToAll(std::exception_ptr exception) {
    for (auto iter = result_promises_.begin(); iter != result_promises_.end();
         iter++) {
      iter->second.set_exception(exception);
    }
    result_promises_.clear();
  }

private:
  uintmax_t response_delay_ = 0;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
};

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canUseExternalCall) {
  if (expectations->result_type_ != DataType::UNKNOWN) {
    auto executor = Executor();
    MockFunction::Executor execute_cb = [&executor](
                                            Function::Parameters params) {
      return executor.execute(params);
    };
    MockFunction::Canceler cancel_cb = [&executor](uintmax_t call_id) {
      executor.cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);

    EXPECT_CALL(*function_mock.get(), call(::testing::_, ::testing::_))
        .Times(AtLeast(2));

    auto call_result_future =
        std::async(std::launch::async, [this]() { return function->call(); });
    executor.respondToAll(expectations->result_value_.value());

    try {
      auto call_result = call_result_future.get();
      EXPECT_EQ(expectations->result_value_.value(), call_result);
    } catch (const exception& ex) {
      FAIL() << "Caught an unexpected exception" << ex.what();
    }

    auto call_exception_future =
        std::async(std::launch::async, [this]() { return function->call(); });
    executor.respondToAll(
        std::make_exception_ptr(std::domain_error("Test exception throwing")));

    EXPECT_THROW(call_exception_future.get(), std::domain_error);
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canUseExternalAsyncCall) {
  if (expectations->result_type_ != DataType::UNKNOWN) {
    auto executor = Executor();
    MockFunction::Executor execute_cb = [&executor](
                                            Function::Parameters params) {
      return executor.execute(params);
    };
    MockFunction::Canceler cancel_cb = [&executor](uintmax_t call_id) {
      executor.cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);

    EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_))
        .Times(AtLeast(2));

    auto async_call_result_future = function->asyncCall();
    executor.respondToAll(expectations->result_value_.value());

    try {
      auto async_call_result = async_call_result_future.second.get();
      EXPECT_EQ(expectations->result_value_.value(), async_call_result);
    } catch (const exception& ex) {
      FAIL() << "Caught an unexpected exception" << ex.what();
    }

    auto async_call_exception_future = function->asyncCall();
    executor.respondToAll(
        std::make_exception_ptr(std::domain_error("Test exception throwing")));

    EXPECT_THROW(async_call_exception_future.second.get(), std::domain_error);
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canUseExternalCancelAsyncCall) {
  if (expectations->result_type_ != DataType::UNKNOWN) {
    auto executor = Executor();
    MockFunction::Executor execute_cb = [&executor](
                                            Function::Parameters params) {
      return executor.execute(params);
    };
    MockFunction::Canceler cancel_cb = [&executor](uintmax_t call_id) {
      executor.cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);

    EXPECT_CALL(*function_mock.get(), cancelAsyncCall(::testing::_))
        .Times(AtLeast(2));

    auto async_call_result_future = function->asyncCall();

    EXPECT_NO_THROW(function->cancelAsyncCall(async_call_result_future.first));

    EXPECT_THROW(function->cancelAsyncCall(202020202), CallerNotFound);
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, throwsLogicErrorOnExternalExecutorSet) {
  EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_)).Times(AtLeast(1));
  try {
    auto future_result = function->asyncCall();
    auto executor = Executor();
    MockFunction::Executor execute_cb = [&executor](
                                            Function::Parameters params) {
      return executor.execute(params);
    };
    MockFunction::Canceler cancel_cb = [&executor](uintmax_t call_id) {
      executor.cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);
    EXPECT_THROW(future_result.second.get(), std::logic_error);
  } catch (const exception& ex) {
    if (expectations->result_type_ != DataType::UNKNOWN) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canGetResultDataType) {
  EXPECT_CALL(*function_mock.get(), getResultDataType()).Times(AtLeast(1));
  EXPECT_EQ(expectations->result_type_, function->getResultDataType());
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canGetSupportedParameterTypes) {
  EXPECT_CALL(*function_mock.get(), getSupportedParameterTypes())
      .Times(AtLeast(1));
  EXPECT_EQ(
      expectations->supported_params_, function->getSupportedParameterTypes());
}

struct SetMetricTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name_;
  }
};

vector<FunctionExpectations> makeFunctionTestParameters() {
  vector<FunctionExpectations> params;
  params.emplace_back("doAndReturnNothing");
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(FunctionTests,
    FunctionParametrizedTests,
    ::testing::ValuesIn(makeFunctionTestParameters()),
    SetMetricTestNameSuffix());
