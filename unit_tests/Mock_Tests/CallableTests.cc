#include "CallableMock.hpp"

#include <gtest/gtest.h>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct CallableTestParam {
  string test_name = "0";
  DataType result_type = DataType::None;
  Callable::ParameterTypes supported_params;
  Executor::Response default_response;
  Callable::Parameters parameters;
};

struct CallableTests : public TestWithParam<CallableTestParam> {
  CallableTests() : expected(GetParam()) {
    tested = make_shared<CallableMock>(expected.result_type,
        expected.supported_params,
        expected.default_response);
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

TEST_P(CallableTests, canExecute) {
  EXPECT_CALL(*tested, execute).Times(Exactly(1));

  EXPECT_NO_THROW(tested->execute(expected.parameters));
}

TEST_P(CallableTests, canCall) {
  EXPECT_CALL(*tested, call(expected.parameters, 100)).Times(Exactly(1));

  if (expected.result_type != DataType::None) {
    auto executor = tested->getExecutor();
    executor->start();
    EXPECT_NO_THROW(tested->call(expected.parameters, 100));
    executor->stop();
  } else {
    EXPECT_THROW(
        tested->call(expected.parameters, 100), ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canTimeoutCall) {
  EXPECT_CALL(*tested, call(expected.parameters, 1)).Times(Exactly(1));

  if (expected.result_type != DataType::None) {
    EXPECT_THROW(tested->call(expected.parameters, 1), CallTimedout);
  } else {
    EXPECT_THROW(
        tested->call(expected.parameters, 1), ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canAsyncCall) {
  EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(Exactly(1));

  if (expected.result_type != DataType::None) {
    EXPECT_NO_THROW({
      auto result = tested->asyncCall(expected.parameters);

      auto executor = tested->getExecutor();
      executor->start();
      EXPECT_EQ(result.get(), get<DataVariant>(expected.default_response));
      executor->stop();
    });
  } else {
    EXPECT_THROW(
        { auto result = tested->asyncCall(expected.parameters); },
        ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canCancelAsyncCall) {
  if (expected.result_type != DataType::None) {
    EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(Exactly(2));
    EXPECT_CALL(*tested, cancelAsyncCall(_)).Times(Exactly(1));

    EXPECT_NO_THROW({
      auto result = tested->asyncCall(expected.parameters);
      result.cancel();

      auto another_result = tested->asyncCall(expected.parameters);
      tested->cancelAsyncCall(another_result.callerID());
      EXPECT_THROW(another_result.get(), CallCanceled);
    });
  } else {
    EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(Exactly(1));

    EXPECT_THROW(
        { auto result = tested->asyncCall(expected.parameters); },
        ResultReturningNotSupported);
  }
}

TEST_P(CallableTests, canCancelAllAsyncCalls) {
  if (expected.result_type != DataType::None) {
    EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times(Exactly(3));
    EXPECT_CALL(*tested, cancelAllAsyncCalls()).Times(Exactly(1));

    EXPECT_NO_THROW({
      auto result_1 = tested->asyncCall(expected.parameters);
      auto result_2 = tested->asyncCall(expected.parameters);
      auto result_3 = tested->asyncCall(expected.parameters);

      auto executor = tested->getExecutor();
      executor->start();

      tested->cancelAllAsyncCalls();
      EXPECT_THROW(result_1.get(), CallCanceled);
      EXPECT_THROW(result_2.get(), CallCanceled);
      EXPECT_THROW(result_3.get(), CallCanceled);
      executor->stop();
    });
  } else {
    EXPECT_CALL(*tested, asyncCall(expected.parameters)).Times((1));

    EXPECT_THROW(
        { auto result = tested->asyncCall(expected.parameters); },
        ResultReturningNotSupported);
  }
}

// TEST_P(CallableTests, executorThrowsCallerNotFound) {
//   auto executor = tested->getExecutor();

//   auto respond_with_value = [&]() { executor->respond(0, true); };
//   EXPECT_THAT(
//       move(respond_with_value), //@todo: check if we can define lambda in
//       place ThrowsMessage<CallerNotFound>(HasSubstr(
//           "No caller with id: 0 for Callable ExternalExecutor call
//           exists")));

//   auto respond_with_exception = [&]() {
//     executor->respond(1, make_exception_ptr(runtime_error("Not thrown")));
//   };
//   EXPECT_THAT(move(respond_with_exception),
//       ThrowsMessage<CallerNotFound>(HasSubstr(
//           "No caller with id: 1 for Callable ExternalExecutor call
//           exists")));

//   auto cancel_call = [&]() { executor->cancel(2); };
//   EXPECT_THAT(move(cancel_call),
//       ThrowsMessage<CallerNotFound>(HasSubstr(
//           "No caller with id: 2 for Callable ExternalExecutor call
//           exists")));
// }

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