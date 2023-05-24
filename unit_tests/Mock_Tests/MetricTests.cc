#include "Metric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct MetricExpectations {
  const string name_;
  const DataType type_;
  const DataVariant value_;

  MetricExpectations(const MetricExpectations& other) = default;

  MetricExpectations(const string& name, DataType type, DataVariant value)
      : name_(name), type_(type), value_(value) {}
};

using MetricExpectationsPtr = shared_ptr<MetricExpectations>;

class MetricMultipleParametersTests
    : public ::testing::TestWithParam<MetricExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<MetricExpectations>(GetParam());
    metric = make_shared<MockMetric>(expectations->type_, expectations->value_);
  }

  MetricExpectationsPtr expectations;
  MetricPtr metric;
};

// NOLINTNEXTLINE
TEST_P(MetricMultipleParametersTests, canGetType) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), getDataType()).Times(1);

  DataType tested;
  ASSERT_NO_THROW(tested = metric->getDataType());
  EXPECT_EQ(expectations->type_, tested);
}

// NOLINTNEXTLINE
TEST_P(MetricMultipleParametersTests, canGetValue) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), getMetricValue()).Times(1);

  DataVariant tested;
  ASSERT_NO_THROW(tested = metric->getMetricValue(););
  EXPECT_EQ(expectations->value_, tested);
}

struct SetMetricTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name_;
  }
};

vector<MetricExpectations> makeMetricTestParameters() {
  vector<MetricExpectations> params;
  params.emplace_back("Bool", DataType::BOOLEAN, DataVariant((bool)true));
  params.emplace_back("Int", DataType::INTEGER, DataVariant((intmax_t)26));
  params.emplace_back(
      "UInt", DataType::UNSIGNED_INTEGER, DataVariant((uintmax_t)74));
  params.emplace_back("Double", DataType::DOUBLE, DataVariant((double)20.2));
  params.emplace_back(
      "Opaque", DataType::OPAQUE, DataVariant(vector<uint8_t>{0, 1, 2, 3}));
  params.emplace_back(
      "String", DataType::STRING, DataVariant(string("Hello world!")));
  params.emplace_back("Time", DataType::TIME, DataVariant(DateTime()));
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(MetricTests,
    MetricMultipleParametersTests,
    ::testing::ValuesIn(makeMetricTestParameters()),
    SetMetricTestNameSuffix());
