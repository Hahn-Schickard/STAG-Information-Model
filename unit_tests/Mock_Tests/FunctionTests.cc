#include "Function_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <regex>
#include <string>
#include <thread>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct FunctionExpectations {
  const std::string name;
  const DataType result_type;
  const Function::ParameterTypes supported_params;
  std::optional<DataVariant> result_value;

  FunctionExpectations(const std::string& name)
      : FunctionExpectations(
            name, DataType::NONE, Function::ParameterTypes(), std::nullopt) {}

  FunctionExpectations(
      const std::string& name, const Function::ParameterTypes& supported_params)
      : FunctionExpectations(
            name, DataType::NONE, supported_params, std::nullopt) {}

  FunctionExpectations(const std::string& name, DataType result_type)
      : FunctionExpectations(name,
            result_type,
            Function::ParameterTypes(),
            setVariant(result_type).value()) {}

  FunctionExpectations(const std::string& name,
      DataType result_type,
      const DataVariant& result_value)
      : FunctionExpectations(
            name, result_type, Function::ParameterTypes(), result_value) {}

  // NOLINTNEXTLINE(modernize-pass-by-value)
  FunctionExpectations(const std::string& expectation_name,
      DataType expectation_result_type,
      // NOLINTNEXTLINE(modernize-pass-by-value)
      const Function::ParameterTypes& expectation_params,
      // NOLINTNEXTLINE(modernize-pass-by-value)
      const std::optional<DataVariant>& expectation_result)
      : name(expectation_name), result_type(expectation_result_type),
        supported_params(expectation_params), result_value(expectation_result) {
  }
};

using FunctionExpectationsPtr = shared_ptr<FunctionExpectations>;

