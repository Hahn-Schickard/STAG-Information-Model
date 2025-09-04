#include "Callable.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct CallableParamTests : public testing::Test {
  CallableParamTests() {
    supported_params = Callable::ParameterTypes{
        {0, Callable::ParameterType{DataType::Boolean, false}},
        {1, Callable::ParameterType{DataType::Unsigned_Integer, true}},
        {2, Callable::ParameterType{DataType::String, false}}};
  }

  Callable::ParameterTypes supported_params;
};

TEST_F(CallableParamTests, throwsMandatoryParameterHasNoValue) {
  Callable::Parameters tested{{1, nullopt}};
  string exception_msg =
      "Mandatory parameter 1:" + toString(DataType::Unsigned_Integer) +
      " has no value set";

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<MandatoryParameterHasNoValue>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsMandatoryParameterMissing) {
  Callable::Parameters tested{{0, true}};
  string exception_msg =
      "Mandatory parameter 1:" + toString(DataType::Unsigned_Integer) +
      " is missing";

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<MandatoryParameterMissing>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsParameterTypeMismatch) {
  Callable::Parameters tested{{1, "hello world"}};
  string exception_msg = "Parameter 1:" + toString(DataType::Unsigned_Integer) +
      " does not accept " + toString(DataType::String);

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<ParameterTypeMismatch>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsParameterDoesNotExist) {
  Callable::Parameters tested{};
  string exception_msg = "No parameter exists at position 5";

  EXPECT_THAT(
      [&]() { addSupportedParameter(tested, supported_params, 5, 20.2); },
      ThrowsMessage<ParameterDoesNotExist>(HasSubstr(exception_msg)));
}
} // namespace Information_Model::testing