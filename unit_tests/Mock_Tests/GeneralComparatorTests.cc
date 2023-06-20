#include "TestElementBuilder.hpp"

#include "gtest/gtest.h"

using namespace Information_Model;
using namespace Information_Model::testing;
using namespace std;

template <typename ElementType> struct Comparator_TestType {
  using ElementTypePtr = shared_ptr<ElementType>;

  Comparator_TestType(const string& name_) : name(name_) {}

  ElementTypePtr makeEmpty() { return ElementTypePtr(); }

  ElementTypePtr make() {
    throw logic_error("Requested Element type is not supported");
  }

  ElementTypePtr makeAnother() {
    throw logic_error("Requested Element type is not supported");
  }

  const string& name;

protected:
  TestElementBuilder builder = TestElementBuilder();
};

struct SameElementTypeTestParam : Comparator_TestType<DeviceElement> {
  SameElementTypeTestParam() : Comparator_TestType("SameElementType") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    // if we do not reset the builder, NamedElement refID_ values will be
    // different, thus elements will not be the same
    return builder.build<DeviceElement>(makeReadable());
  }
};

struct ReadableTestParam : Comparator_TestType<DeviceElement> {
  ReadableTestParam() : Comparator_TestType("Readable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeReadable(DataType::DOUBLE));
  }
};

struct WriteOnlyTestParam : Comparator_TestType<DeviceElement> {
  WriteOnlyTestParam() : Comparator_TestType("WriteOnly") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWriteOnly());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWriteOnly(DataType::INTEGER));
  }
};

struct WritableTestParam : Comparator_TestType<DeviceElement> {
  WritableTestParam() : Comparator_TestType("Writable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWritable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(DataType::STRING));
  }
};

struct ExecutableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableTestParam() : Comparator_TestType("Executable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeExecutable(DataType::OPAQUE));
  }
};

struct ExecutableNotWritableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableNotWritableTestParam()
      : Comparator_TestType("ExecutableNotWritable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable(meta_info));
  }

  DeviceElementPtr makeAnother() {
    builder.resetBuilder();
    return builder.build<DeviceElement>(makeWritable(meta_info));
  }

private:
  const ElementMetaInfo meta_info = ElementMetaInfo(
      "", "executableNotWritable", "Just another executable element mock");
};

struct EmptyDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  EmptyDeviceElementGroupTestParam() : Comparator_TestType("EmptyGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeEmptyGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        makeSingleLevelGroupInfo(
            "", "another-group", "Just another simple element group mock"),
        subelement_types));
  }
};

struct SimpleDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  SimpleDeviceElementGroupTestParam() : Comparator_TestType("SimpleGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup(
        makeSingleLevelGroupInfo(
            "", "another-group", "Just another simple element group mock"),
        subelement_types));
  }
};

struct ComplexDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  ComplexDeviceElementGroupTestParam() : Comparator_TestType("ComplexGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeNestedGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    builder.resetBuilder();
    vector<TestElementType> subsubelement_types = {
        TestElementType::WRITE_ONLY, TestElementType::EXECUTABLE};
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(
        makeNestedGroup(makeNestedGroupInfo("",
                            "another-nested-group",
                            "Just another nested group element mock"),
            subsubelement_types,
            subelement_types));
  }
};

struct SimpleDeviceTestParam : Comparator_TestType<Device> {
  SimpleDeviceTestParam() : Comparator_TestType("SimpleDevice") {}

  DevicePtr make() { return builder.build<Device>(makeSingleLevelGroup()); }

  DevicePtr makeAnother() {
    builder = TestElementBuilder(
        ElementMetaInfo("23456", "AnotherMocky", "It's just another mocked"));
    return builder.build<Device>(makeSingleLevelGroup());
  }
};

struct ComplexDeviceTestParam : Comparator_TestType<Device> {
  ComplexDeviceTestParam() : Comparator_TestType("ComplexDevice") {}

  DevicePtr make() { return builder.build<Device>(makeNestedGroup()); }

  DevicePtr makeAnother() {
    builder = TestElementBuilder(
        ElementMetaInfo("23456", "AnotherMocky", "It's just another mocked"));
    return builder.build<Device>(makeNestedGroup());
  }
};

template <typename T> struct Comparator_TestSuite : public ::testing::Test {
  Comparator_TestSuite() : param(T()) {}

  T param; // NOLINT(readability-identifier-naming)
};

using ComparatorTestTypes = ::testing::Types< //
    SameElementTypeTestParam,
    ReadableTestParam,
    WriteOnlyTestParam,
    ExecutableTestParam,
    ExecutableNotWritableTestParam,
    EmptyDeviceElementGroupTestParam,
    SimpleDeviceElementGroupTestParam,
    ComplexDeviceElementGroupTestParam,
    SimpleDeviceTestParam,
    ComplexDeviceTestParam>;

TYPED_TEST_SUITE(Comparator_TestSuite, ComparatorTestTypes);

TYPED_TEST(Comparator_TestSuite, isEqual) {
  auto empty = this->param.makeEmpty();

  EXPECT_TRUE(empty ? false : true) << " comparator empty ptr is not empty";
  EXPECT_EQ(empty, empty) << " comparator empty ptr is not equal to itself";

  auto tested = this->param.make();

  EXPECT_EQ(tested, tested)
      << this->param.name
      << " comparator tested ptr does not point to the same ptr";

  EXPECT_EQ(*tested, *tested)
      << this->param.name
      << " comparator tested value does not contain the same value";
}

TYPED_TEST(Comparator_TestSuite, isNotEqual) {
  auto tested = this->param.make();
  auto empty = this->param.makeEmpty();
  EXPECT_NE(tested, empty)
      << this->param.name
      << " comparator tested ptr is the same as an empty ptr";

  auto another = this->param.makeAnother();
  EXPECT_NE(tested, another)
      << this->param.name
      << " comparator tested ptr is the same as another ptr";
  EXPECT_NE(*tested, *another)
      << this->param.name
      << " comparator tested value is the same as another value";
}