class FunctionParametrizedTests
    : public ::testing::TestWithParam<FunctionExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<FunctionExpectations>(GetParam());
    function_mock = make_shared<MockFunction>(expectations->result_type,
        expectations->supported_params,
        expectations->result_value);
    function = function_mock;
  }

  FunctionExpectationsPtr expectations; // NOLINT(readability-identifier-naming)
  MockFunctionPtr function_mock; // NOLINT(readability-identifier-naming)
  FunctionPtr function; // NOLINT(readability-identifier-naming)
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
    auto resul_future = std::async(std::launch::async, [this]() {
      // NOLINTNEXTLINE(readability-magic-numbers)
      return function->call(1000);
    });
    this_thread::sleep_for(
        100ms); // if we do not give enough time for the thread to allocate the
                // response future, we will respond before a call was created,
                // thus hanging the test
    auto result = resul_future.get();
    EXPECT_EQ(expectations->result_value, result);
  } catch (const exception& ex) {
    if (expectations->result_type != DataType::NONE) {
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
  if (expectations->result_type != DataType::NONE) {
    function_mock->delegateToFake();
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
    if (expectations->result_type != DataType::NONE) {
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
    function->cancelAsyncCall(future_result.call_id);
    EXPECT_THROW(future_result.get(), CallCanceled);
  } catch (const exception& ex) {
    if (expectations->result_type != DataType::NONE) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, cancelAsyncCallThrowsException) {
  EXPECT_CALL(*function_mock.get(), cancelAsyncCall(::testing::_))
      .Times(AtLeast(1));
  if (function->result_type != DataType::NONE) {
    EXPECT_THROW(function->cancelAsyncCall(202020202), CallerNotFound);
  } else {
    EXPECT_THROW(
        function->cancelAsyncCall(202020202), ResultReturningNotSupported);
  }
}

struct Executor {
  using ExecutorResult = std::pair<uintmax_t, std::future<DataVariant>>;

  Executor(const Executor&) = delete;

  Executor& operator()(const Executor&) = delete;

  Executor() = default;
  /**
   * @brief Create a new Executor callable with an execution delay
   *
   * @param response_delay - number of miliseconds to wait before responding
   */
  Executor(const uintmax_t& response_delay) : response_delay_(response_delay) {}

  ExecutorResult execute(const Function::Parameters& /*params*/) {
    auto execute_lock = std::lock_guard(execute_mx_);
    auto call_id = result_promises_.size();
    auto promise = std::promise<DataVariant>();
    auto result_future = std::make_pair(call_id, promise.get_future());
    result_promises_.emplace(call_id, std::move(promise));
    return result_future;
  }

  void cancel(uintmax_t call_id) {
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_exception(
          std::make_exception_ptr(CallCanceled(call_id, "ExternalExecutor")));
      auto clear_lock = std::lock_guard(erase_mx_);
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respond(uintmax_t call_id, const DataVariant& value = DataVariant()) {
    if (response_delay_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(response_delay_));
    }
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_value(value);
      auto clear_lock = std::lock_guard(erase_mx_);
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respond(uintmax_t call_id, const std::exception_ptr& exception) {
    if (response_delay_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(response_delay_));
    }
    auto iter = result_promises_.find(call_id);
    if (iter != result_promises_.end()) {
      iter->second.set_exception(exception);
      auto clear_lock = std::lock_guard(erase_mx_);
      iter = result_promises_.erase(iter);
    } else {
      throw CallerNotFound(call_id, "ExternalExecutor");
    }
  }

  void respondToAll(const DataVariant& value) {
    for (auto& result_promise : result_promises_) {
      result_promise.second.set_value(value);
    }
    auto clear_lock = std::lock_guard(erase_mx_);
    result_promises_.clear();
  }

  void respondToAll(const std::exception_ptr& exception) {
    for (auto& result_promise : result_promises_) {
      result_promise.second.set_exception(exception);
    }
    auto clear_lock = std::lock_guard(erase_mx_);
    result_promises_.clear();
  }

private:
  std::mutex execute_mx_;
  std::mutex erase_mx_;
  uintmax_t response_delay_ = 0;
  std::unordered_map<uintmax_t, std::promise<DataVariant>> result_promises_;
};

using ExecutorPtr = std::shared_ptr<Executor>;

class ExternalFunctionExecutorParametrizedTests
    : public ::testing::TestWithParam<FunctionExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<FunctionExpectations>(GetParam());
    function_mock = make_shared<MockFunction>(expectations->result_type,
        expectations->supported_params,
        expectations->result_value);
    function = function_mock;

    executor = make_shared<Executor>();
    MockFunction::Executor execute_cb =
        [this](const Function::Parameters& params) {
          return executor->execute(params);
        };
    MockFunction::Canceler cancel_cb = [this](uintmax_t call_id) {
      executor->cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);
  }

  FunctionExpectationsPtr expectations; // NOLINT(readability-identifier-naming)
  MockFunctionPtr function_mock; // NOLINT(readability-identifier-naming)
  FunctionPtr function; // NOLINT(readability-identifier-naming)
  ExecutorPtr executor; // NOLINT(readability-identifier-naming)
};

// NOLINTNEXTLINE
TEST_P(ExternalFunctionExecutorParametrizedTests, canCall) {
  if (expectations->result_type != DataType::NONE) {
    EXPECT_CALL(*function_mock.get(), call(::testing::_, ::testing::_))
        .Times(AtLeast(1));

    auto call_result_future = std::async(std::launch::async, [this]() {
      // NOLINTNEXTLINE(readability-magic-numbers)
      return function->call(1000);
    });
    this_thread::sleep_for(100ms);
    // if we do not give enough time for the thread to allocate the
    // response future, we will respond before a call was created,
    // thus hanging the test
    executor->respondToAll(expectations->result_value.value_or(DataVariant()));

    try {
      auto call_result = call_result_future.get();
      EXPECT_EQ(
          expectations->result_value.value_or(DataVariant()), call_result);
    } catch (const exception& ex) {
      FAIL() << "Caught an unexpected exception" << ex.what();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(ExternalFunctionExecutorParametrizedTests, callThrowsDomainError) {
  if (expectations->result_type != DataType::NONE) {
    EXPECT_CALL(*function_mock.get(), call(::testing::_, ::testing::_))
        .Times(AtLeast(1));

    auto call_exception_future =
        std::async(std::launch::async, [this]() { return function->call(); });
    this_thread::sleep_for(
        100ms); // if we do not give enough time for the thread to allocate the
                // response future, we will respond before a call was created,
                // thus hanging the test
    executor->respondToAll(
        std::make_exception_ptr(std::domain_error("Test exception throwing")));

    EXPECT_THROW(call_exception_future.get(), std::domain_error);
  }
}

// NOLINTNEXTLINE
TEST_P(ExternalFunctionExecutorParametrizedTests, canAsyncCall) {
  if (expectations->result_type != DataType::NONE) {
    EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_))
        .Times(AtLeast(1));

    auto async_call_result_future = function->asyncCall();
    executor->respondToAll(expectations->result_value.value_or(DataVariant()));

    try {
      auto async_call_result = async_call_result_future.get();
      EXPECT_EQ(expectations->result_value.value_or(DataVariant()),
          async_call_result);
    } catch (const exception& ex) {
      FAIL() << "Caught an unexpected exception" << ex.what();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(ExternalFunctionExecutorParametrizedTests, asyncCallThrowsDomainError) {
  if (expectations->result_type != DataType::NONE) {
    EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_))
        .Times(AtLeast(1));

    auto async_call_exception_future = function->asyncCall();
    executor->respondToAll(
        std::make_exception_ptr(std::domain_error("Test exception throwing")));

    EXPECT_THROW(async_call_exception_future.get(), std::domain_error);
  }
}

// NOLINTNEXTLINE
TEST_P(ExternalFunctionExecutorParametrizedTests, canCancelAsyncCall) {
  if (expectations->result_type != DataType::NONE) {
    EXPECT_CALL(*function_mock.get(), cancelAsyncCall(::testing::_))
        .Times(AtLeast(2));

    auto async_call_result_future = function->asyncCall();

    EXPECT_NO_THROW(
        function->cancelAsyncCall(async_call_result_future.call_id));

    EXPECT_THROW(function->cancelAsyncCall(202020202), CallerNotFound);
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, throwsLogicErrorOnExternalExecutorSet) {
  EXPECT_CALL(*function_mock.get(), asyncCall(::testing::_)).Times(AtLeast(1));
  try {
    auto future_result = function->asyncCall();
    auto executor = Executor();
    MockFunction::Executor execute_cb =
        [&executor](const Function::Parameters& params) {
          return executor.execute(params);
        };
    MockFunction::Canceler cancel_cb = [&executor](uintmax_t call_id) {
      executor.cancel(call_id);
    };
    function_mock->delegateToFake(execute_cb, cancel_cb);
    EXPECT_THROW(future_result.get(), std::logic_error);
  } catch (const exception& ex) {
    if (expectations->result_type != DataType::NONE) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, hasResultDataType) {
  EXPECT_EQ(expectations->result_type, function->result_type);
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, hasSupportedParameterTypes) {
  EXPECT_EQ(expectations->supported_params, function->parameters);
}

struct SetFunctionTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name;
  }
};

Function::ParameterTypes makeSupportedTypes(const vector<DataType>& types) {
  Function::ParameterTypes result;
  for (auto type : types) {
    result.emplace(result.size(), make_pair(type, false));
  }
  return result;
}

// We do no want to use toString(DataType), since that returns long strings with
// spaces, which will not work for test name generation
string makeExpectationName(DataType type) {
  switch (type) {
  case DataType::BOOLEAN:
    return "Bool";
  case DataType::INTEGER:
    return "Int";
  case DataType::UNSIGNED_INTEGER:
    return "UInt";
  case DataType::DOUBLE:
    return "Double";
  case DataType::TIME:
    return "Time";
  case DataType::OPAQUE:
    return "Opaque";
  case DataType::STRING:
    return "String";
  case DataType::NONE:
  default:
    return "Unknown";
  }
}

string sanitizeValueName(const DataVariant& value) {
  string result;
  match(
      value,
      [&result](bool val) { result = val ? "True" : "False"; },
      [&result](auto val) {
        if (val < 0) {
          result = "Neg";
          val = val * -1; // std::abs resolution fails due to auto
        }
        result += std::to_string(val);
        result = regex_replace(
            result, regex("\\."), "_"); // fix floating point string
      },
      [&result](DateTime /*val*/) {
        /*don't bother, the value is too complex*/
      },
      [&result](const vector<uint8_t>& val) {
        stringstream ss;
        ss << hex << setfill('0');
        for (auto byte : val) {
          ss << hex << setw(2) << static_cast<int>(byte);
        }
        result = ss.str();
      },
      [&result](const string& val) {
        result = regex_replace(val, regex("\\s"), "_");
      });
  return result;
}

void expandFunctionTestParameters(vector<FunctionExpectations>& expectations,
    const vector<DataType>& types,
    optional<DataVariant> return_value = nullopt) {
  string name = "accept";
  for (auto type : types) {
    name += makeExpectationName(type);
  }
  if (return_value.has_value()) {
    auto value = return_value.value();
    auto return_type = toDataType(value);
    name +=
        "Return" + makeExpectationName(return_type) + sanitizeValueName(value);
  }
  expectations.emplace_back(name, makeSupportedTypes(types));
}

vector<FunctionExpectations> makeFunctionTestParameters() {
  vector<FunctionExpectations> expectations;
  expectations.emplace_back("acceptAndReturnNothing");
  expectations.emplace_back("returnDefaultBoolForNoParams", DataType::BOOLEAN);
  expectations.emplace_back("returnDefaultIntForNoParams", DataType::INTEGER);
  expectations.emplace_back(
      "returnDefaultUintForNoParams", DataType::UNSIGNED_INTEGER);
  expectations.emplace_back("returnDefaultDoubleForNoParams", DataType::DOUBLE);
  expectations.emplace_back("returnDefaultStringForNoParams", DataType::STRING);
  expectations.emplace_back("returnDefaultOpaqueForNoParams", DataType::OPAQUE);
  expectations.emplace_back("returnDefaultTimeForNoParams", DataType::TIME);
  expandFunctionTestParameters(expectations, {DataType::BOOLEAN});
  expandFunctionTestParameters(expectations, {DataType::INTEGER});
  expandFunctionTestParameters(expectations, {DataType::UNSIGNED_INTEGER});
  expandFunctionTestParameters(expectations, {DataType::DOUBLE});
  expandFunctionTestParameters(expectations, {DataType::STRING});
  expandFunctionTestParameters(expectations, {DataType::OPAQUE});
  expandFunctionTestParameters(expectations, {DataType::TIME});
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME});
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      false);
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      (intmax_t)-11); // NOLINT(readability-magic-numbers)
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      (uintmax_t)21); // NOLINT(readability-magic-numbers)
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      3.14); // NOLINT(readability-magic-numbers)
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      "Hello World"); // NOLINT(readability-magic-numbers)
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      vector<uint8_t>{0x0, 0x1, 0x2, 0x3}); // NOLINT(readability-magic-numbers)
  expandFunctionTestParameters(expectations,
      {DataType::BOOLEAN,
          DataType::INTEGER,
          DataType::UNSIGNED_INTEGER,
          DataType::DOUBLE,
          DataType::STRING,
          DataType::OPAQUE,
          DataType::TIME},
      DateTime());
  return expectations;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(FunctionTests,
    FunctionParametrizedTests,
    ::testing::ValuesIn(makeFunctionTestParameters()),
    SetFunctionTestNameSuffix());

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(FunctionTests,
    ExternalFunctionExecutorParametrizedTests,
    ::testing::ValuesIn(makeFunctionTestParameters()),
    SetFunctionTestNameSuffix());