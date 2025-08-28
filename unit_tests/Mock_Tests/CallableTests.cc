#include "CallableMock.hpp"

#include <gtest/gtest.h>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct CallableTestParam {
  string test_name = "0";
  DataType result_type = DataType::None;
  optional<DataVariant> result_value;
  Callable::ParameterTypes supported_params;
  Callable::Parameters parameters;
};

struct CallableTests : public TestWithParam<CallableTestParam> {
  CallableTests() : expected(GetParam()) {
    tested = make_shared<CallableMock>(
        expected.result_type, expected.supported_params);
  }

  CallableTestParam expected;
  CallableMockPtr tested;
};

TEST_P(CallableTests, returnsResultType) {
  EXPECT_CALL(*tested, resultType).Times(Exactly(1));

  EXPECT_EQ(tested->resultType(), expected.result_type);
}

TEST_P(CallableTests, returnsParameterTypes) {
  EXPECT_CALL(*tested, parameterTypes).Times(Exactly(1));

  EXPECT_THAT(tested->parameterTypes(), ContainerEq(expected.supported_params));
}

TEST_P(CallableTests, canExecuteDefaultCallback) {
  EXPECT_CALL(*tested, execute).Times(Exactly(1));

  EXPECT_NO_THROW(tested->execute(expected.parameters));
}

TEST_P(CallableTests, canCallDefaultCallback) {
  EXPECT_CALL(*tested, call(expected.parameters, 100)).Times(Exactly(1));

  if (expected.result_type != DataType::None) {
    auto executor = tested->getExecutor();
    executor->autoRespond(expected.result_value);
    EXPECT_NO_THROW(tested->call(expected.parameters, 100));
  } else {
    EXPECT_THROW(
        tested->call(expected.parameters, 100), ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canAsyncCallDefaultCallback) {
  EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(Exactly(1));

  if (expected.result_type != DataType::None) {
    auto executor = tested->getExecutor();
    executor->autoRespond(expected.result_value);
    EXPECT_NO_THROW({
      auto result = tested->asyncCall(expected.parameters);
      EXPECT_EQ(result.get(), expected.result_value.value());
    });
  } else {
    EXPECT_THROW(
        { auto result = tested->asyncCall(expected.parameters); },
        ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canCancelAsyncCallDefaultCallback) {
  EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(AtLeast(1));

  if (expected.result_type != DataType::None) {
    auto executor = tested->getExecutor();
    executor->autoRespond(expected.result_value);
    EXPECT_NO_THROW({
      auto result = tested->asyncCall(expected.parameters);
      result.cancel();

      auto another_result = tested->asyncCall(expected.parameters);
      tested->cancelAsyncCall(another_result.callerID());
      EXPECT_THROW(another_result.get(), CallCanceled);
    });
  } else {
    EXPECT_THROW(
        { auto result = tested->asyncCall(expected.parameters); },
        ResultReturningNotSupported);
  }
}

// NOLINTBEGIN(readability-magic-numbers)
INSTANTIATE_TEST_SUITE_P(CallableTestsValues,
    CallableTests,
    Values( // clang-format off
        CallableTestParam{}
    ), // clang-format on
    [](const TestParamInfo<CallableTests::ParamType>& info) {
      return info.param.test_name;
    });
// NOLINTEND(readability-magic-numbers)
} // namespace Information_Model::testing