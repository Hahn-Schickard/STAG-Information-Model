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

  Expectations(const string &ref_id, const string &name, const string &desc,
               DataType type, const DataVariant &value)
      : ref_id_(ref_id), name_(name), desc_(desc), type_(type), value_(value) {}
};

struct MetricTestParam {
  string test_name;
  shared_ptr<Metric> metric;
  shared_ptr<Expectations> expectations;

  MetricTestParam(const string &test_case_name, shared_ptr<Metric> metric_arg,
                  shared_ptr<Expectations> expectations_arg)
      : test_name(string(test_case_name)), metric(metric_arg),
        expectations(expectations_arg) {}

  ~MetricTestParam() {
    metric.reset();
    expectations.reset();
  }
};

MetricTestParam buildTestParameter(const string &ref_id, const string &name,
                                   const string &desc, DataType type,
                                   const DataVariant &value) {
  auto expectations =
      make_shared<Expectations>(ref_id, name, desc, type, value);
  auto mock = make_shared<MockMetric>(ref_id, name, desc, type, value);
  return MetricTestParam(name, move(mock), move(expectations));
}

class MetricMultipleParametersTests
    : public ::testing::TestWithParam<MetricTestParam> {
protected:
  void SetUp() override {
    metric = move(GetParam().metric);
    expectations = move(GetParam().expectations);
  }

  void TearDown() override {
    metric.reset();
    expectations.reset();
  }

  shared_ptr<Metric> metric;
  shared_ptr<Expectations> expectations;
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

TEST_P(MetricMultipleParametersTests, canGetType) {
  DataType tested;
  ASSERT_NO_THROW(tested = metric->getDataType());
  // EXPECT_EQ(expectations->type_, tested);
  EXPECT_TRUE(::testing::Mock::VerifyAndClear(metric.get()));
}

TEST_P(MetricMultipleParametersTests, canGetValue) {
  // TODO: check if ON_CALL and EXPECT_CALL is a GTest bug for mock leaking
  // auto mock = static_pointer_cast<MockMetric>(metric);
  // mock->delegateToFake();
  // EXPECT_CALL(*mock.get(), getMetricValue()).Times(1);
  DataVariant tested;
  ASSERT_NO_THROW(tested = metric->getMetricValue());
  // EXPECT_EQ(expectations->value_, tested);
  // EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(metric.get()));
}

struct SetTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    return info.param.test_name;
  }
};

INSTANTIATE_TEST_CASE_P(
    MetricTests, MetricMultipleParametersTests,
    ::testing::Values(
        buildTestParameter("0", "Bool", "Boolean metric", DataType::BOOLEAN,
                           DataVariant((bool)true)),
        buildTestParameter("1", "Int", "Integer metric", DataType::INTEGER,
                           DataVariant((intmax_t)26)),
        buildTestParameter("2", "UInt", "Unsigned integer metric",
                           DataType::UNSIGNED_INTEGER,
                           DataVariant((uintmax_t)74)),
        buildTestParameter("3", "Double", "Double metric", DataType::DOUBLE,
                           DataVariant((double)20.2)),
        buildTestParameter("4", "Opaque", "Opaque metric", DataType::OPAQUE,
                           DataVariant(vector<uint8_t>{0, 1, 2, 3})),
        buildTestParameter("5", "String", "String metric", DataType::STRING,
                           DataVariant(string("Hello world!"))),
        buildTestParameter("6", "Time", "Time metric", DataType::TIME,
                           DataVariant(DateTime()))),
    SetTestNameSuffix());