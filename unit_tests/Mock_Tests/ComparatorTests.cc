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

TestElementInfoPtr makeReadable(const string& parent_ref_id = "",
    const string& name = "readable",
    const string& desc = "readable element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(
      DataType::BOOLEAN, DeviceBuilderInterface::Reader());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

struct ReadableTestParam : Comparator_TestType<DeviceElement> {
  ReadableTestParam() : Comparator_TestType("Readable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeReadable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeReadable(
        "", "another-readable", "Just another readable element mock"));
  }
};

TestElementInfoPtr makeWriteOnly(const string& parent_ref_id = "",
    const string& name = "write-only",
    const string& desc = "write-only element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(
      DataType::BOOLEAN, DeviceBuilderInterface::Writer());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

struct WriteOnlyTestParam : Comparator_TestType<DeviceElement> {
  WriteOnlyTestParam() : Comparator_TestType("WriteOnly") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWriteOnly());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeWriteOnly(
        "", "another-write-only", "Just another write-only element mock"));
  }
};

TestElementInfoPtr makeWritable(const string& parent_ref_id = "",
    const string& name = "read&write",
    const string& desc = "read&write element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(DataType::BOOLEAN,
      DeviceBuilderInterface::Reader(),
      DeviceBuilderInterface::Writer());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

struct WritableTestParam : Comparator_TestType<DeviceElement> {
  WritableTestParam() : Comparator_TestType("Writable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeWritable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeWritable(
        "", "another-writable", "Just another writable element mock"));
  }
};

TestElementInfoPtr makeExecutable(const string& parent_ref_id = "",
    const string& name = "executable",
    const string& desc = "executable element mock") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality(DataType::BOOLEAN,
      DeviceBuilderInterface::Executor(),
      DeviceBuilderInterface::Canceler());
  return make_shared<TestElementInfo>(meta_info, functionality);
}

struct ExecutableTestParam : Comparator_TestType<DeviceElement> {
  ExecutableTestParam() : Comparator_TestType("Executable") {}

  DeviceElementPtr make() {
    return builder.build<DeviceElement>(makeExecutable());
  }

  DeviceElementPtr makeAnother() {
    return builder.build<DeviceElement>(makeExecutable(
        "", "another-executable", "Just another executable element mock"));
  }
};

TestElementInfoPtr makeEmptyGroup(const string& parent_ref_id = "",
    const string& name = "empty-group",
    const string& desc = "Group element mock without any elements") {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);

  return make_shared<TestElementInfo>(meta_info);
}

enum class TestElementType {
  READABLE,
  WRITE_ONLY,
  WRITABLE,
  OBSERVABLE,
  EXECUTABLE
};

TestElementInfoPtr makeSubElement(
    TestElementType type, const string& parent_ref_id) {
  switch (type) {
  case TestElementType::READABLE: {
    return makeReadable(parent_ref_id);
  }
  case TestElementType::WRITE_ONLY: {
    return makeWriteOnly(parent_ref_id);
  }
  case TestElementType::WRITABLE: {
    return makeWritable(parent_ref_id);
  }
  case TestElementType::EXECUTABLE: {
    return makeExecutable(parent_ref_id);
  }
  default: { throw logic_error(""); }
  }
}

vector<TestElementInfoPtr> makeSubElements(
    vector<TestElementType> subelement_types, const string& parent_ref_id) {
  vector<TestElementInfoPtr> subelements;
  for (auto subelement_type : subelement_types) {
    subelements.push_back(makeSubElement(subelement_type, parent_ref_id));
  }
  return subelements;
}

TestElementInfoPtr makeSingleLevelGroup(const string& parent_ref_id = "",
    const string& name = "1-level-group",
    const string& desc =
        "Group element mock containing only Metric/Function elements",
    vector<TestElementType> subelement_types =
        {// clang-format off
          TestElementType::READABLE,
          TestElementType::WRITE_ONLY,
          TestElementType::WRITABLE,
          TestElementType::EXECUTABLE
        } // clang-format on
) {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality();

  return make_shared<TestElementInfo>(meta_info,
      functionality,
      makeSubElements(subelement_types, parent_ref_id));
}

struct EmptyDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  EmptyDeviceElementGroupTestParam() : Comparator_TestType("EmptyGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeEmptyGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup("",
        "another-group",
        "Just another simple element group mock",
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
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeSingleLevelGroup("",
        "another-group",
        "Just another simple element group mock",
        subelement_types));
  }
};

TestElementInfoPtr makeNestedGroup(const string& parent_ref_id = "",
    const string& name = "nested-group",
    const string& desc = "Nested Group element mock",
    vector<TestElementType> subsubelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE},
    vector<TestElementType> subelement_types = {TestElementType::READABLE,
        TestElementType::WRITE_ONLY,
        TestElementType::WRITABLE,
        TestElementType::EXECUTABLE}) {
  auto meta_info = ElementMetaInfo(parent_ref_id, name, desc);
  auto functionality = DeviceMockBuilder::Functionality();
  vector<TestElementInfoPtr> elements = {makeSingleLevelGroup(
      "0", "subelement-group", "Subelement Group mock", subsubelement_types)};
  auto subelements = makeSubElements(subelement_types, parent_ref_id);
  elements.insert(elements.end(), subelements.begin(), subelements.end());
  return make_shared<TestElementInfo>(meta_info, functionality, elements);
}

struct ComplexDeviceElementGroupTestParam
    : Comparator_TestType<DeviceElementGroup> {
  ComplexDeviceElementGroupTestParam() : Comparator_TestType("ComplexGroup") {}

  DeviceElementGroupPtr make() {
    return builder.build<DeviceElementGroup>(makeNestedGroup());
  }

  DeviceElementGroupPtr makeAnother() {
    vector<TestElementType> subsubelement_types = {
        TestElementType::WRITE_ONLY, TestElementType::EXECUTABLE};
    vector<TestElementType> subelement_types = {
        TestElementType::READABLE,
        TestElementType::WRITABLE,
    };
    return builder.build<DeviceElementGroup>(makeNestedGroup("",
        "another-nested-group",
        "Just another nested group element mock",
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
    ReadableTestParam,
    WriteOnlyTestParam,
    ExecutableTestParam,
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

TEST(DeviceElementGroupComparisonTests, emptyGroupsAreEqual) {
  auto builder = TestElementBuilder();
  auto empty_group_info = make_shared<TestElementInfo>(
      ElementMetaInfo("", "empty-group", "Just an empty group mock"));
  auto first_group = builder.build<DeviceElementGroup>(empty_group_info);
  builder.resetBuilder();
  auto second_group = builder.build<DeviceElementGroup>(empty_group_info);

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_EQ(*first_group, *second_group)
      << "First group is not equal to the second one";
}

TEST(DeviceElementGroupComparisonTests, emptyGroupIsNotEqualToNotEmpty) {
  auto builder = TestElementBuilder();
  auto empty_group_info = make_shared<TestElementInfo>(
      ElementMetaInfo("", "empty-group", "Just an empty group mock"));
  auto first_group = builder.build<DeviceElementGroup>(empty_group_info);
  builder.resetBuilder();
  auto second_group = builder.build<DeviceElementGroup>(makeSingleLevelGroup());

  EXPECT_NE(first_group, second_group)
      << "First group points to the same pointer as second group";
  EXPECT_NE(*first_group, *second_group)
      << "First group is equal to the second one";
}
