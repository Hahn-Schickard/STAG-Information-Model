#include "Function_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

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
    function = make_shared<MockFunction>(expectations->result_type_,
        expectations->supported_params_,
        expectations->result_value_);
  }

  FunctionExpectationsPtr expectations;
  FunctionPtr function;
};

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canExecute) {
  EXPECT_CALL(*function.get(), execute(::testing::_)).Times(AtLeast(1));
  ASSERT_NO_THROW(function->execute());
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canCall) {
  EXPECT_CALL(*function.get(), call(::testing::_)).Times(AtLeast(1));
  ASSERT_NO_THROW(function->call());
}

// NOLINTNEXTLINE
TEST_P(FunctionParametrizedTests, canAsyncCall) {
  EXPECT_CALL(*function.get(), asyncCall(::testing::_)).Times(AtLeast(1));
  ASSERT_NO_THROW(function->asyncCall());
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
  //   params.emplace_back("Bool", DataType::BOOLEAN, DataVariant((bool)true));
  //   params.emplace_back("Int", DataType::INTEGER, DataVariant((intmax_t)26));
  //   params.emplace_back(
  //       "UInt", DataType::UNSIGNED_INTEGER, DataVariant((uintmax_t)74));
  //   params.emplace_back("Double", DataType::DOUBLE,
  //   DataVariant((double)20.2)); params.emplace_back(
  //       "Opaque", DataType::OPAQUE, DataVariant(vector<uint8_t>{0, 1, 2,
  //       3}));
  //   params.emplace_back(
  //       "String", DataType::STRING, DataVariant(string("Hello world!")));
  //   params.emplace_back("Time", DataType::TIME, DataVariant(DateTime()));
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(FunctionTests,
    FunctionParametrizedTests,
    ::testing::ValuesIn(makeFunctionTestParameters()),
    SetMetricTestNameSuffix());
