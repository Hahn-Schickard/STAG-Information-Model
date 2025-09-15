#include "Callable.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <optional>
#include <string>
#include <vector>

namespace Information_Model::testing {
using namespace std;
using namespace ::testing;

struct CallableParamTests : public testing::Test {
  CallableParamTests() {
    supported_params =
        ParameterTypes{{0, ParameterType{DataType::Boolean, false}},
            {1, ParameterType{DataType::Unsigned_Integer, true}},
            {2, ParameterType{DataType::String, false}}};
  }

  ParameterTypes supported_params;
};

TEST_F(CallableParamTests, throwsMandatoryParameterHasNoValue) {
  Parameters tested{{1, nullopt}};
  string exception_msg =
      "Mandatory parameter 1:" + toString(DataType::Unsigned_Integer) +
      " has no value set";

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<MandatoryParameterHasNoValue>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsMandatoryParameterMissing) {
  Parameters tested{{0, true}};
  string exception_msg =
      "Mandatory parameter 1:" + toString(DataType::Unsigned_Integer) +
      " is missing";

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<MandatoryParameterMissing>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsParameterTypeMismatch) {
  Parameters tested{{1, string("hello world")}};
  string exception_msg = "Parameter 1:" + toString(DataType::Unsigned_Integer) +
      " does not accept " + toString(DataType::String);

  EXPECT_THAT([&]() { checkParameters(tested, supported_params); },
      ThrowsMessage<ParameterTypeMismatch>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, throwsParameterDoesNotExist) {
  Parameters tested{};
  string exception_msg = "No parameter exists at position 5";

  EXPECT_THAT( // NOLINTNEXTLINE(readability-magic-numbers)
      [&]() { addSupportedParameter(tested, supported_params, 5, 20.2); },
      ThrowsMessage<ParameterDoesNotExist>(HasSubstr(exception_msg)));
}

TEST_F(CallableParamTests, canAddSupportedParameter) {
  Parameters tested{};
  Parameters expected{{1, setVariant(DataType::Unsigned_Integer)}};

  addSupportedParameter(tested, supported_params, 2, string("Hello"));
  addSupportedParameter(tested, supported_params, 2, string("Goodbye"), true);
  addSupportedParameter(tested, supported_params, 2, nullopt);

  EXPECT_EQ(tested.size(), 1);
  EXPECT_EQ(get<string>(tested.at(2).value()), "Goodbye");
}

TEST_F(CallableParamTests, canMakeDefaultParams) {
  Parameters expected{{1, setVariant(DataType::Unsigned_Integer)}};

  auto tested = makeDefaultParams(supported_params);
  EXPECT_THAT(tested, ContainerEq(expected));
}

TEST_F(CallableParamTests, canPrintDefaultParams) {
  {
    string expected = "{}";

    auto tested = toString(ParameterTypes{});
    EXPECT_EQ(tested, expected);
  }
  {
    string expected = "{{0,Boolean,optional},{1,UnsignedInteger,"
                      "mandatory},{2,String,optional}}";

    auto tested = toString(supported_params);
    EXPECT_EQ(tested, expected);
  }
}

TEST_F(CallableParamTests, canPrintParams) {
  {
    string expected = "{}";

    auto tested = toString(Parameters{});
    EXPECT_EQ(tested, expected);
  }
  {
    string expected =
        "{{0,True},{1,32},{2,Neg32},{3,32P200000},{4,"
        "2025Y09M11D17H25M30P000031Z},{5,0x000102},{6,NullOpaque},{"
        "7,hello_world},{8,NullString},{9,NullOpt}}";

    // NOLINTBEGIN(readability-magic-numbers)
    auto params = Parameters{// clang-format off
        {0, DataVariant(true)},
        {1, DataVariant((uintmax_t)32)},
        {2, DataVariant((intmax_t)-32)},
        {3, DataVariant(32.2)},
        {4, DataVariant(Timestamp{2025, 9, 11, 17, 25, 30, 31})},
        {5, DataVariant(vector<uint8_t>{0x00,0x01,0x02})},
        {6, DataVariant(vector<uint8_t>{})},
        {7, DataVariant(string("hello world"))},
        {8, DataVariant(string())},
        {9, optional<DataVariant>()}
    }; // clang-format on
    // NOLINTEND(readability-magic-numbers)
    EXPECT_EQ(toString(params), expected);
  }
}
} // namespace Information_Model::testing