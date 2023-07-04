#include "DeviceElement.hpp"
#include "DeviceMockBuilder.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;
using ::testing::AtLeast;

struct DeviceElementExpectations {
  const string name;
  const string desc;
  const ElementType type;
  const DataType data_type;

  DeviceElementExpectations(const DeviceElementExpectations& other) = default;

  DeviceElementExpectations( // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
      const string& element_name, // NOLINT(modernize-pass-by-value)
      const string& element_desc, // NOLINT(modernize-pass-by-value)
      ElementType element_type,
      DataType element_data_type = DataType::UNKNOWN)
      : name(element_name), desc(element_desc), type(element_type),
        data_type(element_data_type) {}
};

using DeviceElementExpectationsPtr = shared_ptr<DeviceElementExpectations>;

DeviceBuilderInterface::Reader makeDefaultReader(DataType type) {
  return [type]() -> DataVariant { return setVariant(type); };
}

DeviceBuilderInterface::Writer makeDefaultWriter(DataType type) {
  return [type](const DataVariant& value) {
    if (!matchVariantType(value, type)) {
      string error_msg = "Incorrect data variant. Metric expects " +
          toString(type) + " data type, but " + toString(toDataType(value)) +
          " was provided";
      throw logic_error(error_msg);
    }
  };
}

class DeviceElementParametersTests
    : public ::testing::TestWithParam<DeviceElementExpectations> {
protected:
  void SetUp() override {
    expectations = make_shared<DeviceElementExpectations>(GetParam());

    auto builder = make_shared<DeviceMockBuilder>();
    builder->buildDeviceBase("1234", "Mocky", "Mocked device");

    switch (expectations->type) {
    case ElementType::GROUP: {
      builder->addDeviceElementGroup(expectations->name, expectations->desc);
      break;
    }
    case ElementType::READABLE: {
      builder->addReadableMetric(expectations->name,
          expectations->desc,
          expectations->data_type,
          makeDefaultReader(expectations->data_type));
      break;
    }
    case ElementType::WRITABLE: {
      builder->addWritableMetric(expectations->name,
          expectations->desc,
          expectations->data_type,
          makeDefaultWriter(expectations->data_type),
          makeDefaultReader(expectations->data_type));
      break;
    }
    default: {
      string error_msg =
          toString(expectations->type) + " testing is not supported";
      throw logic_error(error_msg);
    }
    }

    device = builder->getResult();
    element = device->getDeviceElement("1234:0").base();
  }

  // NOLINTNEXTLINE(readability-identifier-naming)
  DeviceElementExpectationsPtr expectations;
  DeviceElementPtr element; // NOLINT(readability-identifier-naming)
  DevicePtr device; // NOLINT(readability-identifier-naming)
};

// NOLINTNEXTLINE
TEST_P(DeviceElementParametersTests, canGetElementType) {
  try {
    auto tested = expectations->type;
    EXPECT_EQ(tested, element->getElementType());
  } catch (...) {
    FAIL() << " caught an exception";
  }
}

struct SetDeviceElementTestNameSuffix {
  template <class ParamType>
  string operator()(const ::testing::TestParamInfo<ParamType>& info) const {
    return info.param.name;
  }
};

vector<DeviceElementExpectations> makeDeviceElementTestParameters() {
  vector<DeviceElementExpectations> params;
  params.emplace_back("ReadableBoolean",
      "Read Only Boolean Metric",
      ElementType::READABLE,
      DataType::BOOLEAN);
  params.emplace_back("ReadableInteger",
      "Read Only Integer Metric",
      ElementType::READABLE,
      DataType::INTEGER);
  params.emplace_back("ReadableUnsignedInteger",
      "Read Only Unsigned Integer Metric",
      ElementType::READABLE,
      DataType::UNSIGNED_INTEGER);
  params.emplace_back("ReadableDouble",
      "Read Only Double Metric",
      ElementType::READABLE,
      DataType::DOUBLE);
  params.emplace_back("ReadableTime",
      "Read Only Time Metric",
      ElementType::READABLE,
      DataType::TIME);
  params.emplace_back("ReadableOpaque",
      "Read Only Opaque Metric",
      ElementType::READABLE,
      DataType::OPAQUE);
  params.emplace_back("ReadableString",
      "Read Only String Metric",
      ElementType::READABLE,
      DataType::STRING);
  params.emplace_back("WritableBoolean",
      "Read and Writable Boolean Metric",
      ElementType::WRITABLE,
      DataType::BOOLEAN);
  params.emplace_back("WritableInteger",
      "Read and Writable Integer Metric",
      ElementType::WRITABLE,
      DataType::INTEGER);
  params.emplace_back("WritableUnsignedInteger",
      "Read and Writable Unsigned Integer Metric",
      ElementType::WRITABLE,
      DataType::UNSIGNED_INTEGER);
  params.emplace_back("WritableDouble",
      "Read and Writable Double Metric",
      ElementType::WRITABLE,
      DataType::DOUBLE);
  params.emplace_back("WritableTime",
      "Read and Writable Time Metric",
      ElementType::WRITABLE,
      DataType::TIME);
  params.emplace_back("WritableOpaque",
      "Read and Writable Opaque Metric",
      ElementType::WRITABLE,
      DataType::OPAQUE);
  params.emplace_back("WritableString",
      "Read and Writable String Metric",
      ElementType::WRITABLE,
      DataType::STRING);
  params.emplace_back("Group", "Group", ElementType::GROUP);
  return params;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(DeviceElementTests,
    DeviceElementParametersTests,
    ::testing::ValuesIn(makeDeviceElementTestParameters()),
    SetDeviceElementTestNameSuffix());
