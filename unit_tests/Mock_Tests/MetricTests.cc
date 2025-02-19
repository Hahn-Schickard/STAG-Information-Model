#include "Metric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct MetricExpectations {
  const string name;
  const DataType type;
  const DataVariant value;

  MetricExpectations(const MetricExpectations& other) = default;

  MetricExpectations(
      const string& expectation_name, // NOLINT(modernize-pass-by-value)
      DataType expectation_type,
      const DataVariant& expectation_value) // NOLINT(modernize-pass-by-value)
      : name(expectation_name), type(expectation_type),
        value(expectation_value) {}
};

using MetricExpectationsPtr = shared_ptr<MetricExpectations>;

class MetricMultipleParametersTests
    : public ::testing::TestWithParam<MetricExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<MetricExpectations>(GetParam());
    metric = make_shared<MockMetric>(expectations->type, expectations->value);
  }

  MetricExpectationsPtr expectations; // NOLINT(readability-identifier-naming)
  MetricPtr metric; // NOLINT(readability-identifier-naming)
};

// NOLINTNEXTLINE
TEST_P(MetricMultipleParametersTests, canGetType) {
  EXPECT_EQ(expectations->type, metric->getDataType());
}

// NOLINTNEXTLINE
TEST_P(MetricMultipleParametersTests, canGetValue) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), getMetricValue()).Times(1);

  DataVariant tested;
  ASSERT_NO_THROW(tested = metric->getMetricValue(););
  EXPECT_EQ(expectations->value, tested);
}

struct SetMetricTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name;
  }
};

vector<MetricExpectations> makeMetricTestParameters() {
  vector<MetricExpectations> params;
  // clang-format off
  params.emplace_back(
    "Bool", 
    DataType::BOOLEAN, 
    DataVariant(true)
  );
  params.emplace_back(
    "Int",
    DataType::INTEGER,
    DataVariant((intmax_t)26) // NOLINT(readability-magic-numbers)
  );
  params.emplace_back(
    "UInt", 
    DataType::UNSIGNED_INTEGER, 
    DataVariant((uintmax_t)74) // NOLINT(readability-magic-numbers)
  );
  params.emplace_back(
    "Double", 
    DataType::DOUBLE, 
    DataVariant(20.2) // NOLINT(readability-magic-numbers)
  );
  params.emplace_back(
    "Opaque", 
    DataType::OPAQUE, // NOLINTNEXTLINE(readability-magic-numbers)  
    DataVariant(vector<uint8_t>{0, 1, 2, 3}) 
  );
  params.emplace_back(
    "String", 
    DataType::STRING, 
    DataVariant(string("Hello world!"))
  );
  params.emplace_back(
    "Time", 
    DataType::TIME, 
    DataVariant(DateTime())
  );
  // clang-format on
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(MetricTests,
    MetricMultipleParametersTests,
    ::testing::ValuesIn(makeMetricTestParameters()),
    SetMetricTestNameSuffix());
