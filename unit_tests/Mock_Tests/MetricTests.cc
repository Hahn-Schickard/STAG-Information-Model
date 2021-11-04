#include "Metric_MOCK.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct Expectations {
  const string ref_id_;
  const string name_;
  const string desc_;
  const DataType type_;
  const DataVariant value_;

  Expectations(const Expectations &other)
      : ref_id_(other.ref_id_), name_(other.name_), desc_(other.desc_),
        type_(other.type_), value_(other.value_) {}

  Expectations(const string &ref_id, const string &name, const string &desc,
               DataType type, DataVariant value)
      : ref_id_(ref_id), name_(name), desc_(desc), type_(type), value_(value) {}
};

using ExpectationsPtr = shared_ptr<Expectations>;

class MetricMultipleParametersTests
    : public ::testing::TestWithParam<Expectations> {
protected:
  void SetUp() override {
    expectations = make_shared<Expectations>(GetParam());
    metric = make_shared<MockMetric>(expectations->ref_id_, expectations->name_,
                                     expectations->desc_, expectations->type_,
                                     expectations->value_);
  }

  ExpectationsPtr expectations;
  MetricPtr metric;
};

TEST_P(MetricMultipleParametersTests, hasCorrectID) {
  string testedElement = metric->getElementId();
  string expectedResult = expectations->ref_id_;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, hasCorrectName) {
  string testedElement = metric->getElementName();
  string expectedResult = expectations->name_;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, hasCorrectDescription) {
  string testedElement = metric->getElementDescription();
  string expectedResult = expectations->desc_;

  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, canGetSize) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), size()).Times(1);

  size_t tested;
  size_t expected = size_of(expectations->value_);
  ASSERT_NO_THROW(tested = metric->size());
  EXPECT_EQ(expected, tested);
}

TEST_P(MetricMultipleParametersTests, canGetType) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), getDataType()).Times(1);

  DataType tested;
  ASSERT_NO_THROW(tested = metric->getDataType());
  EXPECT_EQ(expectations->type_, tested);
}

TEST_P(MetricMultipleParametersTests, canGetValue) {
  auto mock = dynamic_pointer_cast<MockMetric>(metric);
  mock->delegateToFake();

  EXPECT_CALL(*mock.get(), getMetricValue()).Times(1);

  DataVariant tested;
  ASSERT_NO_THROW(tested = metric->getMetricValue(););
  EXPECT_EQ(expectations->value_, tested);
}

struct SetTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    return info.param.name_;
  }
};

using TestParameters = std::vector<Expectations>;

TestParameters makeTestParameters() {
  TestParameters params;
  params.emplace_back("0", "Bool", "Boolean metric", DataType::BOOLEAN,
                      DataVariant((bool)true));
  params.emplace_back("1", "Int", "Integer metric", DataType::INTEGER,
                      DataVariant((intmax_t)26));
  params.emplace_back("2", "UInt", "Unsigned integer metric",
                      DataType::UNSIGNED_INTEGER, DataVariant((uintmax_t)74));
  params.emplace_back("3", "Double", "Double metric", DataType::DOUBLE,
                      DataVariant((double)20.2));
  params.emplace_back("4", "Opaque", "Opaque metric", DataType::OPAQUE,
                      DataVariant(vector<uint8_t>{0, 1, 2, 3}));
  params.emplace_back("5", "String", "String metric", DataType::STRING,
                      DataVariant(string("Hello world!")));
  params.emplace_back("6", "Time", "Time metric", DataType::TIME,
                      DataVariant(DateTime()));
  return move(params);
}

INSTANTIATE_TEST_SUITE_P(MetricTests, MetricMultipleParametersTests,
                         ::testing::ValuesIn(makeTestParameters()),
                         SetTestNameSuffix());