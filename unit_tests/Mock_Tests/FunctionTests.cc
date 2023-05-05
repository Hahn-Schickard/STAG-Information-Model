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
  } catch (exception& ex) {
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
    function->cancelAsyncCall(future_result.first);
    EXPECT_THROW(future_result.second.get(), CallCanceled);
  } catch (exception& ex) {
    if (expectations->result_type_ != DataType::UNKNOWN) {
      FAIL() << "Caught an unexpected exception: " << ex.what();
    } else {
      SUCCEED();
    }
  }
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
