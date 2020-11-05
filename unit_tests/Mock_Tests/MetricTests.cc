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
               DataType type, const DataVariant &value,
               shared_ptr<MockMetric> mock)
      : ref_id_(ref_id), name_(name), desc_(desc), type_(type), value_(value),
        mock_(mock) {
    ON_CALL(*mock_.get(), getMetricValue())
        .WillByDefault(::testing::Return(value_));
    ON_CALL(*mock_.get(), getDataType())
        .WillByDefault(::testing::Return(type_));
  }

private:
  shared_ptr<MockMetric> mock_;
};

struct MetricTestParam {
  shared_ptr<Metric> metric;
  Expectations expectations;

  MetricTestParam(shared_ptr<Metric> metric_arg,
                  const Expectations &expectations_arg)
      : metric(metric_arg), expectations(expectations_arg) {}
};

MetricTestParam buildTestParameter(const string &ref_id, const string &name,
                                   const string &desc, DataType type,
                                   const DataVariant &value) {
  auto metric = make_shared<MockMetric>(ref_id, name, desc);
  Expectations expectations(ref_id, name, desc, type, value, metric);

  return MetricTestParam(metric, expectations);
}

class MetricMultipleParametersTests
    : public ::testing::TestWithParam<MetricTestParam> {};

TEST_P(MetricMultipleParametersTests, hasCorrectID) {
  string testedElement = GetParam().metric->getElementId();
  string expectedResult = GetParam().expectations.ref_id_;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, hasCorrectName) {
  string testedElement = GetParam().metric->getElementName();
  string expectedResult = GetParam().expectations.name_;
  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, hasCorrectDescription) {
  string testedElement = GetParam().metric->getElementDescription();
  string expectedResult = GetParam().expectations.desc_;

  EXPECT_EQ(expectedResult, testedElement)
      << "expected: " << expectedResult << endl
      << "provided: " << testedElement << endl;
}

TEST_P(MetricMultipleParametersTests, canGetType) {
  auto metric = GetParam().metric;
  auto expectations = GetParam().expectations;

  DataType tested;
  ASSERT_NO_THROW(tested = metric->getDataType());
  EXPECT_EQ(expectations.type_, tested);
}

TEST_P(MetricMultipleParametersTests, canGetValue) {
  auto metric = GetParam().metric;
  auto expectations = GetParam().expectations;

  DataVariant tested;
  ASSERT_NO_THROW(tested = metric->getMetricValue());
  DataVariant expected = expectations.value_;
  EXPECT_EQ(expected, tested);
}

struct SetTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    return info.param.expectations.name_;
  }
};

INSTANTIATE_TEST_CASE_P(
    MetricTests, MetricMultipleParametersTests,
    ::testing::Values(
        buildTestParameter("0", "Bool", "Boolean metric", DataType::BOOLEAN,
                           DataVariant((bool)true)),
        buildTestParameter("1", "Int", "Integer metric", DataType::INTEGER,
                           DataVariant((int64_t)26)),
        buildTestParameter("2", "UInt", "Unsigned integer metric",
                           DataType::UNSIGNED_INTEGER,
                           DataVariant((uint64_t)74)),
        buildTestParameter("3", "Double", "Double metric", DataType::DOUBLE,
                           DataVariant((double)20.2)),
        buildTestParameter("4", "Opaque", "Opaque metric", DataType::OPAQUE,
                           DataVariant(vector<uint8_t>{0, 1, 2, 3})),
        buildTestParameter("5", "String", "String metric", DataType::STRING,
                           DataVariant(string("Hello world!"))),
        buildTestParameter("6", "Time", "Time metric", DataType::TIME,
                           DataVariant(DateTime()))),
    SetTestNameSuffix());